<template>
  <div id="app">

  
  <el-container :style="{'height':winHeight+'px'}">
      
        <el-container>
            <el-aside width="auto" class="main-meu" >

              

               <!-- <div style="width:100%;padding:10px">
                 <el-button plain icon="el-icon-d-arrow-right" id="menu_control" @click="open_close_menu" ></el-button>
               </div> -->

                <div class="isClossTab" @click="open_close_menu" style="padding-top: 8px;" >
                      <i :class="isCollapse?'el-icon-d-arrow-right':'el-icon-d-arrow-left'" ></i>
                </div>


               <el-menu style="border-right:none;" default-active="1-2" 
               :background-color="userbg" 
               text-color="#fff"
               active-text-color="#ffd04b"
               @open="handleOpen" 
               @close="handleClose" :collapse="isCollapse">
                
                <el-submenu index="1">
                  <template slot="title">
                    <i class="el-icon-date"></i>
                    <span slot="title">设备管理</span>
                  </template>

                  <el-menu-item-group>
                    <span slot="title">打开/关闭设备</span>
                    <el-menu-item index="1-1" @click="menuOpenCamery">打开设备</el-menu-item>
                    <el-menu-item index="1-2" @click="menuCloseCamery">关闭设备</el-menu-item>
                  </el-menu-item-group>

              
                </el-submenu>


                <el-submenu index="2">
                   <template slot="title">
                    <i class="el-icon-menu"></i>
                    <span slot="title">视频流控制</span>
                  </template>
                  
                  <el-menu-item-group>
                    <!-- <el-menu-item index="2-1"  @click="startAcq">打开视频流</el-menu-item>
                    <el-menu-item index="2-2"  @click="stopAcq">关闭视频流</el-menu-item> -->
                    <!-- <el-menu-item index="2-1"  @click="startview">打开视频流</el-menu-item>
                    <el-menu-item index="2-2"  @click="stopView">关闭视频流</el-menu-item> -->

                    <el-menu-item index="2-1"  @click="startview">开始预览</el-menu-item>
                    <el-menu-item index="2-2"  @click="stopView">停止预览</el-menu-item>

                  </el-menu-item-group>
                </el-submenu>

                <el-submenu index="3">
                   <template slot="title">
                    <i class="el-icon-menu"></i>
                    <span slot="title">数据管理</span>
                  </template>
                  
                  <el-menu-item-group>
                    <!-- <el-menu-item index="3-1"  @click="dataControl">{{control_title}}</el-menu-item> -->

                    <el-menu-item index="3-1"  @click="startRec">开始采集</el-menu-item>
                    <el-menu-item index="3-2"  @click="stopRec">停止采集</el-menu-item>

                  </el-menu-item-group>
                </el-submenu>

                <el-submenu index="4">
                   <template slot="title">
                    <i class="el-icon-menu"></i>
                    <span slot="title">积分时间推荐</span>
                  </template>
                  
                  <el-menu-item-group>
                    <el-menu-item index="4-1" @click="calucateExp">积分时间推荐</el-menu-item>
                  </el-menu-item-group>
                </el-submenu>

                <el-submenu index="5" :disabled=Isdisable>
                   <template slot="title">
                    <i class="el-icon-menu"></i>
                    <span slot="title">参数设置</span>
                  </template>
                  
                  <el-menu-item-group>
                    <el-menu-item index="5-1" @click="systemSeting">系统设置</el-menu-item>
                   
                  </el-menu-item-group>
                </el-submenu>
                
              

                <!-- <el-menu-item index="4" disabled>
                  <i class="el-icon-setting"></i>
                  <span slot="title">导航三</span>
                </el-menu-item>
                <el-menu-item index="4">
                  <i class="el-icon-setting"></i>
                  <span slot="title">导航四</span>
                </el-menu-item> -->
              </el-menu>

            </el-aside>

            <el-container>
              <el-header class="main-header">
                <el-row style="padding-top: 8px;" >
                  <el-button @click="window_min" type="primary" class="header_menu" icon="el-icon-minus" size="mini" circle></el-button>
                  <el-button  @click="window_max" type="success" class="header_menu" icon="el-icon-copy-document" size="mini" circle></el-button>
                  <el-button  @click="window_close" type="danger"  class="header_menu" icon="el-icon-close" size="mini" circle></el-button>
                </el-row>
                  <!-- <el-radio-group v-model="isCollapse" style="margin-bottom: 20px;">
                    <el-radio-button :label="false">展开</el-radio-button>
                    <el-radio-button :label="true">收起</el-radio-button>
                  </el-radio-group> -->
              </el-header>
              <el-main style="padding:2px;">
                <router-view></router-view>
              </el-main>
              <el-footer class="main-footer" >
                    
              </el-footer>

            </el-container>
            
        </el-container>
    </el-container>


    <!-- <router-link to="/">首页</router-link>
    <router-link to="/report">舆情报表</router-link>
    <router-link to="/keywords">舆情关键词设置</router-link>
    <router-link to="/alarm">舆情报警设置</router-link> -->

   

    <el-dialog title="积分时间推荐" :visible.sync="dialogFormVisible">
      <el-form :model="formInline" :label-width="formLabelWidth">
        <el-form-item label="照度值">
          <el-input-number v-model="formInline.light_digital" :min="0" :max="0x7fffffff" label="照度"></el-input-number>
        </el-form-item>
        <el-form-item label="反射率">
          <el-input-number v-model="formInline.reflectivity" :min="0" :max="0x7fffffff" label="反射率"></el-input-number>
        </el-form-item>

        <el-form-item label="计算结果">
          <el-input
            placeholder="计算结果"
            v-model="formInline.result"
            :disabled="true">
          </el-input>
        </el-form-item>

       

     
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button @click="dialogFormVisible = false">关闭</el-button>
        <el-button type="primary" @click="calclateResult">计算</el-button>
      </div>
    </el-dialog>
   
  </div>
