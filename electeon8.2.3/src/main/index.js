import { app, BrowserWindow,crashReporter  } from 'electron'
var path = require('path');
import compyfile from '../main/copyFile.js'
/**
 * Set `__static` path to static files in production
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-static-assets.html
 */
if (process.env.NODE_ENV !== 'development') {
  global.__static = require('path').join(__dirname, '/static').replace(/\\/g, '\\\\')
  compyfile.exists( path.resolve(path.join(__dirname,'../../../extraResources')), path.resolve(path.join(__dirname,'../../../../')), compyfile.copy )
}

let mainWindow
const winURL = process.env.NODE_ENV === 'development'
  ? `http://localhost:9080`
  : `file://${__dirname}/index.html`

function createWindow () {
  //console.log("createWindow");
  /**
   * Initial window options
   */
  mainWindow = new BrowserWindow({
    height: 563,
    useContentSize: true,
    width: 1000,
    webPreferences: {
      nodeIntegration: true,
      backgroundThrottling: false,
      nodeIntegrationInWorker: true
    },
    show: false,
    frame:false /*去掉顶部导航 去掉关闭按钮 最大化最小化按钮*/

  })

  mainWindow.loadURL(winURL)

  mainWindow.on('closed', () => {
    mainWindow = null
  })

  // crashReporter.start({
  //   productName: 'YourName',
  //   companyName: 'YourCompany',
  //   submitURL: 'https://your-domain.com/url-to-submit',
  //   uploadToServer: false
  // })
  // mainWindow.on('maximize', () => {
  //   //console.log("maximize");
  //   mainWindow.webContents.send('main-process-messages', 'browser-window-restore');
  // })
  // mainWindow.on('restore', () => {
  //   //console.log("restore");
  //   mainWindow.webContents.send('main-process-messages', 'browser-window-restore');
  // })

  // mainWindow.on('minimize', () => {
  //   //console.log("minimize");
  //   mainWindow.webContents.send('main-process-messages', 'browser-window-minimize');
  // })
  //去掉顶部菜单
  mainWindow.setMenu(null)
  //引入窗口控制菜单
  
}


//app.setPath('temp', 'c:/Crashes')
// app.on('ready', ()=>{
//   if (process.env.NODE_ENV !== 'development') {
//     compyfile.exists( path.resolve(path.join(__dirname,'../../../extraResources')), path.resolve(path.join(__dirname,'../../../../')), compyfile.copy )
//   }
// })

app.whenReady().then(()=>{
  if (process.env.NODE_ENV !== 'development') {
    compyfile.exists( path.resolve(path.join(__dirname,'../../../extraResources')), path.resolve(path.join(__dirname,'../../../../')), compyfile.copy )
  }
  createWindow();
  mainWindow.once('ready-to-show', () => {

    mainWindow.show();
    require('./frameControl.js');

  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow()
  }
})

/**
 * Auto Updater
 *
 * Uncomment the following code below and install `electron-updater` to
 * support auto updating. Code Signing with a valid certificate is required.
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-electron-builder.html#auto-updating
 */

/*
import { autoUpdater } from 'electron-updater'

autoUpdater.on('update-downloaded', () => {
  autoUpdater.quitAndInstall()
})

app.on('ready', () => {
  if (process.env.NODE_ENV === 'production') autoUpdater.checkForUpdates()
})
 */
