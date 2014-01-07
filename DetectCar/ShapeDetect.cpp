#include <opencv2/opencv.hpp>
#include "ShapeDetect.h"
#include "ColorDetect.h"

ShapeDetect::ShapeDetect(){

}

IplImage* ShapeDetect::EqualizeHistColorImage(IplImage *pImage){
	IplImage *pEquaImage = cvCreateImage(cvGetSize(pImage), pImage->depth, 3);

	// 原图像分成各通道后再均衡化,最后合并即彩色图像的直方图均衡化
	const int MAX_CHANNEL = 4;
	IplImage *pImageChannel[MAX_CHANNEL] = {NULL};

	int i;
	for (i = 0; i < pImage->nChannels; i++)
		pImageChannel[i] = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);

	cvSplit(pImage, pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3]);

	for (i = 0; i < pImage->nChannels; i++)
		cvEqualizeHist(pImageChannel[i], pImageChannel[i]);

	cvMerge(pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3], pEquaImage);

	for (i = 0; i < pImage->nChannels; i++)
		cvReleaseImage(&pImageChannel[i]);

	return pEquaImage;
}

Mat ShapeDetect::DetectCarPlateOrLight( Mat src, int choosePlateOrLight ){
	Mat hsv; 
	Mat dst; 
	Mat mask=Mat::zeros(src.rows,src.cols, CV_8U); //為了濾掉其他顏色
	Mat r,r2,w; //各顏色的閥值

	cvtColor(src,hsv,CV_BGR2HSV);//轉成hsv平面

	if ( choosePlateOrLight == 0 )
	{
		//use to car lights
		inRange(hsv,Scalar(0,43,46) , Scalar(10,255,255), r);             
		inRange(hsv,Scalar(156,43,46) , Scalar(180,255,255), r2);      
		mask = r + r2;//red

		src.copyTo(dst,mask ); //將原圖片經由遮罩過濾後，得到結果dst
		// Apply the specified morphology operation
		int morph_size = 1;
		Mat element = getStructuringElement( 1, Size( 2*morph_size + 1, 2*morph_size + 1 ), Point( morph_size, morph_size ) );
		morphologyEx( dst, dst, 1, element );

	}else if ( choosePlateOrLight == 1 )
	{
		//use to car plate
		inRange(hsv,Scalar(0,0,200) ,Scalar(180,30,255), w);       

		mask= w;//white
		src.copyTo(dst,mask ); //將原圖片經由遮罩過濾後，得到結果dst
		// Apply the specified morphology operation
		int morph_size = 1;
		Mat element = getStructuringElement( 1, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
		morphologyEx( dst, dst, 1, element );
		morphologyEx( dst, dst, 1, element );
		morphologyEx( dst, dst, 1, element );
		morphologyEx( dst, dst, 1, element );
		morphologyEx( dst, dst, 1, element );
		morphologyEx( dst, dst, 1, element );


		/*imshow("",dst);
		moveWindow("",0,0);
		cvWaitKey(0);*/

	}else{
		src.copyTo(dst,mask ); //將原圖片經由遮罩過濾後，得到結果dst
	}

	return dst;
}

int ShapeDetect::DetectShape(Mat src){

	cvtColor(src,src,CV_BGR2GRAY);//轉成hsv平面
	cv::Size s = src.size();
	int size = s.width/5;  
	int flag;
	int cnt = 0;

	for ( int i=0; i<s.width-size; i++){
		for ( int j=0; j<s.height-size; j++){
			flag = 1;
			for ( int m=0; m<size; m++){
				for ( int n=0; n<size; n++){
					if(src.at<uchar>(j+n, i+m) == 0)
						flag = 0;
				}
			}
			if(flag == 1)
				return 1;
		}
	}
	return 0;

}