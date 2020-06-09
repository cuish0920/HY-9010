
<template>

  <div>
      <!-- <el-button plain icon="el-icon-d-arrow-right" @click="open_usart" >打开串口</el-button> -->
      <!-- <el-button plain icon="el-icon-d-arrow-right" @click="usart_add" >对焦加</el-button>
      <el-button plain icon="el-icon-d-arrow-left" @click="usart_div" >对焦减</el-button> -->

       <!-- <el-button plain icon="el-icon-d-arrow-left" @click="usart_adp_add" >调光加</el-button>
      <el-button plain icon="el-icon-d-arrow-left" @click="usart_adp_div" >调光减</el-button>

      <el-button plain icon="el-icon-d-arrow-left" @click="usart_get_dis" >获取位置信息</el-button>-->

      <!-- <el-button plain icon="el-icon-d-arrow-left" @click="auto_focus" >2M以内自动对焦</el-button>  -->
      <el-row>
        <el-col :span="12" class="box_width" >
          <div class="grid-content  bg-purple-dark">
            <div class="border_box">
              <div class="line_box position_absolute">
                 <i class="t_l_line"></i>
                 <i class="l_t_line"></i>
                 <i class="t_r_line"></i>
                 <i class="r_t_line"></i>
                 <i class="l_b_line"></i>
                 <i class="b_l_line"></i>
                 <i class="r_b_line"></i>
                 <i class="b_r_line"></i>
                 <canvas width="640" height="480"  id="canvas">
                  Your browser does not support canvas!  光谱仪图像
                  </canvas>   
              </div>
               
            </div>
            <div class="box_tools">
              <el-form :inline="true" :model="formInline" class="user_form_itm">            
                  <el-form-item label="R">
                    <el-select v-model="formInline.R" filterable placeholder="请选择" @change="r_change" class="select_width">
                      <el-option
                        v-for="item in options"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                      </el-option>
                    </el-select>

                  </el-form-item>

                  <el-form-item label="G">
                    <el-select v-model="formInline.G" filterable placeholder="请选择" @change="g_change" class="select_width">
                      <el-option
                        v-for="item in options"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                      </el-option>
                    </el-select>
                  </el-form-item>

                  <el-form-item label="B">
                    <el-select v-model="formInline.B" filterable placeholder="请选择" @change="b_change" class="select_width">
                      <el-option
                        v-for="item in options"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value">
                      </el-option>
                    </el-select>
                  </el-form-item>


                 
              </el-form>
            </div>
              
          </div>
  
        </el-col>
        <el-col :span="12" class="box_width">

          <div class="grid-content  bg-purple-dark">
              <div class="border_box">
                  <div class="line_box position_absolute">
                    <i class="t_l_line"></i>
                    <i class="l_t_line"></i>
                    <i class="t_r_line"></i>
                    <i class="r_t_line"></i>
                    <i class="l_b_line"></i>
                    <i class="b_l_line"></i>
                    <i class="r_b_line"></i>
                    <i class="b_r_line"></i>
                    <!-- <canvas width="640" height="480" id="michrome">
                        Your browser does not support canvas! 
                    </canvas>  -->
                     <canvas width="720" height="480"  id="spectrumChart">
                          Your browser does not support canvas!
                     </canvas> 

                  </div>
                </div>
                 <div class="box_tools">
                   <el-form :inline="true" :model="formInline" class="user_form_itm">
                      <!-- <el-form :inline="true" :model="spformInline" class="user_form_itm"> -->
                      <el-form-item label="position"  >
                        <el-select v-model="spformInline.spectrum" filterable placeholder="请选择" @change="spectrum_change" class="select_width">
                          <el-option
                            v-for="item in spoptions"
                            :key="item.value"
                            :label="item.label"
                            :value="item.value">
                          </el-option>
                        </el-select>
                      </el-form-item>
                      <!-- </el-form> -->

                      <el-form-item label="state">
                         <el-button   :type="state_type"  class="header_menu" icon="el-icon-data-analysis" ></el-button>
                      </el-form-item>

                      <el-form-item label="GNSS">
                         <el-button type="primary" plain>{{EKF_STATUS}}</el-button>
                      </el-form-item>
                      <el-form-item label="RTK">
                         <el-button type="primary" plain>{{RTK_STATUS}}</el-button>
                      </el-form-item>

                   </el-form>
                 </div>
               
          </div>
        </el-col>
      </el-row>

      
     
    <el-col :span="12" class="box_width" >
      <div class="grid-content  bg-purple-dark">
        <div class="border_box">
          <div class="line_box position_absolute">
            <i class="t_l_line"></i>
            <i class="l_t_line"></i>
            <i class="t_r_line"></i>
            <i class="r_t_line"></i>
            <i class="l_b_line"></i>
            <i class="b_l_line"></i>
            <i class="r_b_line"></i>
            <i class="b_r_line"></i>
            

          </div>
          <canvas width="640" height="375" id="imperxChart">
                Your browser does not support canvas!
          </canvas> 
        </div>

        
      </div>
    </el-col>

    <el-col :span="12" class="box_width">
      <div class="grid-content  bg-purple-dark">
        <div class="border_box">
          <div class="line_box">
            <i class="t_l_line position_absolutei"></i>
            <i class="l_t_line position_absolutei"></i>
            <i class="t_r_line position_absolutei"></i>
            <i class="r_t_line position_absolutei"></i>
            <i class="l_b_line position_absolutei"></i>
            <i class="b_l_line position_absolutei"></i>
            <i class="r_b_line position_absolutei"></i>
            <i class="b_r_line position_absolutei"></i>
              
          </div>
          <canvas width="640" height="375"   id="Temperature">
              Your browser does not support canvas!
          </canvas> 
        </div>

        
      </div>
    </el-col>
    

  <el-drawer title="setting"
    :visible.sync="drawer"
    :direction="direction"
    :before-close="handleClose"
    size="30%"
    :modal="coveringlayer">

    <system-Information></system-information>

  </el-drawer>

  <!-- <el-button type="text" @click="dialogVisible = true">点击打开 Dialog</el-button> -->
  <el-dialog
    title="Dangerous Operation"
    :visible.sync="dialogVisible"
    width="80%"
    :before-close="handleDialogClose">

    <system-SuperSettings></system-SuperSettings>
   
  </el-dialog>

  </div>
