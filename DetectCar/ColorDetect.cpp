#include "ColorDetect.h"

ColorDetect::ColorDetect(){

}

void ColorDetect::SetMinDistance(int dist){
	minDist = dist;
}

void ColorDetect::SetTargetColor(Vec3b color){
	target = color;
}

void ColorDetect::SetTargetColor(uchar red,uchar green,uchar blue){
	target.val[0] = red;
	target.val[1] = green;
	target.val[2] = blue;
}

Mat ColorDetect::process(const Mat& image)
{
	Mat ImageLab=image.clone();
	result.create(image.rows,image.cols,CV_8U);

	//将image转换为Lab格式存储在ImageLab中
	cvtColor(image,ImageLab,CV_BGR2Lab);
	//将目标颜色由BGR转换为Lab
	Mat temp(1,1,CV_8UC3);
	temp.at<Vec3b>(0,0)=target;//创建了一张1*1的临时图像并用目标颜色填充
	cvtColor(temp,temp,CV_BGR2Lab);
	target=temp.at<Vec3b>(0,0);//再从临时图像的Lab格式中取出目标颜色

	// 创建处理用的迭代器
	Mat_<Vec3b>::iterator it=ImageLab.begin<Vec3b>();
	Mat_<Vec3b>::iterator itend=ImageLab.end<Vec3b>();
	Mat_<uchar>::iterator itout=result.begin<uchar>();
	while(it!=itend)
	{
		//两个颜色值之间距离的计算
		int dist=static_cast<int>(norm<int,3>(Vec3i((*it)[0]-target[0],
			(*it)[1]-target[1],(*it)[2]-target[2])));
		if(dist<minDist)
			(*itout)=255;
		else
			(*itout)=0;
		it++;
		itout++;
	}
	return result;
}
