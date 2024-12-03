const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('weatherService', {
  getWeather: async (city) => {
    try {
      const weatherData = await ipcRenderer.invoke('weather:getWeather', city);
      return weatherData;
    } catch (error) {
      console.error('Preload: Weather retrieval error', error);
      throw error;
    }
  }
});