</template>


<style>
</style>

<script>
  //const { ipcRenderer } = require('electron')
  import systemInformation from './LandingPage/SystemInformation'
  import systemSuperSettings from './LandingPage/SuperSettings'

  import "./LandingPage.scss"
  import { resolve } from 'url';
  //require('echarts/theme/shine');
  

  var imageDataBuffer = new Uint8ClampedArray(640 * 480 * 4);
  var imageData = new ImageData(imageDataBuffer, 640, 480);

  var imageDataBufferMi = new Uint8ClampedArray(640 * 480 * 4);
  var imageDataMi = new ImageData(imageDataBufferMi, 640, 480);

  /*空间维图像显示 */
  var canvas;
  var context;

 
  /*光谱维图像显示 */
  var canvas_sp;
  var context_sp;

  /*其它参数显示 */
  var imperx_canvas;
  var imperx_context;

  let Temper_canvas;
  let Temper_context;
  var stats = null;
  export default {
      name: 'landing-page',
      components: { systemInformation, systemSuperSettings},
      data(){
          return{
              drawer: false,
              coveringlayer:false,
              direction: 'rtl',
              dialogVisible:false,
              formInline: {
                R: 107,
                G:70,
                B:43
              },
              labelPosition:"left",
              spformInline:{
                 spectrum:100
              },
              options:optionsdata(270),
              spoptions:optionsdata(480),
           
              SuperTriggerKey:13,
              SuperTriggerKey_Count:0,
              window_status:false,
              that_hibernate:null,
              state_type:'danger',
              EKF_STATUS:'unknow',
              RTK_STATUS:'unknow'
          }
      },
      created(){
        this.$bus.$on('hibernate', (that) => {
            this.that_hibernate = that;
        });
        this.$bus.$on('systemSeting', (data) => {
            this.systemSeting(data);
        });
        // ipcRenderer.on('main-process-messages', (event, arg) => {

        //   if(arg=="browser-window-minimize")
        //   {
        //     this.window_status=true;
        //   }
        //   else if(arg=="browser-window-restore")
        //   {

        //     this.window_status=false;
          
        //   }
         
        // });
        //从数据库查询配置实例化参数


        let isTrigger=false;
        var _this = this;
        this.systemIni();
      


        document.onkeydown = function(e) {
          
            if(_this.drawer || _this.dialogVisible){
              return;
            }
            
            if(_this.SuperTriggerKey==e.keyCode){
              _this.SuperTriggerKey_Count++;
              //console.log(_this.SuperTriggerKey_Count);
              if( _this.SuperTriggerKey_Count==15){
                _this.$message({
                  type: 'success',
                  message: 'Developer model'
                });
              }
              if( _this.SuperTriggerKey_Count>=20){
                //弹出高级系统设置权限页面
                  _this.$prompt('password', 'Warning! DANGER', {
                    confirmButtonText: 'OK',
                    cancelButtonText: 'CANCLE'
                  }).then(({ value }) => {
                  
                    if(value=="1307725524"){
                       _this.dialogVisible=true;
                    }
                    else{
                       _this.$message({
                        type: 'error',
                        message: 'Password error! ' + value
                      });
                    }
                   
                  }).catch(() => {
                          
                  });

               
                _this.SuperTriggerKey_Count = 0;
              }
              return;
            }
            else
            {
              _this.SuperTriggerKey_Count = 0;

            }
            
        };
        document.addEventListener("visibilitychange", ()=> {
          if(document.visibilityState=="hidden"){
            this.window_status=true;
          }
          else if(document.visibilityState=="visible"){
            this.window_status=false;
          }
        
          //console.log( document.visibilityState );
        });
      },
      mounted() {
        stats = new Stats();
        stats.setMode(0);
        stats.domElement.setAttribute("style", "-webkit-app-region: no-drag;");
        stats.domElement.style.position = 'absolute';
        stats.domElement.style.right = '150px';
        stats.domElement.style.top = '0px';
        document.body.appendChild( stats.domElement );

        canvas = document.getElementById("canvas");
        context = canvas.getContext('2d');

        

         
        
        
        ///////////////////////////////////////////////////
        

        //双缓冲技术绘制光谱图像（空间维）
        let SpcanvasBuffer = document.createElement("canvas");  
        SpcanvasBuffer.width = canvas.width;  
        SpcanvasBuffer.height = canvas.height; 
        let SpcontextBuffer = SpcanvasBuffer.getContext("2d");
       


        //双缓冲技术绘制光谱图像（光谱维）
        canvas_sp = document.getElementById('spectrumChart');
        context_sp = canvas_sp.getContext('2d');

        let spectrumcanvasBuffer = document.createElement("canvas");  
        spectrumcanvasBuffer.width = canvas_sp.width;  
        spectrumcanvasBuffer.height = canvas_sp.height; 
        let spectrumcontextBuffer = spectrumcanvasBuffer.getContext("2d");
        
        let xdata;
        try{
          
            xdata =  this.$addon.getHdrInfo("wavelength");//
            
           if(xdata){
              xdata = xdata.split(',');
            }

            for(var index=0;index<xdata.length;index++)
            {
              xdata[index] = parseInt(xdata[index]);
            }
        }catch(err){

          xdata=[];
          for(var index=0;index<270;index++)
          {
              xdata.push(index);
              //ydata.push(0);
          }
        }
        this.options = optionsdata_format(xdata);
        let spectrumChart = this.spectrumDrawLine(spectrumcanvasBuffer,xdata);
        try{
            xdata =  this.$addon.getHdrInfo("default bands");//
          
            
          if(xdata){
              xdata = xdata.split(',');
            }
        }catch(err){

          xdata=[];
          xdata.push(105);
          xdata.push(59);
          xdata.push(34);

        }
        this.formInline.R = parseInt(xdata[0]);
        this.formInline.G = parseInt(xdata[1]);
        this.formInline.B = parseInt(xdata[2]);
        this.spectrum_change(this.spformInline.spectrum);
        this.r_change(this.formInline.R);
        this.g_change(this.formInline.G);
        this.b_change(this.formInline.B);
        ///////////////////////////////////////////////////


         //双缓冲技术绘制光谱图像（相机其它信息项）
         imperx_canvas = document.getElementById('imperxChart');
         imperx_context = imperx_canvas.getContext('2d');

         var imperx_canvas_off = document.createElement("canvas");
         imperx_canvas_off.width  = imperx_canvas.width;
         imperx_canvas_off.height = imperx_canvas.height;
         var imperx_context_off = imperx_canvas_off.getContext("2d");


       
        
         let imperxChartsOff = this.imperxInfo(imperx_canvas_off);
        
         //双缓冲技术绘制相机温度
         Temper_canvas = document.getElementById('Temperature');
         Temper_context = Temper_canvas.getContext('2d');

         var Temper_canvas_off = document.createElement("canvas");
         Temper_canvas_off.width  = Temper_canvas.width;
         Temper_canvas_off.height = Temper_canvas.height;
         var Temper_context_off = Temper_canvas_off.getContext("2d");


         let TemperChartsOff = this.imperxTemper(Temper_canvas_off);
        //  let that=this;
        //  setInterval(function (){
           
           
             
            
        //       option_imp.series[0].data[0].value = (Math.random()*100).toFixed(2) - 0;
        //       option_imp.series[1].data[0].value = (Math.random()*7).toFixed(2) - 0;
        //       imperxChartsOff.setOption(option_imp,true);

        //       imperx_context.clearRect(0, 0, imperx_canvas.width, imperx_canvas.height);
        //       imperx_context.drawImage(imperx_canvas_off,0,0,imperx_canvas_off.width,imperx_canvas_off.height);

        //   },2000)

        this.$addon.setLogCallback((message) =>{
          console.log(message);
        });

       var option = spectrumChart.getOption();
          
         
       
        
        //let frameBuffer = this.$addon.createFrameBuffer();
        //let spectrumBuffer=this.$addon.createSpecBuffer();
        //let ydata= [];
        let spec_index=0;
        this.$addon.callBack((frameBuffer,spectrumBuffer) =>{
           try{
                  if(this.window_status)
                  {
                   
                    return;
                  }
                
                  imageDataBuffer.set(frameBuffer);


                
                  SpcontextBuffer.putImageData(imageData, 0, 0);
                  context.clearRect(0, 0, SpcanvasBuffer.width, SpcanvasBuffer.height);
                  context.drawImage(SpcanvasBuffer,0,0,SpcanvasBuffer.width,SpcanvasBuffer.height);

                  let ydata= [];
                  spectrumBuffer.forEach(function(element, index, array){
                    ydata.push(element);
                  });
                  //option.series[0].data = ydata;
                  spectrumChart.setOption({
                      series: [{
                          data: ydata
                      }]
                  });
                  context_sp.clearRect(0, 0, spectrumcanvasBuffer.width, spectrumcanvasBuffer.height);
                  context_sp.drawImage(spectrumcanvasBuffer,0,0,spectrumcanvasBuffer.width,spectrumcanvasBuffer.height);
                  
                  ydata = null;




                  stats.update();
                  frameBuffer =null;
                  spectrumBuffer=null;
                 

           }catch(err)
           {
              this.$electron_log.error(err.message);
           }
            
            
        }); 
          
        let exp_option = imperxChartsOff.getOption();
        let Temperature_option = TemperChartsOff.getOption();
        this.$addon.sysInfoCallBack((data)=>{

          try{
              if(this.window_status)
              {
                return;
              }
              var jsonData = JSON.parse(data);
              //exp_option.series[0].data[0].value =jsonData.ExposureTime;
              //exp_option.series[1].data[0].value =jsonData.FrameRate;
              //imperxChartsOff.clear();
              //TemperChartsOff.clear();
              imperxChartsOff.setOption({
                    series: [
                      {
                        data: [{value: jsonData.ExposureTime, name: "积分时间"}]
                      },
                      {
                        data: [{value: jsonData.FrameRate, name: "帧频 fps"}]
                      }
                    ]
                });
              //imperxChartsOff.setOption(exp_option,true);jsonData.Temperature
              TemperChartsOff.setOption({
                    series: [
                      {
                        data: [{value: jsonData.Temperature, name: '温度 ℃'}]
                      }
                    ]
                });

              imperx_context.clearRect(0, 0, imperx_canvas.width, imperx_canvas.height);
              imperx_context.drawImage(imperx_canvas_off,0,0,imperx_canvas_off.width,imperx_canvas_off.height);

              Temper_context.clearRect(0, 0, Temper_canvas.width, Temper_canvas.height);
              Temper_context.drawImage(Temper_canvas_off,0,0,Temper_canvas_off.width,Temper_canvas_off.height);
              //data=null;
          }catch(err)
          {
            this.$electron_log.error(err.message);
          }
          

        });
        let m_this = this;
        this.$addon.RecCmdCallBack((cmd,value)=>{
          //console.log(cmd,value);
          if(cmd=="AA01"){
            //通讯状态通讯状态
            if(value==0){
              m_this.state_type="danger";
            }
            else{
              m_this.state_type="success";
            }
            // if(m_this.state_type=="danger"){
            //   m_this.state_type="success";
            // }
            // else{
            //   m_this.state_type="danger";
            // }
          }
          else if(cmd=="AA10")
          {
              //仪器控制-00：开 01：关
              //this.that_hibernate.menuOpenCamery();
              
              if(value==0)
              {
                m_this.that_hibernate.menuOpenCamery();
              }
              else if(value==1)
              {
                m_this.that_hibernate.menuCloseCamery();
              }
          }else if(cmd=="AA12"){
            //开始采集-00：图像记录 01：图像预览
              if(value==1)
              {
                m_this.that_hibernate.startview();
              }
              else if(value==0){
                
                // if(m_this.that_hibernate.data_view_save!=1)
                // {
                //    //打开视频流 并且开始保存
                //    m_this.that_hibernate.startrec();
                // }
                // else{
                //   //视频流已经打开 直接开始保存
                //   m_this.that_hibernate.dataControl();
                // }
                m_this.that_hibernate.startRec();
                
              }
              else if(value==2){
                if(m_this.$addon.getCameryRecStatus()){
                   m_this.that_hibernate.stopRec();
                }
                else{
                   m_this.that_hibernate.stopView();
                }
                // if(m_this.that_hibernate.data_view_save==3){
                //    m_this.that_hibernate.dataControlAuto();
                // }
                // //m_this.that_hibernate.stopAcq();
                // else{
                //   m_this.that_hibernate.stopAcq();
                // }
                
              }
          }else if(cmd=="AA13"){
            //切换光谱维 RGB 改变 改成同一个
            this.formInline.R = parseInt(value);
            this.formInline.G = parseInt(value);
            this.formInline.B = parseInt(value);
            this.r_change(value);
            this.g_change(value);
            this.b_change(value);
          }else if(cmd=="AA14"){
            //切换空间维 position
            this.spformInline.spectrum = value;
            this.spectrum_change(value);
          }else if(cmd=="AA11"){
            //积分时间设置（微秒）
            
            value = value*10;
            //this.$message({message: value+"", type: "success"});
             try{
              var msg = this.$addon.setParamImperx("ExposureTimeRaw",value);
              this.updateDbParam(value,"ExposureTimeRaw");
              this.$message({message: msg, type: "success"});
         

           }catch(err){
              this.$message.error(err.message+"[0XIPMERR000D]");
           }
          }else if(cmd=="AA23"){
            //电脑状态-开 关
          }
          
        });
        this.$addon.RecGpsCallBack((ekf,rtk)=>{
          m_this.RTK_STATUS = rtk;
          m_this.EKF_STATUS = ekf;
        });

      },
      methods: {
        r_change(value){
            
            //console.log(value);
             try{
                   var msg = this.$addon.wavelengthSwitch("R",value);
                    this.$message({
                        message: msg,
                        type: "success"
                      });
              }
              catch(err){
                  this.$message.error(err.message+"[0XIPMERR0006]");
              }
        },
        g_change(value){
          try{
                var msg = this.$addon.wavelengthSwitch("G",value);
                 this.$message({
                        message: msg,
                        type: "success"
                      });
            }
            catch(err){
                this.$message.error(err.message+"[0XIPMERR0007]");
            }
        },
        b_change(value){
          try{
                var msg = this.$addon.wavelengthSwitch("B",value);
                 this.$message({
                        message: msg,
                        type: "success"
                      });
            }
            catch(err){
                this.$message.error(err.message+"[0XIPMERR0008]");
            }
        },
        spectrum_change(value){
           try{
                var msg = this.$addon.wavelengthSwitch("spectrum",value);
                 this.$message({
                        message: msg,
                        type: "success"
                      });
            }
            catch(err){
                this.$message.error(err.message+"[0XIPMERR0009]");
            }
        },
        spectrumDrawLine(elemet,xdata){
            var spectrumChart = this.$echarts.init(elemet)
              spectrumChart.setOption({
              title: { 
                text: 'position',
                textStyle:{
                  color:"white"
                }
              },
              tooltip: {},
              //工具框，可以选择
              
              xAxis: {
                 type: 'category',
                 data:xdata,
                 //坐标轴颜色
                 axisLine:{
                    lineStyle:{
                        color:'white'
                    }
                }
              },
              // dataZoom:[{
              //   type:"inside"         //详细配置可见echarts官网
              // }],
              yAxis: { 
                type: 'value',
                axisLine:{
                    lineStyle:{
                        color:'white'
                    }
                }
              },
              series: [{
                  type: 'line',
                  data: []
              }]
          });
          return spectrumChart;
        },
        imperxInfo(elemet){
          let option = {
             tooltip : {
                  formatter: "{a} <br/>{b} : {c}%"
              },
              // toolbox: {
              //     feature: {
              //         restore: {},
              //         saveAsImage: {}
              //     }
              // },
              series: [
                {
                      title : {
                          // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                          color: '#fff'
                    },
                    name: "积分时间",
                    type: 'gauge',
                    min: 0,
                    max: 1,
                    center: ['75%', '55%'],    // 默认全局居中
                    detail: {formatter:'{value}S'},
                    data: [{value: 50, name: "积分时间"}]
                    
                },
                {
                  title : {
                          // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                          color: '#fff'
                    },
                    name: "帧频",
                    type: 'gauge',
                    min: 0,
                    max: 100,
                    center: ['25%', '55%'],    // 默认全局居中
                    detail: {formatter:'{value}fps'},
                    data: [{value: 50, name: "帧频 fps"}]
                }
              ]
          };
          let imperxChart = this.$echarts.init(elemet)
          imperxChart.setOption(option);
          return imperxChart;
        },
        imperxTemper(elemet){
          let option = {
              tooltip : {
                  formatter: "{a} <br/>{b} : {c}%"
              },
              // toolbox: {
              //     feature: {
              //         restore: {},
              //         saveAsImage: {}
              //     }
              // },
              series: [
                  {
                     title : {
                          // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                          color: '#fff'
                      },
                      name: '温度',
                      type: 'gauge',
                      detail: {formatter:'{value}℃'},
                      data: [{value: 50, name: '温度 ℃'}]
                  }
              ]
          };
          let imperxChart = this.$echarts.init(elemet)
          imperxChart.setOption(option);
          return imperxChart;

        },
        systemSeting(data){
          //系统参数设置
          //弹出参数设置界面
          this.drawer=true;

          setTimeout(() => {
            this.$bus.$emit('openSystemPage');
          }, 200);
        },
        handleClose(done){
         this.systemIni();
         done();
        
        },
        handleDialogClose(done){
          done();
        },
        systemIni(){
            
            
        },
        updateDbParam(p_value,p_name){
          try{
                var sql = "UPDATE sys_paramconfig set param_value = '"+ p_value+"' where param_name = '"+p_name+"' ";
                this.$dboper.executeSql(sql);
            }catch(err){
              this.$message.error(err.message+"[0XIPMERR0011]");
            }
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


function optionsdata(range){


      var array_item=new Array();
        for(var index=0;index<range;index++)
        {
          var map={};
          map["value"]=index;
          map["label"]=index;
          array_item.push(map);
        }
        
        return array_item;
}

function optionsdata_format(format_array){


      var array_item=new Array();
        for(var index=0;index<format_array.length;index++)
        {
          var map={};
          map["value"]=index;
          map["label"]=index+"--"+format_array[index];
          array_item.push(map);
        }
        
        return array_item;
}

</script>

