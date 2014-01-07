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

class ShapeDetect{

public:

	ShapeDetect();
	IplImage* EqualizeHistColorImage(IplImage *pImage);
	Mat DetectCarPlateOrLight( Mat src, int choosePlateOrLight );
	int DetectShape(Mat src);
};