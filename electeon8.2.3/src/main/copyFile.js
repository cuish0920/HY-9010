var fs = require( 'fs' );
var   stat = fs.stat;
var path = require('path');

class copyFile{
    static getInstance(){
       
        if(!copyFile.instance){
            copyFile.instance = new copyFile();
        };
        // 简化性能提升
        //console.log("getInstance");
        //test();
        return copyFile.instance;
    }
    constructor(){

    }
    
    exists( src, dst, callback ){

        return new Promise((resolve,reject)=>{
            fs.exists( dst, function( exists ){
                // 已存在
                if( exists ){
                    callback( src, dst );
                }
                // 不存在
                else{
                   
                    fs.mkdir( dst, function(){
                        callback( src, dst );
                    });
                }
            });
        });

        
       
    };

   copy( src, dst ){
        return new Promise((resolve,reject)=>{
                // 读取目录中的所有文件/目录
            fs.readdir( src, function( err, paths ){
                if( err ){
                    throw err;
                }
        
            
        
                paths.forEach(function( path ){
                    var _src = src + '/' + path,
                        _dst = dst + '/' + path,
                        readable, writable;      
                    
                    stat( _src, function( err, st ){
                        if( err ){
                            throw err;
                        }
        
                        if( st.isFile() ){
                        fs.stat(_dst,function(err){
                            if( err ){
        
                            // 创建读取流
                            readable = fs.createReadStream( _src );
                            // 创建写入流
                            writable = fs.createWriteStream( _dst ); 
                            // 通过管道来传输流
                            readable.pipe( writable );
                            //console.log("createReadStream"+_src);
                            //throw err;
                            }
        
                        
        
                        });
        
                        }
                        else if( st.isDirectory() ){
                            copyFile.instance.exists( _src, _dst, copyFile.instance.copy );
                        } 
                    });
                });
            });
        });
        
    };
    copy_file(){
        return new Promise((resolve,reject)=>{
            //console.log(path.resolve(path.join(__dirname,'../../extraResources')));
            //console.log(path.resolve(path.join(__dirname,'../../')));
            

             copyFile.instance.exists( path.resolve(path.join(__dirname,'../../../extraResources')), path.resolve(path.join(__dirname,'../../../../')),  copyFile.instance.copy );
        });
    };
    test(){
        return new Promise((resolve,reject)=>{
            console.log("test");
        });
       
    };
};
export default copyFile.getInstance();;
// 导出模块
//module.exports = copyFile.getInstance();
  