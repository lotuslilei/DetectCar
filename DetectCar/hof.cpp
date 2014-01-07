#include <iostream>
#include <string>
#include <cv.h> 
#include <highgui.h> 
#include <gpu/gpu.hpp>
#include "hof.h"

#define UNKNOWN_FLOW_THRESH 1e9  
/************************************************************************/
/* HOG的Cell,Block大小設定，預設參數為PAPER中建議的最佳參數，通常不用調          */
/************************************************************************/
#define Bin_num  9
#define Block_size 16
#define Block_stride 8
#define Cell_size 8


/************************************************************************/
/* Slide Window大小設定: 依照偵測的目標物體，需要調整slide window的長寬設定
	行人偵測的window建議設定為64*128或32*64(W*H)
	汽車偵測的window可以設定為64*56或64*64(W*H)								*/
/************************************************************************/
#define Win_width 64
#define Win_height 64

using namespace std;
using namespace cv;

void makecolorwheel(vector<Scalar> &colorwheel)  
{  
    int RY = 15;  
    int YG = 6;  
    int GC = 4;  
    int CB = 11;  
    int BM = 13;  
    int MR = 6;  
  
    int i;  
  
    for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255,       255*i/RY,     0));  
    for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255-255*i/YG, 255,       0));  
    for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0,         255,      255*i/GC));  
    for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0,         255-255*i/CB, 255));  
    for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255*i/BM,      0,        255));  
    for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255,       0,        255-255*i/MR));  
}  
  
void motionToColor(Mat flow, Mat &color)  
{  
    if (color.empty())  
        color.create(flow.rows, flow.cols, CV_8UC3);  
  
    static vector<Scalar> colorwheel; //Scalar r,g,b  
    if (colorwheel.empty())  
        makecolorwheel(colorwheel);  
  
    // determine motion range:  
    float maxrad = -1;  
  
    // Find max flow to normalize fx and fy  
    for (int i= 0; i < flow.rows; ++i)   
    {  
        for (int j = 0; j < flow.cols; ++j)   
        {  
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);  
            float fx = flow_at_point[0];  
            float fy = flow_at_point[1];  
            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))  
                continue;  
            float rad = sqrt(fx * fx + fy * fy);  
            maxrad = maxrad > rad ? maxrad : rad;  
        }  
    }  
  
    for (int i= 0; i < flow.rows; ++i)   
    {  
        for (int j = 0; j < flow.cols; ++j)   
        {  
            uchar *data = color.data + color.step[0] * i + color.step[1] * j;  
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);  
  
            float fx = flow_at_point[0] / maxrad;  
            float fy = flow_at_point[1] / maxrad;  
            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))  
            {  
                data[0] = data[1] = data[2] = 0;  
                continue;  
            }  
            float rad = sqrt(fx * fx + fy * fy);  
  
            float angle = atan2(-fy, -fx) / CV_PI;  
            float fk = (angle + 1.0) / 2.0 * (colorwheel.size()-1);  
            int k0 = (int)fk;  
            int k1 = (k0 + 1) % colorwheel.size();  
            float f = fk - k0;  
            //f = 0; // uncomment to see original color wheel  
  
            for (int b = 0; b < 3; b++)   
            {  
                float col0 = colorwheel[k0][b] / 255.0;  
                float col1 = colorwheel[k1][b] / 255.0;  
                float col = (1 - f) * col0 + f * col1;  
                if (rad <= 1)  
                    col = 1 - rad * (1 - col); // increase saturation with radius  
                else  
                    col *= .75; // out of range  
                data[2 - b] = (int)(255.0 * col);  
            }  
        }  
    }  
}  

HOF::HOF()
{
	// do nothing
}

void HOF::test(const char* filename)
{
	//char filename[]="0.jpg"; // 想要取得的影像名稱
	IplImage *image; // 影像的資料結構
	image = cvLoadImage(filename,CV_LOAD_IMAGE_UNCHANGED); // 讀取影像的資料結構
	if(!image){
		cout<<"Error: Couldn't open the image file.\n";
	}
	cvShowImage("HelloWorld", image); // 顯示影像於視窗
	cvWaitKey(0); // 停留視窗
	cvDestroyWindow("HelloWorld"); // 銷毀視窗
	cvReleaseImage(&image); // 釋放IplImage資料結構
}

