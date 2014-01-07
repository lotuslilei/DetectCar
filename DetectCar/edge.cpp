#include "edge.h"

int EdgeHandle::singleEdgeHandle( string InImgPosition , string InImageName, string OutImgPosition, string OutImgName ){

	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;

	/// Load an image
	src = imread(InImgPosition+InImageName);
	if( !src.data )
	{ return -1; }

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

	/// Convert it to gray
	cvtColor( src, src_gray, CV_RGB2GRAY );

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//sobel
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	// binary image.
	//threshold(grad,grad,0,255,THRESH_BINARY);

	// Apply the specified morphology operation
	int morph_size = 1;
	Mat element = getStructuringElement( 1, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	morphologyEx( grad, grad, 5, element );
	morphologyEx( grad, grad, 5, element );
	morphologyEx( grad, grad, 5, element );

	// Create window
	string outs = OutImgPosition + OutImgName;
	imwrite(outs,grad);

	return 0;

}

Mat EdgeHandle::MatIllumination(Mat img){

	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;

	/// Load an image
	src = img;

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

	/// Convert it to gray
	cvtColor( src, src_gray, CV_RGB2GRAY );

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//sobel
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	// binary image.
	//threshold(grad,grad,0,255,THRESH_BINARY);

	// Apply the specified morphology operation
	int morph_size = 1;
	Mat element = getStructuringElement( 1, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	morphologyEx( grad, grad, 5, element );
	morphologyEx( grad, grad, 5, element );
	morphologyEx( grad, grad, 5, element );

	return grad;

}