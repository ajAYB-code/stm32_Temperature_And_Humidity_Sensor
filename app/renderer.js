
let tempProgressEle = document.querySelector(".circular-progress.temperature");
let tempTextEle = document.querySelector(".circular-progress.temperature + .percent-text span.number");
let humdProgressEle = document.querySelector(".circular-progress.humidity");
let humidTextEle = document.querySelector(".circular-progress.humidity .percent-text span.number");
const maxTempValue = 125;
const maxHumidValue = 100;

// Function to calculate gradient color based on temperature
function getGradientColor(temperature) {
  // Example logic: green for low temperatures, yellow for moderate, red for high
  if (temperature < 10) {
    return 'url(#blue)';
  } else if (temperature < 25) {
    return 'url(#green)';
  } else if (temperature < 50) {
    return 'url(#yellow)';
  } else {
    return 'url(#red)';
  }
}

function updateTemperatureProgressBar(temp){
    // Calculate percentage
    let progress = temp / maxTempValue * 100;
    tempProgressEle.style.setProperty('--progress', progress);
    tempTextEle.innerText = temp;
    tempProgressEle.querySelector('circle.fg').style.stroke = getGradientColor(temp);
}

function updateHumidityProgressBar(humd){
    // Calculate percentage
    let progress = humd / maxHumidValue * 100;
    humdProgressEle.style.setProperty('--progress', progress);
    humidTextEle.innerText = humd;
}

// Listen to receiving data

ipcRenderer.on('sensor-data', (event, data) => {
    updateTemperatureProgressBar(data.temp);
    updateHumidityProgressBar(data.humid);
  });

setTimeout(() => {
  updateTemperatureProgressBar(30)
}, 2000);
setTimeout(() => {
  updateTemperatureProgressBar(60)
}, 4000);
setTimeout(() => {
  updateTemperatureProgressBar(10)
}, 7000);
setTimeout(() => {
  updateTemperatureProgressBar(50)
}, 10000);
