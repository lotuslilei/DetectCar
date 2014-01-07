#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class HOF{

	private:
		Mat velx;
		Mat vely;
	
	public:

		HOF();

		void test(const char*);
		void opticalFlow(const char*, const char*);
		void opticalFlow(Mat, Mat);
		Mat getVelX();
		Mat getVelY();
};