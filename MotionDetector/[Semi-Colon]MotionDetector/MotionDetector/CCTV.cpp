//CCTV Code

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp> // for cvtColor

#include <iostream>


using namespace cv;
using namespace std;


int main()
{

	Mat frame;

	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cerr << "ī�޶� �� �� �����ϴ�." << endl;
		return -1;
	}

	int fps = 30;
	int width = cap.get(CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);
	int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');

	VideoWriter outputVideo;
	outputVideo.open("output.avi", fourcc, fps, Size(width, height), true);


	while (1)
	{
		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ĸ�� ����" << endl;
			break;
		}

		//cvtColor(frame, frame, COLOR_BGR2GRAY);

		imshow("Live", frame);

		outputVideo.write(frame);

		if (waitKey(1) >= 0)
			break;
	}

	outputVideo.release();

	return 0;
}