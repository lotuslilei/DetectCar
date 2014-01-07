#include "generatenegativeimages.h"

int TrainNegImg::trainNeg( int cutWidth, int cutHeigh , string inImgPos, string inImgNameList, string outImgPos, string outImgNameList ){

	int CropImageCount = 0;
	Mat src;
	string ImgName;

	//the filename of cropped images.
	char saveName[256];
	//the input images list file.
	ifstream finObj(inImgNameList);
	ofstream myimagename(outImgNameList.c_str());

	//read the file line by line.
	while(getline(finObj,ImgName))
	{
		//input
		cout<<"Train Neg Handle: "<<ImgName<<endl;
		ImgName = inImgPos + ImgName;
		src = imread(ImgName);//read images

		//the oral image must big over 64*128
		if(src.cols >= cutWidth && src.rows >= cutHeigh)
		{
			srand(time(NULL));

			//cropping...
			for(int i=0; i<5; i++)
			{
				int x = ( rand() % (src.cols-cutWidth) ); //左上角x坐标
				int y = ( rand() % (src.rows-cutHeigh) ); //左上角y坐标
				Mat imgROI = src(Rect(x,y,cutWidth,cutHeigh)); 
				sprintf(saveName,"noperson%06d.jpg",++CropImageCount);//generate the filename of cropped images
				string so = outImgPos + saveName;
				imwrite(so, imgROI);//store images.
				myimagename<<saveName<<endl;	//write to output images name in XXX.txt
			}
		}else{
			cout<<"Too Small Image!"<<endl;
		}
	}

	return 0;

}