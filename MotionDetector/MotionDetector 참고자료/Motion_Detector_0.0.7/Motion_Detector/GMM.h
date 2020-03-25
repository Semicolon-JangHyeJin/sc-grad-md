#pragma once
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/background_segm.hpp>

/*
	가우시안 믹스쳐 모델 Wrapping 객체

	void		update(IplImage* img)
	IplImage*	foreground()

	reference : http://skkuassa.tistory.com/233
*/

class GMM
{
private:
	CvBGStatModel* bgModel;
	int learnCnt;
	int frameCnt;

	IplImage* fgclone;
public:
	GMM(int _learnCnt = 20);
	~GMM(void);
	void update(IplImage* curr, int mDiffLevel);
	IplImage* foreground(void);

	// 이것들은 안쓰는게 좋다.
	IplImage* foregroundOri(void);
	IplImage* GMM::backgroundOri(void);
};