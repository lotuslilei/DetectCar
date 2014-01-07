/************************************************************************/
/*      

the file use to train good and bad examples.

IMPUT: 
CarPos.txt[include images in D:\Data\PR\Pos\Original] which include good examples.
CarNeg.txt[include images in D:\Data\PR\Neg\Original] which include bad examples.
CarHard.txt[include images in D:\Data\PR\Hard\Original] which include hard examples.

HANDLE:
Pos: Cropped by user --> Handle Illumination --> Store
Neg: Auto Crop --> Handle Illumination --> Store
Hard: Auto Crop --> Handle Illumination --> Store

OUTPUT
D:\Data\PR\Pos\Car && D:\Data\PR\Pos\Mot which store the cropped images as good examples.
D:\Data\PR\Neg\Car && D:\Data\PR\Neg\Mot which store the cropped images as bad examples.
D:\Data\PR\Hard\Car && D:\Data\PR\Hard\Mot which store the cropped images as hard examples.

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

class Train{

private:
	//the number of images of different kinds of train.
	int PosSamNO;
	int NegSamNO;
	int HardExampleNO;

	//the size of object you trained.
	int ObjWidth;
	int ObjHeigh;

public:

	 Train();	//start train

	 int setObjSize(int obj_width, int obj_heigh);
	 int setSamNO(int pos_num, int neg_num, int hard_num);

	 int trainHOGSVM(string PosSamPos, string PosSamListFile, 
		 string NegSamPos, string NegSamListFile, 
		 string HardSamPos, string HardSamListFile,
		 string OutputSampleFeatureMatrixFile, string OutputHOGSVMFile);

};