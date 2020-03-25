#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


using namespace cv;
using namespace std;


int main()
{

	Mat frame;

	VideoCapture cap("output.avi");
	if (!cap.isOpened())
	{
		cerr << "ī�޶� �� �� �����ϴ�." << endl;
		return -1;
	}


	while (1)
	{
		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ĸ�� ����" << endl;
			break;
		}


		imshow("Live", frame);

		if (waitKey(30) >= 0)
			break;
	}


	return 0;
}
