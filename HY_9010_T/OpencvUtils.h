#pragma once
#ifndef __OPENCV_UTILS_H__
#define __OPENCV_UTILS_H__
#include "opencv2\opencv.hpp"
#include<string>

class OpencvUtils {
	 public:
		 static std::string Mat2Base64(const cv::Mat &img, std::string imgType);
		 static cv::Mat Base2Mat(std::string &base64_data);
};




#endif 