#include "generateImage.h"

GenerateImage::GenerateImage(){
	totalFrameNumber = 0;
}

int GenerateImage::setVideo( string inputVideoPosAndName ){
	try{
		useVideo.open(inputVideoPosAndName);
		totalFrameNumber = useVideo.get(CV_CAP_PROP_FRAME_COUNT);
	}catch(int e){
		cout<<"Open Video Failed: "<<inputVideoPosAndName<<endl;
	}

	return 0;
}

Mat GenerateImage::getImg( long frame ){

	//store
	try{
		useVideo.set( CV_CAP_PROP_POS_FRAMES,frame);
		useVideo.read(useframe);
		return useframe;
	}catch(int e){
		cout<<" The video not have the frame of No."<<frame<<endl;
	}
}

CvSize GenerateImage::getVideoSize(){
	try{
		CvSize oc;
		oc.width = useVideo.get(CV_CAP_PROP_FRAME_WIDTH);
		oc.height = useVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
		return oc;
	}catch(int e){
		cout<<" The input video is not legal"<<endl;
	}
}

bool GenerateImage::hasImg( long frame ){
	if ( frame <= totalFrameNumber )
	{
		return true;
	}else{
		return false;
	}
}

int GenerateImage::VideoToImgStore( string inputVideo, string outputImgPos, string outputImgNameListPos, string outputImgNameListName ){

	VideoCapture myvideo;
	string outputImgNameList = outputImgNameListPos + outputImgNameListName;
	ofstream myimagename(outputImgNameList.c_str());

	//input
	string VideoName = inputVideo;
	cout<<"Handle Video: "<<VideoName<<endl;
	try{
		myvideo.open(VideoName);
	}catch(int e){
		cout<<"Open Video Failed: "<<VideoName<<endl;
	}

	//store
	int imagCount = 0;
	char saveImgName[256];
	string saveimg;
	char saveListName[256];//store to imagesname.txt
	strcpy(saveListName,outputImgNameList.c_str());
	try{
		for(;;)
		{
			Mat frame;
			myvideo >> frame; // get a new frame from camera
			cvtColor(frame, frame, CV_BGR2GRAY);
			sprintf(saveImgName,"v_%06d.jpg",++imagCount);//generate the filename of cropped images
			saveimg = outputImgPos + saveImgName;

			imwrite(saveimg, frame);//write the image.

			myimagename<<saveImgName<<endl;	//write to output images name in XXX.txt

		}
	}catch(int e){
		cout<<"Store Images Finished to "<<outputImgPos<<endl;
	}

	return 0;
}