#pragma warning(disable:4996)

//#include <opencv2/tracking.hpp>
//#include <unistd>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace cv;
using namespace std;

int insert = 0;

std::string currentDateTime() {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��
	return buf;
}

void capPicture(Mat image) {
	if (insert == 0) {
		//char savefile[200];
		string savefile = "image_" + currentDateTime() + ".jpg";
		//sprintf(savefile, "image_%d.jpg", currentDateTime());
		printf("cap %s\n", savefile);
		imwrite(savefile, image);        // img�� ���Ϸ� �����Ѵ�.
		insert++;
		putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);

	}
	else if (insert > 3) {
		//char savefile[200];
		string savefile = "image_" + currentDateTime() + ".jpg";
		//sprintf(savefile, "image_%d.jpg", currentDateTime());
		imwrite(savefile, image);        // img�� ���Ϸ� �����Ѵ�.
		printf("cap %s\n", savefile);
		insert = 0;
		putText(image, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
	}
	else {
		insert++;
	}

}

int main(int argc, char** argv) {

		Mat frame, gray, frameDelta, thresh, firstFrame;
		vector<vector<Point> > cnts;
		VideoCapture camera(0); //open camera
		int v = 0;
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
				capPicture(frame);
			}

			imshow("Camera", frame);
			//camera.read(frame);
			Sleep(10);
			//convert to grayscale and set the first frame
			//v++;
			//if (v > 10) {
			cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
			GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
			//v = 0;
		//}
			if (waitKey(1) == 27) {
				//exit if ESC is pressed
				break;
			}

		}

	return 0;
}