void HOF::opticalFlow(const char* preImage, const char* Image)
{  
    IplImage *aaa = cvLoadImage(preImage,CV_LOAD_IMAGE_UNCHANGED);

	Mat preGray = imread(preImage, CV_LOAD_IMAGE_GRAYSCALE);
	Mat gray = imread(Image, CV_LOAD_IMAGE_GRAYSCALE);
    Mat flow, cflow, frame;  
    Mat motion2color;  
    
  
    //cvtColor(frame, gray, CV_BGR2GRAY);  
    //imshow("original", frame);  
	//cvShowImage("original", aaa);
   
    calcOpticalFlowFarneback(preGray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);  
    motionToColor(flow, motion2color);  
	Mat velx = Mat(flow.size(), CV_32F);
	Mat vely = Mat(flow.size(), CV_32F);
	for(int i=0; i<flow.rows; i++)
	{
		for(int j=0; j<flow.cols; j++)
		{
			//velx.at<double>(i,j) = 1.0;
			velx.at<float>(i,j) = flow.at<cv::Point2f>(i, j).x;
			//vely.at<double>(i,j) = 1.0;
			vely.at<float>(i,j) = flow.at<cv::Point2f>(i, j).y;
		}
	}
    //imshow("flow", motion2color);  
	imshow("velx", velx);
	imshow("vely", vely);

	//HOGDescriptor *hogx = new HOGDescriptor(flow.size(),cvSize(Block_size,Block_size),cvSize(Block_stride,Block_stride),cvSize(Cell_size,Cell_size),Bin_num);
	//vector<float>xdescriptors;
	//hogx->compute(velx, xdescriptors, Size(1,1), Size(0,0));
	//printf("descriptor size = %d\n",xdescriptors.size());

	cvWaitKey(0);
}

void HOF::opticalFlow(Mat preGray, Mat gray)
{  
    //IplImage *aaa = cvLoadImage(preImage,CV_LOAD_IMAGE_UNCHANGED);

	//Mat preGray = imread(preImage, CV_LOAD_IMAGE_GRAYSCALE);
	//Mat gray = imread(Image, CV_LOAD_IMAGE_GRAYSCALE);
    Mat flow, cflow, frame;  
    Mat motion2color;  
    
  
    //cvtColor(frame, gray, CV_BGR2GRAY);  
    //imshow("original", frame);  
	//cvShowImage("original", aaa);
   
    calcOpticalFlowFarneback(preGray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);  
    motionToColor(flow, motion2color);  
	velx = Mat(flow.size(), CV_32F);
	vely = Mat(flow.size(), CV_32F);
	for(int i=0; i<flow.rows; i++)
	{
		for(int j=0; j<flow.cols; j++)
		{
			//velx.at<double>(i,j) = 1.0;
			velx.at<float>(i,j) = flow.at<cv::Point2f>(i, j).x;
			//vely.at<double>(i,j) = 1.0;
			vely.at<float>(i,j) = flow.at<cv::Point2f>(i, j).y;
		}
	}
    //imshow("flow", motion2color);  
	//imshow("velx", velx);
	//imshow("vely", vely);
	//imwrite("velx.png", velx*255);
	//imwrite("vely.png", vely*255);

	//HOGDescriptor *hogx = new HOGDescriptor(flow.size(),cvSize(Block_size,Block_size),cvSize(Block_stride,Block_stride),cvSize(Cell_size,Cell_size),Bin_num);
	//vector<float>xdescriptors;
	//hogx->compute(velx, xdescriptors, Size(1,1), Size(0,0));
	//printf("descriptor size = %d\n",xdescriptors.size());

	//cvWaitKey(0);
}

Mat HOF::getVelX(){
	return velx;
}

Mat HOF::getVelY(){
	return vely;
}