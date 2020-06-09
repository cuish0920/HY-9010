{
  "targets": [
    {
      "target_name": "factoryInterface",
      "sources": [ "factoryInterface.cc","objectFactory.cc","CJsonObject.cpp","SerialUtils.cpp",
	  "cJSON.cpp","EncodeUtils.cpp","xmlOper.cpp","OpencvUtils.cpp","IpxCamera.cpp","threadsafe_queue.h","LogUtils.cpp"],
      "include_dirs":[
         "<!(node -e \"require('nan')\")",
		 "C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Tools/MSVC/14.16.27023/atlmfc/include",
		 "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um",
		 "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared",
		 "E:/opencv/opencv_sdk/include",
		 "F:/ELECTRON/HY-9010_V1.0/HY_9010_T/Imperx Camera SDK/inc",
		 "F:/ELECTRON/HY-9010_V1.0/HY_9010_T/log4api/include"
      ],
      "libraries":[],
      "link_settings":{
        "libraries":["-l C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Tools/MSVC/14.16.27023/atlmfc/lib/x64/nafxcw.lib",
		"C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Tools/MSVC/14.16.27023/atlmfc/lib/x64/atls.lib",
		"E:/opencv/opencv_sdk/x64/vc14/lib/opencv_world347.lib",
		"F:/ELECTRON/HY-9010_V1.0/HY_9010_T/Imperx Camera SDK/lib/win64_x64/IpxCameraApi.lib",
		"F:/ELECTRON/HY-9010_V1.0/HY_9010_T/log4api/log4cpp.lib"
		]
      },
      #"cflags!": [ "-fno-exceptions" ],
      #"cflags": [ "-std=c++11" ],
      #"cflags_cc!": [ "-fno-exceptions" ]
    }
  ]
}