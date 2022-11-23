const {app, BrowserWindow, ipcMain, shell} = require('electron')
const path = require('path')

function createWindow () {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true,
    }
  })
  mainWindow.loadFile(path.join(__dirname, 'index.html'))
}

ipcMain.on('openFile', (event, arg) => {
//  console.log(event.senderFrame.url);
  shell.openExternal(arg.path);
});

app.enableSandbox()
app.whenReady().then(() => {
  createWindow()

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})