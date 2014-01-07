/************************************************************************/
/* 

generate the train objects by the data from PR class.

*/
/************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

class GenerateTrainImage{

public:
void generateTrainImage();
void splitNegImage();

};