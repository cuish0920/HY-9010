<template>
  <div class="system_box">
    <el-collapse v-model="activeNames" @change="handleChange">
      <el-collapse-item title="Camera Parameter Settings" name="1">
        <div class="system_padding">
          <el-divider><i class="el-icon-more"></i></el-divider>
          <div>
            <span>FrameRate</span>
            <el-slider v-model="frameFps" show-input
            :min="fpsMin" 
            :max="fpsMax"
            @change="frameRateUpdate">
            </el-slider>
          </div>

          <div>
            <span>ExposureTimeRaw</span>
            <el-slider v-model="exposure" show-input
            :min="exposureMin" 
            :max="exposureMax"
            @change="exposureTimeUpdate">
            </el-slider>
          </div>
        
          <div>
            <span>GainRaw</span>
            <el-slider v-model="gain" show-input
            :min="gainMin" 
            :max="gainMax"
            @change="gainRawUpdate">
            </el-slider>
          </div>
          <el-divider><i class="el-icon-camera"></i></el-divider>
          <el-form ref="form" label-width="135px">
          
            <el-form-item label="TriggerMode">
              <el-select v-model="TriggerModevalue" placeholder="TriggerMode" @change="TriggerMode_change">
                  <el-option
                    v-for="item in options_TriggerMode"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value">
                  </el-option>
              </el-select>
            </el-form-item>

            <el-form-item label="TriggerActivation">
              <el-select v-model="TriggerActivationvalue" placeholder="TriggerActivation" @change="TriggerActivation_change">
                  <el-option
                    v-for="item in options_TriggerActivation"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value">
                  </el-option>
              </el-select>
            </el-form-item>

            <!-- 相元合并 -->
            <el-form-item label="BinningHorizontal">
              <el-select v-model="BinningHorizontal" placeholder="BinningHorizontal" @change="BinningHorizontal_change">
                  <el-option
                    v-for="item in options_Binning"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value">
                  </el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="BinningVertical">
              <el-select v-model="BinningVertical" placeholder="BinningVertical" @change="BinningVertical_change">
                  <el-option
                    v-for="item in options_Binning"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value">
                  </el-option>
              </el-select>
            </el-form-item>

            <el-form-item label="GpsPort">
              <el-input-number v-model="form.gpsport" @change="GpsportChange" :min="1" :max="255"></el-input-number>
            </el-form-item>

            <el-form-item label="FileStoragePath">
             <el-input placeholder="default" :disabled="true" v-model="input_filePath">
                <el-button slot="append" icon="el-icon-files" @click="select_filePath"></el-button>
              </el-input>
            </el-form-item>



          </el-form>
        </div>
      </el-collapse-item>
      

       

    </el-collapse>
  </div>
</template>

