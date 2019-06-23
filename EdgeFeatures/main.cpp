#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

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
	const int ratio = 3;
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

	for (;;)
	{	
		cap >> frame; // get a new frame from camera

		cvtColor(frame, edges, COLOR_BGR2GRAY);	//converting into gray

		/*
		Applying canny
		normally its recommended to blur the image slightly to remove shot noises
		then apply canny

		void Canny( InputArray image, OutputArray edges,double threshold1, double threshold2,int apertureSize = 3, bool L2gradient = false );
		*/

		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);	//bluring with gaussina
		Canny(edges, edges, lowThreshold, lowThreshold * ratio, kernalSize);

		imshow(windowCanny, edges);
		imshow(windowColor, frame);
		
		key = waitKey(1);
		if ( key == 'q')
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}