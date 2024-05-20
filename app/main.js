const { app, BrowserWindow, ipcMain, ipcRenderer } = require('electron')
const path = require('node:path')
const SerialPort = require('serialport').SerialPort;
const sqlite3 = require('sqlite3').verbose();

let mainWindow;
let alarmsList;
let db;

// Load alarms
function loadAlarms(){
  db.all('select * from alarms', [], (err, rows) => {
    if (err) {
      console.error(err.message);
      return;
    }

    alarmsList = rows;
  });
}

const createMainWindow = () => {
     mainWindow = new BrowserWindow({
      width: 800,
      height: 600,
      webPreferences: {
        contextIsolation: true,
        nodeIntegration: true,
        preload: path.join(__dirname, 'preload.js')
      }
    })
  
    mainWindow.loadFile('index.html')

    mainWindow.on('closed', function () {
      mainWindow = null
    })
  }

app.whenReady().then(() => {
    createMainWindow()

    // Init serial port
    const port = new SerialPort({
      path : 'COM3', 
      baudRate: 115200
    });

    // Init database
    db = new sqlite3.Database('../sensor_database.db', function(err){
      if(err){
        console.log('error opening database');
        process.exit(1);
      }
      
    });

    // Load alarms list
    loadAlarms();

    mainWindow.webContents.on("did-finish-load", () => {

      // read sensor data
      let receivedData = ''; 
      let temp, humd;

      port.on('data', function(data) {
        receivedData += data.toString(); 
        if (receivedData.includes('\n')) {
          [temp, humd] = receivedData.split('-');
          temp = parseFloat(temp).toFixed(1);
          humd = parseInt(humd);
              let temp = 40.5;
              let humd = 50;

          const isValidTempratureValue = temp >= -40 && temp <= 125;
          const isValidHumidityValue = humd >= 0 && humd <= 100;

          // Check alarm
          const temperatureAlarms = [];
          const humidityAlarms = [];
          
          alarmsList.forEach(alarm => {
            let humidityAlarm = {message: []};
            let temperatureAlarm = {message: []};

            if(alarm['measure'] == 'temperature'){
              if(alarm['min_value'] && temp < alarm['min_value']){
                temperatureAlarm.message.push(`Température est en dessous de ${alarm['min_value']}°`)
              }
              if(alarm['max_value'] && temp > alarm['max_value']){
                temperatureAlarm.message.push(`Température est au dessus de ${alarm['max_value']}°`)
              } 
            }
          
            if(alarm['measure'] == 'humidity'){
              if(alarm['min_value'] && humd < alarm['min_value']){
                humidityAlarm.message.push(`Humidité est en dessous de ${alarm['min_value']}%`)
              }
              if(alarm['max_value'] && humd > alarm['max_value']){
                humidityAlarm.message.push(`Humidité est au dessus de ${alarm['max_value']}%`)
              }
            }
          
            if(temperatureAlarm.message.length > 0){
              temperatureAlarms.push(temperatureAlarm)
            }
                          
          if(humidityAlarm.message.length > 0){
              humidityAlarms.push(humidityAlarm)
            }
          });
          
          if(temperatureAlarms.length > 0 || humidityAlarms.length > 0){
            mainWindow.webContents.send('alert', {temperature: temperatureAlarms, humidity: humidityAlarms})
          }

          if(!isNaN(temp) && !isNaN(humd) && isValidHumidityValue && isValidTempratureValue){
            db.serialize(() => {
              db.run('INSERT INTO sensor_data  VALUES (?, ?, ?)', [new Date().toISOString().replace('T', ' ').slice(0, 19), temp, humd], function(err){
                if(err){
                  return console.log(err.message)
                }
              });
            });
  
            mainWindow.webContents.send('current-sensor-data', {temp: temp, humid: humd});
          }
          receivedData = '';
        }
      });

      port.on('error', function(err) {
        console.error('Error:', err.message);
      });

      // Listen to fetch data
      ipcMain.on('fetch-sensor-data', (event, data) => {

        // Fetch data for different time interval
        let sql = ''

        switch(data.timeInterval){

        // Last 24 hours
         case '24 hours':
            sql += `SELECT 
                      strftime('%H', timestamp) AS time,
                      AVG(${data.column}) as ${data.column}
                      FROM 
                      sensor_data
                      WHERE 
                      timestamp >= datetime('now', '-24 hours')
                      GROUP BY time;`; break;

          // Last week
          case 'last week':
            sql += `SELECT 
                      CASE strftime('%w', timestamp)
                        WHEN '0' THEN 'Dimanche'
                        WHEN '1' THEN 'Lundi'
                        WHEN '2' THEN 'Mardi'
                        WHEN '3' THEN 'Mercredi'
                        WHEN '4' THEN 'Jeudi'
                        WHEN '5' THEN 'Vendredi'
                        WHEN '6' THEN 'Samedi'
                      END AS time,
                      AVG(${data.column}) as ${data.column}
                      FROM
                      sensor_data
                      WHERE 
                      timestamp >= datetime('now', '-7 days')
                      GROUP BY time;`; break;
        
        // Last month
        case 'last month':
            sql += `SELECT 
                      strftime('%d', timestamp) as time,
                      AVG(${data.column}) as ${data.column}
                      FROM 
                      sensor_data
                      WHERE 
                      timestamp >= datetime('now', '-30 days')
                      GROUP BY time;`; break;

        // Defined time interval
        case 'custom':
            sql += `SELECT 
                      strftime('%Y-%m-%d', timestamp) as time,
                      AVG(${data.column}) as ${data.column}
                      FROM 
                      sensor_data
                      WHERE 
                      time >= '${data.minTime}' AND time <= '${data.maxTime}'
                      GROUP BY time;`;console.log(data.maxTime); break;
        }

        db.all(sql, [], (err, rows) => {
          if (err) {
              console.error(err.message);
              return;
          }

          // Process the retrieved rows
          mainWindow.webContents.send('fetched-sensor-data', {list: rows, column: data.column});
        });
        
      })

      // Create new alert
      ipcMain.on('create-alert', (event, data) => {
        // Parse numeric values
        let min = !isNaN(parseFloat(data['min-value']).toFixed(1)) ?  parseFloat(data['min-value']).toFixed(1) : null;
        let max = !isNaN(parseFloat(data['max-value']).toFixed(1)) ?  parseFloat(data['max-value']).toFixed(1) : null;
        let ring = parseFloat(data['ring'])
        db.run('INSERT INTO alarms  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)', [null, data['name'], data['measureType'], min, max, 1, new Date().toISOString().replace('T', ' ').slice(0, 19), data['tel'], ring], function(err){
            if(err){
              return console.log(err.message)
            }

            // Load alarms again
            loadAlarms();
      })
    })

    // Close Database and Serial connection
    app.on('before-quit', () => {
      db.close();
      port.close()
    });
    
  })
})