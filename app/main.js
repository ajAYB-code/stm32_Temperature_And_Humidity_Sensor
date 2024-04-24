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
          temp = parseInt(temp);
          humd = parseInt(humd);

          //Save to database
          con.query(`INSERT INTO weather(temperture,humidity) VALUES (${temp},${humd})`, function (error, results) {
            if (error) throw error;
            
          });

          mainWindow.webContents.send('sensor-data', {temp: temp, humid: humd});
          receivedData = '';
        }
      });

      port.on('error', function(err) {
        console.error('Error:', err.message);
      });

    })
    
  })
 