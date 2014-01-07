/************************************************************************/
/* 

Use to make the train images objects have the same size to use HOG

*/
/************************************************************************/

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

class ResizeImage{

public:
	int resizeImage(string src_img_location,string src_img_name,int width,int height);
	void resizeImageList(string src_img_location, string src_img_list, int width, int height);
};