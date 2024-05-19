
let tempProgressEle = document.querySelector(".circular-progress.temperature");
let tempTextEle = document.querySelector(".circular-progress.temperature + .percent-text span.number");
let humdProgressEle = document.querySelector(".circular-progress.humidity");
let humdTextEle = document.querySelector(".circular-progress.humidity + .percent-text span.number");
const maxTempValue = 125;
const maxHumidValue = 100;
let humidityChart;
let temperatureChart;

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

function fetchTemperatureData(interval){
  let data = {timeInterval: null, column: 'temperature', minTime: null, maxTime: null}

  if(typeof interval === 'object'){
    data.timeInterval = 'custom'
    data.minTime = interval.start;
    data.maxTime = interval.end;
  } else {
    data.timeInterval = interval
  }

  ipcRenderer.send('fetch-sensor-data', data);
}

function fetchHumidityData(interval){
  let data = {timeInterval: null, column: 'humidity', minTime: null, maxTime: null}

  if(typeof interval === 'object'){
    data.timeInterval = 'custom'
    data.minTime = interval.start;
    data.maxTime = interval.end;
  } else {
    data.timeInterval = interval
  }

  ipcRenderer.send('fetch-sensor-data', data);
}

// Update temperature/humidity graph

function updateTemperatureGraph(data){
  const timestamps = data.map(entry => entry.time);
  const temperatures = data.map(entry => entry.temperature);

  // If chart is already rendered then update
  if(temperatureChart){
    temperatureChart.data.datasets[0].data = temperatures;
    temperatureChart.data.datasets[0].labels = timestamps;
    temperatureChart.data.labels = timestamps;
    temperatureChart.update();
    return true;
  }

  // Render the chart
  const cx = document.getElementById('temperatureChart');
  Chart.register(Chart.LinearScale, Chart.TimeScale);
  temperatureChart =  new Chart(cx, {
        type: 'line',
        data: {
            labels: timestamps,
            datasets: [{
                label: 'Temperature (°C)',
                data: temperatures,
                borderColor: 'red',
                fill: true
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x:  {
                  // type: 'time',
                  // time: {
                  //   unit: 'day'
                  // },
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

// Update humidity graph
function updateHumidityGraph(data){
  const timestamps = data.map(entry => entry.time);
  const humidites = data.map(entry => entry.humidity);

  // If chart is already rendered then update
  if(humidityChart){
    humidityChart.data.datasets[0].data = humidites;
    humidityChart.data.datasets[0].labels = timestamps;
    humidityChart.data.labels = timestamps;
    humidityChart.update();
    return true;
  }

  const cx = document.getElementById('humidityChart');
  Chart.register(Chart.LinearScale, Chart.TimeScale);
  humidityChart =  new Chart(cx, {
              type: 'line',
              data: {
                  labels: timestamps,
                  datasets: [{
                      label: 'Humidité (°%)',
                      data: humidites,
                      borderColor: 'blue',
                      fill: true
                  }]
              },
              options: {
                  responsive: true,
                  maintainAspectRatio: false,
                  scales: {
                      x:  {
                        // type: 'time',
                        // time: {
                        //   unit: 'week'
                        // },
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

  if(data.column == 'temperature'){
    updateTemperatureGraph(data.list);
  } else {
    updateHumidityGraph(data.list);
  }
});

// Fetch data based on time interval selected
const dateSelectEles = Array.from(document.querySelectorAll('.date-select'));
const customDateDialogs = Array.from(document.querySelectorAll('.custom-date-dialog'));
const closeDialogBtns = Array.from(document.querySelectorAll('.close-dialog'));
const submitBtns = Array.from(document.querySelectorAll('.submit-btn'));


for (let ele of dateSelectEles){
  ele.addEventListener('change', (event) =>{
    const selectedOption = event.target.options[event.target.selectedIndex].value;
    if (selectedOption === 'custom') {
      for(let dialog of customDateDialogs){
        if(dialog.getAttribute('data-measure') == ele.getAttribute('data-measure')){
          ele.style.display = 'none';
          dialog.style.display = 'block';
        }
      }

    } else {
        if(ele.getAttribute('data-measure') == 'temperature')
          fetchTemperatureData(selectedOption);
        else fetchHumidityData(selectedOption);
    }
  })
}

// Close custom date dialog
for (let ele of closeDialogBtns){
  ele.addEventListener('click', () => {
    const measureAttr = ele.parentElement.parentElement.getAttribute('data-measure');
    for(let i = 0; i < dateSelectEles.length; i++){
      if(dateSelectEles[i].getAttribute('data-measure') == measureAttr)
        dateSelectEles[i].style.display = 'block'
      if(customDateDialogs[i].getAttribute('data-measure') == measureAttr)
        customDateDialogs[i].style.display = 'none'
    }
  });
}

// Submit date picker form
for (let ele of submitBtns){
  ele.addEventListener('click', (event) => {
    event.preventDefault();

    // Get start and end date
    const measureAttr = ele.parentElement.parentElement.getAttribute('data-measure');

    let startDateVal = document.querySelector(`.custom-date-dialog[data-measure="${measureAttr}"] #fromDate`).value;
    let endDateVal = document.querySelector(`.custom-date-dialog[data-measure="${measureAttr}"] #toDate`).value;

    let startDate = new Date(startDateVal);
    let endDate = new Date(endDateVal);
    if(!startDateVal || !endDateVal || startDate >= endDate) return false;

    // Fetch data for specific date interval
    if(measureAttr == 'temperature'){
      fetchTemperatureData({start: startDateVal, end: endDateVal});
    } else
        fetchHumidityData({start: startDateVal, end: endDateVal});

    // Close dialog and display select
    for(let i = 0; i < dateSelectEles.length; i++){
      if(dateSelectEles[i].getAttribute('data-measure') == measureAttr)
        dateSelectEles[i].style.display = 'block'
      if(customDateDialogs[i].getAttribute('data-measure') == measureAttr)
        customDateDialogs[i].style.display = 'none'
    }
  });
}

setTimeout(() => {
  fetchTemperatureData('24 hours');
  fetchHumidityData('24 hours');
}, 1500)




