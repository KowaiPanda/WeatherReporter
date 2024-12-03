const {app, BrowserWindow, ipcMain} = require('electron')
const weatherService = require('./build/Release/weatherService.node');
const path = require('path');

const createWindow = () => {
    const win = new BrowserWindow({
        width: 800,
        height: 640,
        maximizable: false,
        resizable: false,
        webPreferences: {
          nodeIntegration: false,
          contextIsolation: true,
          preload: path.join(__dirname, 'preload.js')
        }
    })
    win.loadFile('index.html')
    win.removeMenu()
}

ipcMain.handle('weather:getWeather', async (event, city) => {
  try {
    const weatherDataString = weatherService.getWeather(city);
    const weatherData = JSON.parse(weatherDataString);
    return weatherData;
  } catch (error) {
    console.error('Main process: Weather service error', error);
    throw error;
  }
});


app.whenReady().then(() => {
    createWindow()
    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
          createWindow()
        }
      })
    app.on('window-all-closed', () => {
      if (process.platform !== 'darwin') {
        app.quit()
      }
    })
})