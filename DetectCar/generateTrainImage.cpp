#include "generateTrainImage.h"
#include "illumination.h"

//VideoList.txt 每行格式為 "type(0:car,1:mot) filename(no .avi)" ex."0 00120006_xvid"

void GenerateTrainImage::generateTrainImage(){

	int type;

	string VideoString;
	ifstream fin_video("D:\\Data\\PR\\TrainVideo\\VideoList.txt");
	ofstream fin_carimagesname("D:\\Data\\PR\\Pos\\Car\\CarImagesName.txt");
	ofstream fin_motimagesname("D:\\Data\\PR\\Pos\\Mot\\MotImagesName.txt");
	vector<string> VideoVector;
	int carImgCount = 1179, motImgCount = 768;

	while(getline(fin_video,VideoString)){

		VideoVector.assign(istream_iterator<string>(stringstream(VideoString)),istream_iterator<string>());
		istringstream typeString(VideoVector[0]);
		typeString >> type;
		if(type==0) cout<<"Car::";
		else cout<<"Mot::";

		string VideoName;
		if(type==0) VideoName = "D:\\Data\\PR\\TrainVideo\\Vehicle\\" + VideoVector[1] + ".avi";
		if(type==1) VideoName = "D:\\Data\\PR\\TrainVideo\\Motor\\" + VideoVector[1] + ".avi";
		cout<<"Handle TrainVideo: "<<VideoName<<endl;

		string PtsName;
		if(type==0) PtsName = "D:\\Data\\PR\\TrainVideo\\Vehicle\\" + VideoVector[1] + "_pts.txt";
		if(type==1) PtsName = "D:\\Data\\PR\\TrainVideo\\Motor\\" + VideoVector[1] + "_pts.txt";;

		ifstream pts_name(PtsName);
		//ofstream pts_name2(PtsName+"2");
		string ptsLine;
		
		VideoCapture myvideo;
		myvideo.open(VideoName);
		if(!myvideo.isOpened())  // check if we succeeded
			continue;
		cout<<"Frames Total: "<<myvideo.get(CV_CAP_PROP_FRAME_COUNT)<<endl;

		char saveName[256], sname[256];
		string saveString;
		int ptsFrame = 0;
		double pts1x, pts1y, pts2x, pts2y;
		vector<string> ptsVector;
		for(int f=1;f<myvideo.get(CV_CAP_PROP_FRAME_COUNT);f++)
		{
			if(f>ptsFrame){
				if(!getline(pts_name, ptsLine)) break;
				ptsVector.assign(istream_iterator<string>(stringstream(ptsLine)),istream_iterator<string>());
				istringstream buffer(ptsVector[0]);
				buffer >> ptsFrame;
			}

			Mat frame;
			myvideo >> frame; // get a new frame from camera

			if(f==ptsFrame){
				//pts_name2<<ptsFrame<<" ";
				for(int i=0; i<(ptsVector.size()-1)/4; i++){

					istringstream pt1x(ptsVector[i*4+1]);
					istringstream pt1y(ptsVector[i*4+2]);
					istringstream pt2x(ptsVector[i*4+3]);
					istringstream pt2y(ptsVector[i*4+4]);
					pt1x >> pts1x;
					pt1y >> pts1y;
					pt2x >> pts2x;
					pt2y >> pts2y;
					//pts_name2 << (int)pts1x+8 << " " << pts1y << " " << (int)pts2x+8 << " " << pts2y << " ";

					Mat myCrop, frame_ill;
					//Illumination ill;
					//frame_ill = ill.MatIllumination(frame);

					if(type==0) { //car
						double x = abs(pts1x-pts2x);
						double a = 1.1*x;
						double b = 0.2*x;
						double c = 0.3*x;

						int cropx = (pts1x+pts2x)/2-x/2-b;
						int cropy = (pts1y+pts2y)/2-a;
						Rect myRect(cropx, cropy, x+2*b, a+c);
						if(myRect.x<0||myRect.y<0||myRect.x+myRect.width>=frame.cols||myRect.y+myRect.height>=frame.rows) continue; 
						myCrop = frame(myRect);
						sprintf(saveName,"D:\\Data\\PR\\Pos\\Car\\o_%06d.jpg",++carImgCount);
						sprintf(sname,"o_%06d.jpg",carImgCount);
						fin_carimagesname<<sname<<endl;
						//rectangle (frame, myRect, Scalar( 0, 255, 255 ), 1, 8, 0);
					}
					else if(type==1) { //mot
						double x = abs(pts1y-pts2y);
						double a = 1.2*x;
						int b = 0.4*x;
						double c = 0.2*x;

						int cropx = (pts1x+pts2x)/2-b;
						int cropy = (pts1y+pts2y)/2-x/2-a;
						Rect myRect(cropx, cropy, (int)2*b, (int)6*b);
						if(myRect.x<0||myRect.y<0||myRect.x+myRect.width>=frame.cols||myRect.y+myRect.height>=frame.rows||b<10) continue; 
						myCrop = frame(myRect);
						sprintf(saveName,"D:\\Data\\PR\\Pos\\Mot\\o_%06d.jpg",++motImgCount);
						sprintf(sname,"o_%06d.jpg",motImgCount);
						fin_motimagesname<<sname<<endl;
						//rectangle (frame, myRect, Scalar( 0, 255, 255 ), 1, 8, 0);
					}
					//imwrite(saveName, frame);
					imwrite(saveName, myCrop);
				}
				//pts_name2 << endl;
			}
		}
	}
}

int NegCount = 0;

void GenerateTrainImage::splitNegImage(){
	ifstream car_negList("D:\\Data\\PR\\Neg\\TrainNegativeCarImageList.txt");
	ofstream mot_negList("D:\\Data\\PR\\Neg\\TrainNegativeMotImageList.txt");
	string car_negName;
	while(getline(car_negList,car_negName)){
		string carName = "D:\\Data\\PR\\Neg\\Car\\"+car_negName;
		Mat src_img = imread(carName);
		Mat img(144,144,src_img.type());
		resize(src_img,img,img.size(),0,0,INTER_LINEAR);
		Rect myRect1(0, 0, 48, 144), myRect2(48, 0, 48, 144), myRect3(96, 0, 48, 144);
		Mat Crop1 = img(myRect1), Crop2 = img(myRect2), Crop3 = img(myRect3);
		char saveName[256], sname[256];
		sprintf(saveName,"D:\\Data\\PR\\Neg\\Mot\\c_noperson%06d.jpg",++NegCount);
		sprintf(sname,"c_noperson%06d.jpg",NegCount);
		imwrite(saveName, Crop1);
		mot_negList<<sname<<endl;
		sprintf(saveName,"D:\\Data\\PR\\Neg\\Mot\\c_noperson%06d.jpg",++NegCount);
		sprintf(sname,"c_noperson%06d.jpg",NegCount);
		imwrite(saveName, Crop2);
		mot_negList<<sname<<endl;
		sprintf(saveName,"D:\\Data\\PR\\Neg\\Mot\\c_noperson%06d.jpg",++NegCount);
		sprintf(sname,"c_noperson%06d.jpg",NegCount);
		imwrite(saveName, Crop3);
		mot_negList<<sname<<endl;
	}
}