
const {contextBridge, ipcRenderer} = require('electron')

//const { Chart, LinearScale, PointElement, Tooltip, Legend, TimeScale } = require("chart.js");


//contextBridge.exposeInMainWorld('Chart', Chart);
contextBridge.exposeInMainWorld('ipcRenderer', {
    send: (channel, data) => ipcRenderer.send(channel, data),
    on: (channel, func) => ipcRenderer.on(channel, func),
  })

/*contextBridge.exposeInMainWorld('chart', {
   createChart: (ctx, options) => new Chart(ctx, options),
   register: (...items) => Chart.register(...items),
   LinearScale: LinearScale,
   TimeScale: TimeScale,
});*/

/*import('chartjs-adapter-date-fns').then(dateFns => {
  contextBridge.exposeInMainWorld('dateFns', dateFns);
}).catch(error => {
  console.error('Failed to load chartjs-adapter-date-fns:', error);
});*/