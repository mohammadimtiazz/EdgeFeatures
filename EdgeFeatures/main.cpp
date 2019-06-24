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



void operateSobel(Mat &srcImg, Mat &xGrad, Mat &yGrad, Mat &grad, int xWeight, int yWeight) {
/*
Sobel operator:
1st apply small blur to remove noises
*/
	Mat grad_x, grad_y;

	GaussianBlur(srcImg, srcImg, Size(3, 3), 0, 0, BORDER_DEFAULT);	//applying blur
	Sobel(srcImg, grad_x, CV_32F, 1, 0);	//derivatives in x direction
	Sobel(srcImg, grad_y, CV_32F, 0, 1);	//derivative in y direction


	double minVal, maxVal;
	minMaxLoc(grad_x, &minVal, &maxVal);	//find minimum and maximum intensities
	//Mat xDraw, yDraw;
	grad_x.convertTo(xGrad, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

	minMaxLoc(grad_y, &minVal, &maxVal);	//find minimum and maximum intensities
	grad_y.convertTo(yGrad, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));


	// converting back to CV_8U
	Mat abs_grad_x, abs_grad_y;
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, double(xWeight / 100.0), abs_grad_y, (double(yWeight / 100.0)), 0, grad);

	grad_x.release();
	grad_y.release();
	abs_grad_x.release();
	abs_grad_y.release();

}


void operateLaplacian(Mat &srcImg, Mat &outImg, int kernalSize) {

	//Blur the input image first
	GaussianBlur(srcImg, srcImg, Size(7, 7), 1.5, 1.5, BORDER_DEFAULT);

	//Check kernal size 
	//it needs to an odd number
	if (kernalSize % 2 == 0)
		kernalSize++;
	printf("Kernal size applied in laplacian is: %d\n", kernalSize);

	Laplacian(srcImg, outImg, CV_16S, kernalSize);

	// converting back to CV_8U
	convertScaleAbs(outImg, outImg);
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

	Mat frame, grayImg, tmpGray, cannyEdges, sobelEdges;

	//for canny
	int lowThreshold = 0;
	const int max_lowThreshold = 100;
	int ratio = 3;
	int kernalSize = 3;
	int minKSize = 1;
	int maxKernalSize = 7;

	//for sobel
	int xWeight = 0, yWeight = 0;
	int maxWeight = 200;

	//Naming windows
	const string windowColor = "Raw images from webcam";
	const string windowCanny = "Feature1: Canny edges";
	const string windowSobel = "Feature2: Sobel edges";
	const string windowLaplacian = "Feature3: Laplacian edges";

	//Setting display windows 
	namedWindow(windowColor, 0);
	namedWindow(windowCanny, 0);
	namedWindow(windowSobel, 0);
	namedWindow(windowLaplacian, 0);

	//Resizing display window
	resizeWindow(windowColor, 400, 300);
	resizeWindow(windowCanny, 400, 600);
	resizeWindow(windowSobel, 400, 600);
	resizeWindow(windowLaplacian, 400, 600);

	//Setting trackber
	createTrackbar("MinThresh:", windowCanny, &lowThreshold, max_lowThreshold);
	createTrackbar("X weight:", windowSobel, &xWeight, maxWeight);
	createTrackbar("Y weight:", windowSobel, &yWeight, maxWeight);
	createTrackbar("kernal Size:", windowLaplacian, &minKSize, maxKernalSize);

	int key;
	bool stop = true;

	Mat grad_x, grad_y, grad;

	//Laplacian
	Mat tmpGray1, lapImg;

	while(stop)	
	{	
		cap >> frame; // get a new frame from camera

		cvtColor(frame, grayImg, COLOR_BGR2GRAY);	//converting into gray
		cannyEdges = grayImg.clone();
		tmpGray = grayImg.clone();
		tmpGray1 = grayImg.clone();

		operateCanny(cannyEdges, lowThreshold, ratio, kernalSize);		//apply canny

		operateSobel(tmpGray, grad_x, grad_y, grad, xWeight, yWeight);	//apply sobel


		operateLaplacian(tmpGray1, lapImg, minKSize);


		imshow(windowCanny, cannyEdges);
		imshow(windowColor, frame);
		imshow(windowSobel, grad);
		imshow(windowLaplacian, lapImg);
		////See for own interest
		//imshow("Sobel at X direction", grad_x);
		//imshow("Sobel at Y direction", grad_y);

		key = waitKey(1);
		if (key == 'q') {
			stop = false;
			break;
		}
			
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}