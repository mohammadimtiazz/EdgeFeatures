#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;



void operateCanny(Mat &srcImg, int thresh, int mult, int kSize) {
	/*
	Applying canny
	normally its recommended to blur the image slightly to remove shot noises
	then apply canny
	*/

	GaussianBlur(srcImg, srcImg, Size(7, 7), 1.5, 1.5);	//bluring with gaussina
	Canny(srcImg, srcImg, thresh, thresh * mult, kSize);

	printf("Lower Threshold: %d Upper Threshold: %d\n", thresh, thresh * mult);

}



int main(int, char**)
{
	// open the default camera, usually it always the laptop's webcam 
	//unless some other default settings are in place
	VideoCapture cap(0);

	// check if we succeeded
	if (!cap.isOpened()) {
		printf("Couldn't able to located default webcam\n");
		return -1;
	}

	Mat frame, edges;

	int lowThreshold = 0;
	const int max_lowThreshold = 100;
	int ratio = 3;
	int kernalSize = 3;

	//Naming windows
	const string windowColor = "Raw images from webcam";
	const string windowCanny = "Feature1: Canny edges";

	//Setting display windows 
	namedWindow(windowColor, 0);
	namedWindow(windowCanny, 0);

	//Resizing display window
	resizeWindow(windowColor, 400, 300);
	resizeWindow(windowCanny, 400, 300);
	createTrackbar("MinThresh:", windowCanny, &lowThreshold, max_lowThreshold);

	int key;
	bool stop = true;

	while(stop)	
	{	
		cap >> frame; // get a new frame from camera

		cvtColor(frame, edges, COLOR_BGR2GRAY);	//converting into gray

		operateCanny(edges, lowThreshold, ratio, kernalSize);

		imshow(windowCanny, edges);
		imshow(windowColor, frame);
		
		key = waitKey(1);
		if (key == 'q') {
			stop = false;
			break;
		}
			
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}