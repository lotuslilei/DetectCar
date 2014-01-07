#include "resize.h"

int ResizeImage::resizeImage(string src_img_location,string src_img_name, int width, int height){

	//input
	string inimage;
	string outimage;
	Mat src_img;
	try
	{
		inimage = src_img_location + src_img_name;
		outimage = src_img_location + "c_" + src_img_name;
		src_img=imread(inimage,1);
	}
	catch (int e)
	{
		cout<< "Failed input the src_crop image: "<<inimage<<endl;
	}

	//handle and output
	try{
		Mat dst_img_rsize(width,height,src_img.type());
		resize(src_img,dst_img_rsize,dst_img_rsize.size(),0,0,INTER_LINEAR);
		imwrite(outimage,dst_img_rsize);
	}catch(int e){
		cout<< "Failed store cropped the image: "<<outimage<<endl;
	}

	return 0;
}

void ResizeImage::resizeImageList (string src_img_location, string src_img_list, int width, int height){
	ifstream img_list(src_img_location + src_img_list);
	string img_file_name;
	while(getline(img_list,img_file_name)){
		resizeImage(src_img_location, img_file_name, width, height);
	}
}