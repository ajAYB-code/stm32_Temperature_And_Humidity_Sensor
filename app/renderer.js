
let tempEle = document.getElementById("temp");
let humdEle = document.getElementById("humd");

ipcRenderer.on('sensor-data', (event, data) => {
    tempEle.innerText = `${data.temp}°C`
    humdEle.innerText = `${data.humid}%`
  });