<script>
  var {remote}=require('electron');
  export default {
    name: 'system-SuperSetting',
    data () {
      return {
        fpsMin:1,
        fpsMax:80,
        frameFps:10,
        exposureMin:1,
        exposureMax:12500,
        exposure:4000,
        gainMin:1,
        gainMax:1000,
        gain:140,
        TriggerModevalue:1,
        options_TriggerMode:[{"value":0,"label":"OFF"},{"value":1,"label":"ON"}],
        TriggerActivationvalue:0,
        options_TriggerActivation:[{"value":0,"label":"RisingEdge"},{"value":1,"label":"FallingEdge"}],
        activeNames:['1'],
        options:[{"label":"X1","value":1},{"label":"X2","value":2},{"label":"X3","value":3}],
        options_Binning:[{"label":"X1","value":1},{"label":"X2","value":2},{"label":"X3","value":3},{"label":"X4","value":4}],
        BinningHorizontal:4,
        BinningVertical:4,
        form: {
              gpsport:1
        },
        input_filePath:''
      }
    },
    beforeCreate(){
      
        // this.$bus.$on('openSystemPage', (data) => {
        //     //this.$message("data");
        //     //查询相机参数并显示到界面
        //     //1获取相机当前帧频
        //     try{
        //         var msg = this.$addon.getParamImperx("CurrentFrameRate");//当前帧频
        //         this.frameFps = msg;
        //         msg = this.$addon.getParamImperx("ExposureTimeRaw");
        //         this.exposure = msg;
        //         msg = this.$addon.getParamImperx("GainRaw");//增益
        //         this.gain=msg;
        //         //msg = this.$addon.getParamImperx("ProgFrameTimeAbs");//设置帧频
        //         msg = this.$addon.getParamImperx("TriggerMode");//设置帧频
        //         this.TriggerModevalue= (msg==0)?"OFF":"ON";
        //         msg = this.$addon.getParamImperx("TriggerActivation");//设置帧频
        //         this.TriggerActivationvalue= (msg==0)?"RisingEdge":"FallingEdge";


        //         //this.$message(""+msg);
        //     }catch(err)
        //     {
        //         this.$message.error(err.message+"[0XIPMERR000B]");
        //     }
        // })
        
    },
    mounted(){
      this.systemIni();
    },
    methods:{
      frameRateUpdate(value){
          
          this.exposureMax = 1.0/value *1000000;
          this.exposure =  this.exposureMax;
          //设置帧频，设置积分时间
           try{
              
              this.updateDbParam(value,"ProgFrameTimeAbs");
             
              
              this.updateDbParam(this.exposure,"ExposureTimeRaw");
              this.$message({message: "update success", type: "success"});

           }catch(err){
              this.$message.error(err.message);
           }

        },
        exposureTimeUpdate(value){
          try{
             
              this.updateDbParam(value,"ExposureTimeRaw");
              this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }
        },
        gainRawUpdate(value){
           try{
              
              this.updateDbParam(value,"GainRaw");
               this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }

        },
        TriggerMode_change(value){
           try{
            
              this.updateDbParam(value,"TriggerMode");
              this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }
        },
        TriggerActivation_change(value){
          try{
             
              this.updateDbParam(value,"TriggerActivation");
              this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }
        },
        BinningHorizontal_change(value){
          try{
             
              this.updateDbParam(value,"BinningHorizontal");
              this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }
        },
        BinningVertical_change(value){
          try{
             
              this.updateDbParam(value,"BinningVertical");
              this.$message({message: "update success", type: "success"});
         

           }catch(err){
              this.$message.error(err.message);
           }
        },
        updateDbParam(p_value,p_name){
          try{
                var sql = "UPDATE sys_paramconfig set param_value = '"+ p_value+"' where param_name = '"+p_name+"' ";
                this.$dboper.executeSql(sql);
            }catch(err){
              this.$message.error(err.message+"[0XIPMERR0011]");
            }
        },
        keyCodeChange(param_name,e){
          //debugger
          e.preventDefault();
          //console.log("设置"+e.keyCode+" "+e.code);
          try
          {
             this.updateDbParam(e.code,param_name);
             this.$message({message: "update success", type: "success"});
          }
          catch(err){
            this.$message.error(err.message);
          }
         
          this.form[param_name]=e.code;

        },
        GpsportChange(value){
         try
          {
             this.updateDbParam(value,"gpsport");
             this.$message({message: "update success", type: "success"});
          }
          catch(err){
             this.$message.error(err.message);
          }
         
          //this.form.zaddr=value;
        },
        select_filePath(){
          var dir= remote.dialog.showOpenDialog({

                properties:['openDirectory']

            }).then(result => {
              if(!result.canceled)
              {
                this.input_filePath = result.filePaths[0];
                this.updateDbParam(this.input_filePath,"filePath");
              }

            //console.log(result.canceled)
            //console.log(result.filePaths)
          }).catch(err => {
            //console.log(err)
          })
        },
        handleChange(val){
          
        },
        
        systemIni(){
            var _this = this;
            var querySql = "select * from sys_paramconfig";
            this.$dboper.queryData(querySql, function(objects){
              let map = {};
              for(var i = 0; i < objects.length; ++i){
                  
                  map[objects[i].param_name]= objects[i].param_value;
                  
              }




              _this.TriggerModevalue=map["TriggerMode"];
              _this.BinningHorizontal = map["BinningHorizontal"],
              _this.BinningVertical = map["BinningVertical"],
              _this.TriggerActivationvalue=map["TriggerActivation"];         
              _this.frameFps=map["ProgFrameTimeAbs"];
              _this.exposureMax = 1.0/ _this.frameFps *1000000;             
              _this.exposure=map["ExposureTimeRaw"];
              _this.gain=map["GainRaw"];
              _this.input_filePath = map["filePath"];
              _this.form.gpsport =  map["gpsport"];

             

            });

        }
    }
  }
</script>

<style scoped  lang="scss">
  .system_box{
    padding: 13px;
    height:95%;
    max-height:900px;
    overflow:auto;
    //border: 1px solid red;
  }
  .system_box .system_padding{
    padding: 13px;
  }
</style>
