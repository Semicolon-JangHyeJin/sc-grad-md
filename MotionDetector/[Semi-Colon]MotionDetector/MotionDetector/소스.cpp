#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

	namedWindow("Video", 1);
	namedWindow("Video1", 1);

	while (1)
	{
		Mat frame, gray_frame;
		cap >> frame;
		cvtCOlor(frame, gray_frame, COLOR_BGR2GRAY);
		imshow("Video1", frame);
		imshow("Video", gray_frame);

		if (waitKey(30) == 'c') break;
	}
	return 0;
}