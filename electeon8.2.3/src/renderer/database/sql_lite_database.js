//引入sqlite3 
import sql_db from 'sqlite3'


/**
 * File: sqlite.js.
 * Author: W A P.
 * Email: 610585613@qq.com.
 * Datetime: 2018/07/24.
 */
 
var fs = require('fs');
var sqlite3 = sql_db.verbose();
 
var DB = DB || {};
 
DB.SqliteDB = function(file){
    
    DB.db = new sqlite3.Database(file);
    console.log("open db file!");
    DB.exist = fs.existsSync(file);
    if(!DB.exist){
        console.log("Creating db file!");
        fs.openSync(file, 'w');
    };

   
    
    //return DB;
};
 
DB.printErrorInfo = function(err){
    console.log("Error Message:" + err.message + " ErrorNumber:" + errno);
};
 
DB.SqliteDB.prototype.createTable = function(sql){
    DB.db.serialize(function(){
        DB.db.run(sql, function(err){
            if(null != err){
                DB.printErrorInfo(err);
                return;
            }
        });
    });
};
 
/// tilesData format; [[level, column, row, content], [level, column, row, content]]
DB.SqliteDB.prototype.insertData = function(sql, objects){
    DB.db.serialize(function(){
        var stmt = DB.db.prepare(sql);
        for(var i = 0; i < objects.length; ++i){
            stmt.run(objects[i]);
        }
    
        stmt.finalize();
    });
};
 
DB.SqliteDB.prototype.queryData = function(sql, callback){
    DB.db.all(sql, function(err, rows){
        if(null != err){
            DB.printErrorInfo(err);
            return;
        }
 
        /// deal query data.
        if(callback){
            callback(rows);
        }
    });
};
 
DB.SqliteDB.prototype.executeSql = function(sql){
    DB.db.run(sql, function(err){
        if(null != err){
            DB.printErrorInfo(err);
        }
    });
};
 
DB.SqliteDB.prototype.close = function(){
    DB.db.close();
};

DB.SqliteDB.prototype.init = function(){
    
    DB.SqliteDB.prototype.createTable("create table if not exists sys_paramconfig(param_name text, param_value INTEGER,param_type INTEGER);");
    let querySql = "select * from sys_paramconfig where param_name = 'system_ini' and param_value='1' ";
    DB.SqliteDB.prototype.queryData(querySql, function(objects){
        
        if(objects.length==0){
            //说明是第一次创建表，需要写入系统初使化参数
            var paramter_data = [["system_ini",1,1],["ProgFrameTimeAbs", 50,1], ["ExposureTimeRaw", 20,1], ["BinningHorizontal", 4,1], ["BinningVertical", 4,1]
            ,["Width",480,1],["Height",270,1],["ReverseX",0,1],["PixelFormat",17825797,1],["TriggerMode",0,1],["TriggerActivation",0,1]
            ,["TriggerSource",27,1],["ExposureMode",1,1],["ProgFrameTimeEnable",1,1],["TriggerType",1,1],["GainRaw",134,1],["gpsport",1,1],["filePath","",1]];
            var insertTileSql = "insert into sys_paramconfig(param_name, param_value,param_type) values(?, ?, ?)";
            DB.SqliteDB.prototype.insertData(insertTileSql, paramter_data);
         }
       
    });
};

/// export SqliteDB.
// exports.SqliteDB = DB.SqliteDB;


// export default {
//     install:function(Vue,options) {
//         DB.SqliteDB("./cui_sh.db");
//         Vue.prototype.$db=DB;
//     }
    
//   }


// export default {
//     base_db:DB
    
//   }

let dbInstance = new DB.SqliteDB("./cui_test.db");
dbInstance.init();


export default dbInstance;
