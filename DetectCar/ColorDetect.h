#include <iostream>
#include <fstream>
#include <stdlib.h> //srand() rand()
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h> //time() 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

class ColorDetect{
private:
	int minDist; //minium acceptable distance
	Vec3b target;//target color;    
	Mat result; //the result
public:
	ColorDetect();
	void SetMinDistance(int dist);
	void SetTargetColor(uchar red,uchar green,uchar blue);
	void SetTargetColor(Vec3b color); //set the target color
	Mat process(const Mat& image); //main process
};