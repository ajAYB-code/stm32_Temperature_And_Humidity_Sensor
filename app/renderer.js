
let tempProgressEle = document.querySelector(".circular-progress.temperature");
let tempTextEle = document.querySelector(".circular-progress.temperature + .percent-text span.number");
let humdProgressEle = document.querySelector(".circular-progress.humidity");
let humdTextEle = document.querySelector(".circular-progress.humidity + .percent-text span.number");
const maxTempValue = 125;
const maxHumidValue = 100;

// Function to calculate gradient color based on temperature
function getTemperatureGradientColor(temperature) {
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

// Function to calculate gradient color based on humidity
function getHumidityGradientColor(humidity) {
  // Example logic: green for low humiditys, yellow for moderate, red for high
  if (humidity < 60) {
    return 'url(#blue)';
  } else if (humidity < 80) {
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
    tempProgressEle.querySelector('circle.fg').style.stroke = getTemperatureGradientColor(temp);
}

function updateHumidityProgressBar(humd){
    // Calculate percentage
    let progress = humd / maxHumidValue * 100;
    humdProgressEle.style.setProperty('--progress', progress);
    humdTextEle.innerText = humd;
    humdProgressEle.querySelector('circle.fg').style.stroke = getHumidityGradientColor(humd);
}

// Listen to receiving current sensor data

ipcRenderer.on('current-sensor-data', (event, data) => {
    updateTemperatureProgressBar(data.temp);
    updateHumidityProgressBar(data.humid);
});

// fetch temperature/humidity data based on specified time interval

function fetchTemperatureData(){
  ipcRenderer.send('fetch-sensor-data', {column: 'temperature'});
}

function fetchHumidityData(){
  ipcRenderer.send('fetch-sensor-data', {column: 'humidity'});
}

// Update temperature/humidity graph

function updateTemperatureGraph(data){
  const timestamps = data.map(entry => entry.timestamp);
  const temperatures = data.map(entry => entry.temperature);

  // Render the chart
  const cx = document.getElementById('temperatureChart');
  Chart.register(Chart.LinearScale, Chart.TimeScale);
  const temperatureChart =  new Chart(cx, {
        type: 'line',
        data: {
            labels: timestamps,
            datasets: [{
                label: 'Temperature (°C)',
                data: temperatures,
                borderColor: 'blue',
                fill: false
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x:  {
                  type: 'time',
                  time: {
                    unit: 'day'
                  },
                  display: true,
                  title: {
                    display: true,
                    text: 'Date'
                  },
                y:{
                  display: true,
                  title: {
                    display: true,
                    text: 'value'
                  }
               }
        }
      }
      }});
}

// listen temperature/humidity fetched data based on specified time interval

ipcRenderer.on('fetched-sensor-data', (event, data) => {
  console.log(data)
  if(data.column == 'temperature'){
    updateTemperatureGraph(data.list);
  } else {
    updateHumidityGraph(data.list);
  }
});
setTimeout(() => {
  fetchTemperatureData();
}, 5000)