</template>

<script>
  //const { remote  } = require('electron')
  
  export default {
    name: 'electron',
    data(){
        return{
          winHeight:document.documentElement.clientHeight,
          isCollapse: false,
          control_title:"开始采集",
          control_flag:true,
          Isdisable:false,
          userbg:"#051f3a",
          dialogFormVisible: false,
          formLabelWidth: '120px',
          formInline:{
            light_digital:1,
            reflectivity:1,
            result:0
          }
        }
    },
    mounted() {
      window.onresize=()=>{

        this.winHeight=document.documentElement.clientHeight;
      };

      this.$bus.$emit('hibernate',this);
      this.autoOpenCmdPort();
      process.on('uncaughtException', function (err) {
           this.$electron_log.error(err.message);
          
      });

      window.onerror = (message, source, lineno, colno, err) =>{
             this.$electron_log.error(err.stack || err);
      };
      setTimeout(() => {
         this.window_max();
      }, 1000);
     
      // GPU进程崩溃
      // remote.app.on('gpu-process-crashed', (event)=>{
      //   debugger
      //  this.$electron_log.error(event);
      // });
      //  remote.app.on('renderer-process-crashed', (event,webContents,killed)=>{
      //   debugger
      //  this.$electron_log.error(event);
      // });

    
    },
    
     methods: {
       	
       open_close_menu(){
         
          this.isCollapse=!this.isCollapse;
       },
       handleOpen(key, keyPath) {
         console.log(key, keyPath);
       },
       handleClose(key, keyPath) {
         console.log(key, keyPath);
       },
       menuOpenCamery()
       {
          const loading = this.loading("Initialization");
          var info = this.$addon.getCameryList();
          if(info)
          {
            info = JSON.parse(info);
            if(info.imperx.length>0)
            {
                this.$message({
                  message: 'Device:'+info.imperx[0].name,
                  type: 'success'
                });
                var m_this = this;
                try
                {
                  setTimeout(() => {
                    
                    var querySql = "select * from sys_paramconfig";
                    
                    m_this.$dboper.queryData(querySql, function(objects){
                    var map = {};
                      for(var i = 0; i < objects.length; ++i){
                          
                          map[objects[i].param_name]= objects[i].param_value;
                          
                      }
                      var param = JSON.stringify(map);
                      try{
                         
                          var mymsg = m_this.$addon.openSerialPort(map["gpsport"]);
                          m_this.$message({
                            message: mymsg,
                            type: "success"
                          });

                          mymsg = m_this.$addon.openCamery(0,param);
                          m_this.$message({
                            message: mymsg,
                            type: "success"
                          });

                          //打开串口连接
                          m_this.Isdisable=false;
                      }
                      catch(err)
                      {
                        m_this.$message.error(err.message+"[0XIPMERR0002]");
                      }
                       
                    });

                  }, 100);
                    //获得系统参数，并用于初使化配置
                    
                    
                }
                catch(err)
                {
                   this.$message.error(err.message+"[0XIPMERR0002]");
                }
                
            }
            else{
              this.$message.error('error!，Device not found![0XIPMERR0001]');
              
            }

          }
          
         
          loading.close();
    
       },
       menuCloseCamery()
       {
          try
          {
            

              var msg =  this.$addon.closeCamery();
              this.$message({
                        message: msg,
                        type: msg
                      });

              msg =  this.$addon.closeSerialPort();
              this.$message({
                        message: msg,
                        type: msg
                      });
            
           
              this.Isdisable=true;
          }
          catch(err){
              this.$message.error(err.message+"[0XIPMERR0003]");
          }
           
       },
       startview(){
         const loading = this.loading("please wait");
          try{
            
             

              let m_that=this; 
              this.$addon.start(()=>{
                // setTimeout(() => {
                //    m_that.$addon.UART_SEND("AA36000000003655",4,0,()=>{
                //    m_that.$message({
                //         message: "success",
                //         type: "success"
                //       });
                //    //loading.close();
                //   });
                // }, 100);
                  m_that.$addon.UART_SEND("AA36000000003655",800,0,()=>{
                   m_that.$message({
                        message: "success",
                        type: "success"
                      });
                   loading.close();
                  });
               
               
              });
             
              // loading.close();
          }
          catch(err)
          {
               loading.close();
               this.$message.error(err.message+"[0XIPMERR0004]");
          }
       },
       stopView(){
          const loading = this.loading("please wait");
         try{
            let m_that = this;
             m_that.$addon.UART_SEND("AA37000000003755",0,800,()=>{
                try{
                  m_that.$addon.stop(1,()=>{
                     m_that.$message({
                        message: "success",
                        type: "success"
                      });
                  });
                  loading.close();
                }catch(err){
                  loading.close();
                  this.$message.error(err.message);
                }
              });
            // setTimeout(() => {
            //   m_that.$addon.UART_SEND("AA37000000003755",0,4,()=>{
            //     try{
            //       m_that.$addon.stop(1,()=>{
            //          m_that.$message({
            //             message: "success",
            //             type: "success"
            //           });
            //       });
            //       loading.close();
            //     }catch(err){
            //       loading.close();
            //       this.$message.error(err.message);
            //     }
            //   });
            // }, 100);
            // m_that.$addon.UART_SEND("AA37000000003755",0,4,()=>{
            //   // setTimeout(() => {
            //   //   try{
            //   //     m_that.$addon.stop(1,()=>{
            //   //        m_that.$message({
            //   //           message: "success",
            //   //           type: "success"
            //   //         });
            //   //     });
            //   //     loading.close();
            //   //   }catch(err){
            //   //     loading.close();
            //   //     this.$message.error(err.message);
            //   //   }
                  

            //   // }, 1500);

            //    try{
            //       m_that.$addon.stop(1,()=>{
            //          m_that.$message({
            //             message: "success",
            //             type: "success"
            //           });
            //       });
            //       loading.close();
            //     }catch(err){
            //       loading.close();
            //       this.$message.error(err.message);
            //     }
              
            // });
         }catch(err){
            loading.close();
            this.$message.error(err.message);
         }
        
       },
       startRec(){
         const loading = this.loading("please wait");
         try{
           

              let m_that=this; 
              if(m_that.$addon.getCameryAQStatus())
              {
                 m_that.$message.error("Illegal operation[Preview]");
                 loading.close();
                return;
              }
              if(m_that.$addon.getCameryRecStatus())
              {
                m_that.$message.error("Illegal operation[storage]");
                loading.close();
                return;
              }
              

              var querySql = "select * from sys_paramconfig where  param_name = 'filePath'";
              m_that.$dboper.queryData(querySql, function(objects){
                    var map = {};
                    for(var i = 0; i < objects.length; ++i){
                        
                        map[objects[i].param_name]= objects[i].param_value;
                        
                    }

                    try{
                       m_that.$addon.start_rec(map["filePath"],()=>{
                        //先开始数据记录
                        //先开始数据记录 在打开触发信号
                        m_that.$addon.start(()=>{

                          // setTimeout(() => {
                            
                          //   m_that.$addon.UART_SEND("AA36000000003655",4,0,()=>{
                          //       m_that.$message({
                          //             message: "success",
                          //             type: "success"
                          //           });
                          //       loading.close();
                          //   });
                          // }, 100);

                            m_that.$addon.UART_SEND("AA36000000003655",3000,0,()=>{
                                m_that.$message({
                                      message: "success",
                                      type: "success"
                                    });
                                loading.close();
                            });
                        });
                        //end
                      
                    });
                    }catch(err){
                      loading.close();
                      m_that.$message.error(err.message);
                    }
                   

              });

              
              
          }
          catch(err)
          {
            loading.close();
            this.$message.error(err.message+"[0XIPMERR0004]");
          }
       },
       stopRec(){
         const loading = this.loading("please wait");
         try{
            let m_that=this;
            if(!m_that.$addon.getCameryRecStatus()){
              m_that.$message.error("Illegal operation");
              loading.close();
              return;
            }
            //先停止触发信号 停止视频流 在停止数据保存\
             m_that.$addon.UART_SEND("AA37000000003755",0,3000,()=>{
                 try{
                    m_that.$addon.stop(0,()=>{
                      m_that.$addon.stop_rec();
                      m_that.$message({
                        message: "success",
                        type: "success"
                      });
                      loading.close();
                    });
                  }catch(err) {
                    loading.close();
                    m_that.$message.error(err.message);
                }
              });
            // setTimeout(() => {
            //   m_that.$addon.UART_SEND("AA37000000003755",0,4,()=>{
            //      try{
            //         m_that.$addon.stop(0,()=>{
            //           m_that.$addon.stop_rec();
            //           m_that.$message({
            //             message: "success",
            //             type: "success"
            //           });
            //           loading.close();
            //         });
            //       }catch(err) {
            //         loading.close();
            //         m_that.$message.error(err.message);
            //     }
            //   });
            // }, 100);
          //   this.$addon.UART_SEND("AA37000000003755",0,4,()=>{
              
          //     // setTimeout(() => {
          //     //   try{
          //     //       m_that.$addon.stop(0,()=>{
          //     //         m_that.$addon.stop_rec();
          //     //         m_that.$message({
          //     //           message: "success",
          //     //           type: "success"
          //     //         });
          //     //         loading.close();
          //     //       });
          //     //     }catch(err) {
          //     //       loading.close();
          //     //       m_that.$message.error(err.message);
          //     //   }
          //     // }, 4200);
          //    try{
          //           m_that.$addon.stop(0,()=>{
          //             m_that.$addon.stop_rec();
          //             m_that.$message({
          //               message: "success",
          //               type: "success"
          //             });
          //             loading.close();
          //           });
          //         }catch(err) {
          //           loading.close();
          //           m_that.$message.error(err.message);
          //       }
            
          // });
         }catch(err) {
            loading.close();
            this.$message.error(err.message);
         }
          
       },
       autoOpenCmdPort(){
          //查询数据库获得gps端口端口
         
          try{
             var mymsg = this.$addon.openCmdSerialPort(1);
             this.$message({
              message: mymsg,
              type: "success"
             });
          }catch(err){

             this.$confirm('致命错误,连接STM控制板失败。系统将不能工作在触发模式下且不能与地面电台通讯。Some functions will not be available due to abnormal equipment', 'ERROR', {
                confirmButtonText: '重试',
                cancelButtonText: '确定',
                type: 'warning',
                closeOnClickModal:false
              }).then(() => {
                //const loading = this.loading("Initialization");
                this.autoOpenCmdPort();
                //loading.close();
                 
              }).catch(() => {
                 this.$message.error(err.message);
                    
              });
            
          }

          
         
       },
       calclateResult(){
         if(this.formInline.light_digital==0)
         {
            this.$message.error("无效的照度值");
           return;
         }
         if(this.formInline.reflectivity==0)
         {
            this.$message.error("无效的反射率");
           return;
         }
         this.formInline.result = (22500/this.formInline.light_digital)/this.formInline.reflectivity;
       },
       calucateExp(){
         //积分时间推荐
         this.dialogFormVisible = true;
       },
       window_min(){
         this.$electron.ipcRenderer.send('window-min');     
       },
       window_max(){
         this.$electron.ipcRenderer.send('window-max');
       },
       window_close(){
         try{
              if(this.$addon.getCameryRecStatus()){
                this.$message.error("Illegal operation[Data writing]");
                return;
              }
            
            this.menuCloseCamery();
            this.$addon.closeCmdSerialPort();
            this.$electron.ipcRenderer.send('window-close');
         }catch(err){
           this.$electron_log.warn(err.message);
         }
        
       },
       systemSeting(){
         
          this.$bus.$emit('systemSeting');
          
       },
       loading(textdata){
             const loading = this.$loading({
              lock: true,
              text: textdata,
              spinner: 'el-icon-loading',
              background: 'rgba(0, 0, 0, 0.7)'
            });
            return loading;
        }
    }
  }




</script>

<style lang="scss">
 $background-color: #051f3a;
 $color: #FFF;
*{

  margin:0px;
  padding:0px;
}
.main-meu{
   background-color: $background-color;
   //opacity: .5;
  color: $color;
}
.main-header{
  text-align: right;;
  background-color: $background-color;
  color: $color;

  //opacity: .5;
  max-height: 40px;
  -webkit-app-region: drag;/*窗口可拖拽 */
}
.main-header .header_menu{
  -webkit-app-region: no-drag;
}

.main-footer{
  text-align: center;
  background-color: $background-color;
  color: $color;
  //opacity: .5;
  max-height: 40px;
}

.isClossTab{
  width: 100%;
  cursor: pointer;
  font-size: 25px;
  text-align: center;

  font-weight: bold;
  border-right: 1px solid #807c7c;
  box-sizing: border-box;
}

body {
    width: 100%;
    height: 100%;
    //background-color:#0A438E;
    //opacity: .1;
    background: url("../.../../../static/img/bg.jpg") no-repeat;
    //background: url("../.../../../static/img/bg_body.jpg") no-repeat;
    background-size: 100% 100%;
}

</style>
