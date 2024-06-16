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
const dashSubmitBtns = Array.from(document.querySelectorAll('.tab.dash .submit-btn'));



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
for (let ele of dashSubmitBtns){
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

// Handle tabs 

const tabLinks = document.querySelectorAll('.tab-link');
const tabs = document.querySelectorAll('.tab')

tabLinks.forEach(link => {
  link.addEventListener('click', () =>{
    tabLinks.forEach(ele => {
      ele.classList.remove('active');
    })

    link.classList.add('active')
    const tabName = link.getAttribute('data-tab');
    // Dispaly tab
    tabs.forEach(tab => {
      tab.classList.remove('show')
      if(tab.classList.contains(tabName))
        tab.classList.add('show')
    })

  })
})

// Alarm form
const alarmForm = document.querySelector('.alarm-form-container form');
const submitAlarmBtn = document.querySelector('.alarm-form-container form .submit-btn');
const closeFormBtn = document.querySelector('.alarm-form-container form .close-btn');
const createAlamrBtn = document.querySelector('.tab.alarms .create-alarm-btn');
const formError = document.querySelector('.tab.alarms form p.form-error');

createAlamrBtn.addEventListener('click', event => {
  event.target.style.display = 'none';
  alarmForm.style.display = 'block'
})
closeFormBtn.addEventListener('click', event => {
 createAlamrBtn.style.display = 'initial';
  alarmForm.style.display = 'none'
})

// Validate form data
function validForm(data){
  // Valid name input
  if(!data.name) return false

  // Valid valueType input
  if(!data.valueType) return false

  // Valid min value and max value
  if(!data['min-value'] && !data['max-value']) return false

  // Valid ring
  // if(data.ring == null) return false

  // Valid tel
  // if(!data.tel || (''+data.tel).length != 10) return false

  return true
}

let resetFormBtn = document.getElementById('resetForm');
// Submit alarm form

submitAlarmBtn.addEventListener('click', event =>{
  event.preventDefault();
  const formData = new FormData(alarmForm);

  // Convert FormData to an object for easier manipulation
  const formKeys = ['name', 'min-value', 'max-value', 'valueType', 'measureType']
  const formObject = {};
  formKeys.forEach((key) => {

    if(formData.get(key))
      formObject[key] = formData.get(key);
    else formObject[key] = null;
  });

  // Validate form
  if(!validForm(formObject)){
    formError.style.display = 'initial';
    return false;
  }

  formError.style.display = 'none';


  // Submit data
  console.log(formObject)

  ipcRenderer.send('create-alert', formObject);
  
  resetFormBtn.click()

  // Show success message
  const Toast = Swal.mixin({
    toast: true,
    position: "top-end",
    showConfirmButton: false,
    timer: 5000,
    timerProgressBar: true,
    didOpen: (toast) => {
      toast.onmouseenter = Swal.stopTimer;
      toast.onmouseleave = Swal.resumeTimer;
    }
  });
  Toast.fire({
    icon: "success",
    title: "Alerte ajouté avec succès"
  });
  closeFormBtn.click();
})

// Handle when disable min/max input 
const thresholdValueTypeInpts = document.querySelectorAll('.alarm-form-container form input[name="valueType"]');

thresholdValueTypeInpts.forEach(ele => {
  ele.addEventListener('change', e => {
    const minInput = document.querySelector('.alarm-form-container form input[name="min-value"]');
    const maxInput = document.querySelector('.alarm-form-container form input[name="max-value"]');
    
    minInput.parentElement.style.display = 'none';
    maxInput.parentElement.style.display = 'none';

    if(e.target.value == 'min')
      minInput.parentElement.style.display = 'initial';
    else 
      maxInput.parentElement.style.display = 'initial'

  })
})

// Listen to alerts

ipcRenderer.on('alert', (event, data) => {
  
  let text = document.createElement('p');
  let iconUrl;
  console.log(data)
  // It's just temperature alarms
  if(data['temperature'].length > 0 && data['humidity'].length == 0){
    data['temperature'].forEach(alarm => {
      text.innerHTML += '-' + alarm.message + '<br>' 
    })
    iconUrl = './assets/icons/high-temperature.png';
  }

    // It's just temperature alarms
  if(data['temperature'].length == 0 && data['humidity'].length > 0){
    data['temperature'].forEach(alarm => {
      text.innerHTML += '-' + alarm.message + '<br>' 
    })
    iconUrl = './assets/icons/humidity.png';
  }

    // It's both temprature and humidity alert
  if(data['temperature'].length > 0 && data['humidity'].length > 0){
    data['temperature'].forEach(alarm => {
      text.innerHTML += '-' + alarm.message + '<br>' 
    })
    iconUrl = './assets/icons/danger.png';
  }

  // Play alert sound
  var alertSound = document.getElementById('alertSound');
  alertSound.play();

  Swal.fire({
    title: "<span style='color: red;'>Alerte!</span>",
    html: text,
    imageUrl: iconUrl,
    imageWidth: 128,
    imageHeight: 128,
    imageAlt: "Custom image",
    confirmButtonText: 'Fermer'
    }).then(() => {
      // Stop the audio when the SweetAlert is closed
      alertSound.pause();
      alertSound.currentTime = 0; // Reset the audio to the beginning
    })
})

// Function to generate HTML for an alert card

function createAlertCard(alert) {
  let card = document.createElement("div");
  card.className = "card";
  card.setAttribute("id", alert.id)
  let header = document.createElement('div');
  header.setAttribute('style', 'display:flex;justify-content:space-between;align-items: center');
  let title = document.createElement("h3");
  header.appendChild(title);
  let toggleDiv = document.createElement('div');
  let label = document.createElement('label');
  label.setAttribute('class', 'switch')
  let check = document.createElement('input')
  check.setAttribute('type', 'checkbox')
  if(alert.enabled){
    check.setAttribute('checked', '')
  }

  //Enable / Disable alert
  check.addEventListener('change', e => {
    let toggleVal = 0;

    if(e.target.checked){
      toggleVal = 1;
    } 

    ipcRenderer.send('toggle-alert', {toggle:  toggleVal, id: alert.id})
  })

  let span = document.createElement('span');
  span.setAttribute('class', 'slider round');
  label.appendChild(check)
  label.appendChild(span)
  toggleDiv.appendChild(label)
  // toggleDiv.innerHTML =  `
  //   <label class="switch">
  // <input type="checkbox" checked onchange='foo(this)'>
  // <span class="slider round"></span>
  // </label>`

  title.textContent = alert.name;
  header.appendChild(title);
  header.appendChild(toggleDiv);
  card.appendChild(header);

  let measureType = document.createElement("p");
  measureType.innerHTML = "Mesure: " + "<span class='second-text'>" + alert.measure + '</span>';
  card.appendChild(measureType);

  let symbol = alert.measure == 'temperature' ? '°' : '%'

  if(alert.min_value){
    let minValue = document.createElement("p");
    minValue.innerHTML = alert.min_value ?  "Min: " + "<span class='second-text'>" + alert.min_value + symbol + '</span>' : "";
    card.appendChild(minValue);
  }

  if(alert.max_value){
    let maxValue = document.createElement("p");
    maxValue.innerHTML =  alert.max_value ?  "Max: " +  "<span class='second-text'>" + alert.max_value + symbol + '</span>' : "-";
    card.appendChild(maxValue);
  }

  let date = document.createElement("p");
  date.innerHTML = "Date: " + "<span class='second-text'>" + alert.created_at+'</span>';
  card.appendChild(date);

  let deleteBtn = document.createElement("button");
  deleteBtn.setAttribute("id", "deleteAlert")
  deleteBtn.setAttribute("id-alert", alert.id)
  deleteBtn.textContent = "Supprimer";

  //Delete button logic
  deleteBtn.addEventListener("click", function(e) {
    let idAlert = e.target.getAttribute("id-alert");
    ipcRenderer.send('delete-alert', {id: idAlert});
  
    // Show success message
    const Toast = Swal.mixin({
      toast: true,
      position: "top-end",
      showConfirmButton: false,
      timer: 5000,
      timerProgressBar: true,
      didOpen: (toast) => {
        toast.onmouseenter = Swal.stopTimer;
        toast.onmouseleave = Swal.resumeTimer;
      }
    });
    Toast.fire({
      icon: "success",
      title: "Alerte supprimeé avec succès"
    });
    
  });

  card.appendChild(deleteBtn);

  return card;
}

// Listen to fetch alerts list
ipcRenderer.on('fetched-alerts', (event, data) => {
  renderAlerts(data);
})

// Function to render alert cards
function renderAlerts(alerts) {
  let alertList = document.querySelector(".alarms-list .list");
  alertList.innerHTML = ""; // Clear previous content

  alerts.forEach(alert => {
      let card = createAlertCard(alert);
      alertList.appendChild(card);
  });
}