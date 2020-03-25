//#pragma warning(disable:4996)
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <time.h>
//
//using namespace cv;
//
//const std::string currentDateTime() {
//	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링
//
//	return buf;
//}
//
//int main() {
//	Mat img;                // 동영상 파일 재생을 위한 행렬 선언
//	VideoCapture cap(0);    // 캠으로부터 영상을 받아온다 (혹은 1번) cam : 0번
//
//	int count = 0;
//	char savefile[200];        // 이미지 파일 이름을 200자 이내로 제한하기 위한 char 변수 선언
//
//	while (1) {
//		cap >> img;
//		imshow("image", img);      // 영상 출력    
//
//		//resize(img, img, Size(100, 100), 0, 0, INTER_CUBIC);    // 사진으로 저장할 때는 크기를 downsizing해서 저장한다
//
//		if (waitKey(10) == 32) {
//			sprintf(savefile, "image_%d.jpg", currentDateTime());
//			imwrite(savefile, img);        // img를 파일로 저장한다.
//		}
//		 
//		if (waitKey(100) == 27)  break; // esc를 누르면 종료한다
//	}
//	return 0;
//}

//카메라로부터 영상 가져오기
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // for cvtColor

#include <iostream>


using namespace cv;
using namespace std;


int main()
{

	Mat frame; //matrix 자료형의 frame이라는 이름을 가진 친구 만들기
	Mat frame2;
	VideoCapture cap(0); //비디오캡쳐를 하기 위해 카메라가 연결된 포트(괄호 안) IO 사용

	//cap이 열리지 않았는지 확인하기(cap은 비디오 IO이다)
	if (!cap.isOpened())
	{
		//error 표시
		cerr << "카메라를 열 수 없습니다." << endl;
		return -1;
	}

	while (1)
	{
		//cap으로부터 한 frame씩 읽어와 frame 변수에 저장
		cap.read(frame);

		if (frame.empty())
		{
			cerr << "캡쳐 실패" << endl;
			break;
		}

		cvtColor(frame, frame2, COLOR_BGR2GRAY);
		//Live라는 이름을 가진 창을 열어 frame의 정보를 출력한다.
		imshow("Live", frame);
		imshow("binary", frame2);
		//0 이상의 정보를 가진 키를 입력하면 창 종료
		if (waitKey(1) >= 0)
			break;
	}


	return 0;
}

/*
	vector<int> v;
	v.reserve(10);

	for (int i = 0; i < 10; i++) {
		v.push_back(i*10);
	}

	for (int i = 0; i < 10; i++) {
		cout << v[i] << endl;
	}

	cout << "size:" << v.size() << endl //벡터 원소 개수
		<< "capacity:" << v.capacity() << endl //벡터의 할당된 메모리의 크기
		<< "max_size" << v.max_size() << endl; //최대 저장 가능한 원소 개수

	v.resize(15);

	v.clear();

*/