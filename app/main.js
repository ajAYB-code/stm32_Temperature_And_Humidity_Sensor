const { app, BrowserWindow, ipcMain, ipcRenderer } = require('electron')
const path = require('node:path')
const SerialPort = require('serialport').SerialPort;
var mysql = require('mysql');

let mainWindow;

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


// Receive data from sensor using serial port

app.whenReady().then(() => {
    createMainWindow()

    // Init serial port
    const port = new SerialPort({
      path : 'COM3', 
      baudRate: 115200
    });

    // Init database connection
    const con = mysql.createConnection({
      host: "localhost",
      user: "root",
      password: "",
      database: "project_stm32"
    });

    mainWindow.webContents.on("did-finish-load", () => {

      con.connect();

      // read sensor data
      let receivedData = ''; 
      let temp, humd;

      port.on('data', function(data) {
        receivedData += data.toString(); 
        if (receivedData.includes('\n')) {
          [temp, humd] = receivedData.split('-');
          temp = parseFloat(temp).toFixed(1);
          humd = parseInt(humd);
          const isValidTempratureValue = temp >= -40 && temp <= 125;
          const isValidHumidityValue = humd >= 0 && humd <= 100;
          if(!isNaN(temp) && !isNaN(humd) && isValidHumidityValue && isValidTempratureValue){
            con.query(`INSERT INTO weather(temperature,humidity) VALUES (${temp},${humd})`, function (error, results) {
              if (error) console.log(error);
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
        let sql = `SELECT timestamp,${data.column} FROM weather`;
        con.query(sql, function (error, results) {
          if (error) console.log(error);
          console.log(results)
          mainWindow.webContents.send('fetched-sensor-data', {list: results, column: data.column});
        });
        
      })

    })
    
  })
 