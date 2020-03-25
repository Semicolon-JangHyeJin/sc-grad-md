
//사진 불러오기 코드
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image;

	image = imread("./후니.jpg", 1);

	if (!image.data) {
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	namedWindow("window", 1);
	imshow("window", image);

	imwrite("result.jpg", image);

	waitKey(0);
	return 9;
}



//웹캠으로부터 영상 가져오기
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
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		imshow("Video1", frame);
		imshow("Video", gray_frame);

		if (waitKey(30) == 'c') break;
	}
	return 0;
}


//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/ocl.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//void capPicture(Mat image) {
//
//}
//
//int main() {
//	VideoCapture cap(0);
//	//VideoCapture cap1(0);
//	namedWindow("RGB image", 1);
//	//namedWindow("Gray image", 1);
//
//	if (!cap.isOpened()) {
//		cout << "Could not open or find the image" << endl;
//		return -1;
//	}
//	Mat frame, gray, frameDelta, thresh, firstFrame;
//	vector<vector<Point> > cnts;
//	while (1) {
//		//Mat frame;
//		//Mat gray;
//		cap.read(frame);
//		gray = frame;
//		GaussianBlur(gray, gray, Size(21, 21), 0);
//
//		cap.read(frame);
//		GaussianBlur(frame, frame, Size(21, 21), 0);
//
//		//cap.read(gray);
//
//		//cvtColor(image, gray_Image, COLOR_BGR2GRAY);
//		absdiff(firstFrame, gray, frameDelta);
//		threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
//
//		dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
//		findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//		for (int i = 0; i < cnts.size(); i++) {
//			if (contourArea(cnts[i]) < 1000) {
//				continue;
//			}
//
//			putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//		}
//
//		imshow("RGB image", frame);
//		//imshow("Gray image", gray);
//
//		if(waitKey(1)==27) break;
//	}
//
//	return 0;
//}

#pragma warning(disable:4996)

//#include <opencv2/tracking.hpp>
//#include <unistd.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <thread>

using namespace cv;
using namespace std;
thread t;

const std::string currentDateTime() {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return buf;
}

void capPicture(Mat image) {
	mutex lock();
	char savefile[200];
	sprintf(savefile, "image_%d.jpg", currentDateTime());
	imwrite(savefile, image);        // img를 파일로 저장한다.
	printf("Wating...");
	for (int i = 0; i < 10000; i++);
	mutex unlock();
}

int main(int argc, char** argv) {

	Mat frame, gray, frameDelta, thresh, firstFrame;
	vector<vector<Point> > cnts;
	VideoCapture camera(0); //open camera

	//set the video size to 512x288 to process faster
	camera.set(3, 512);
	camera.set(4, 288);

	//sleep(3);

	camera.read(frame);
	//convert to grayscale and set the first frame
	cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
	GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

	while (camera.read(frame)) {

		//convert to grayscale
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(21, 21), 0);

		//compute difference between first frame and current frame
		absdiff(firstFrame, gray, frameDelta);
		threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);

		dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
		findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < cnts.size(); i++) {
			if (contourArea(cnts[i]) < 1500) {
				continue;
			}
			t = thread(capPicture, frame);
			t.join();
			putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}

		imshow("Camera", frame);
		camera.read(frame);
		//convert to grayscale and set the first frame
		cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
		GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
		if (waitKey(1) == 27) {
			//exit if ESC is pressed
			break;
		}

	}

	return 0;
}