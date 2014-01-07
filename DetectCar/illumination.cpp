#include "illumination.h"

int Illumination::singleIllumination( string InImgPosition , string InImageName, string OutImgPosition, string OutImgName ){

	Mat img, gray, float_gray, blur, num, den;

	// Load color image
	string inimage = InImgPosition + InImageName;
	string outimage = OutImgPosition + OutImgName;

	img = cv::imread(inimage, 1);
	if( !img.data ) {
		return -1;
	}

	// convert to grayscale
	cv::cvtColor(img, gray, CV_BGR2GRAY);

	// convert to floating-point image
	gray.convertTo(float_gray, CV_32F, 1.0/255.0);

	// numerator = img - gauss_blur(img)
	cv::GaussianBlur(float_gray, blur, Size(0,0), 2, 2);
	num = float_gray - blur;

	// denominator = sqrt(gauss_blur(img^2))
	cv::GaussianBlur(num.mul(num), blur, Size(0,0), 20, 20);
	cv::pow(blur, 0.5, den);

	// output = numerator / denominator
	gray = num / den;

	// normalize output into [0,1]
	cv::normalize(gray, gray, 0.0, 1.0, NORM_MINMAX, -1);
	gray = gray*255;

	// store
	char saveName[256];
	strcpy(saveName, outimage.c_str());
	imwrite(saveName, gray);//store images.

	return 0;

}

Mat Illumination::MatIllumination( Mat img ){

	Mat gray, float_gray, blur, num, den;

	// convert to grayscale
	cv::cvtColor(img, gray, CV_BGR2GRAY);

	// convert to floating-point image
	gray.convertTo(float_gray, CV_32F, 1.0/255.0);

	// numerator = img - gauss_blur(img)
	cv::GaussianBlur(float_gray, blur, Size(0,0), 2, 2);
	num = float_gray - blur;

	// denominator = sqrt(gauss_blur(img^2))
	cv::GaussianBlur(num.mul(num), blur, Size(0,0), 20, 20);
	cv::pow(blur, 0.5, den);

	// output = numerator / denominator
	gray = num / den;

	// normalize output into [0,1]
	cv::normalize(gray, gray, 0.0, 1.0, NORM_MINMAX, -1);
	gray = gray*255;

	return gray;

}