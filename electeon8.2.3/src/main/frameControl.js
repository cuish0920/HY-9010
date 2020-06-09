//接收渲染进程广播的数据执行最小化 最大化  关闭的操作


var {ipcMain,BrowserWindow} =require('electron');


//获取当前的窗口对象   BrowserWindow.getFocusedWindow();


var mainWindow= BrowserWindow.getFocusedWindow();



ipcMain.on('window-min',(event)=>{

    //console.log('window-min')

    mainWindow.minimize()
    event.preventDefault()
})

ipcMain.on('window-max',(event)=>{
    
    if(mainWindow.isMaximized()){
        mainWindow.restore();
     }else{
        mainWindow.maximize()
    }
    event.preventDefault()
})

ipcMain.on('window-close',(event)=>{
    mainWindow.close()
    event.preventDefault()
})