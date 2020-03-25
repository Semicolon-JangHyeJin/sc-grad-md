#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "GMM.h"
#include "BBox.h"

class ImageManager
{
public:
	ImageManager();
	virtual ~ImageManager();

public:
	GMM *mGMM;					// Gaussian Mixture Model 객체
	BBox *mBBox;				// Binding Box 객체

	CvCapture*	mCapture;		// Capture

	IplImage*	currentImage;	// 현재 이미지
	IplImage*	beforeImage;	// 1프레임 전 이미지
	IplImage*	resultImage;	// 테스트에 사용하는 차영상 결과 이미지

	BOOL isRunning;				// 시작되었는 지 저장

public:
	BOOL Start();				// imagemanager 시작
	void Stop();				// imagemanager 중지
	int GrabFrame(int mDiffLevel);			// 현재 프레임 잡기
	void CopyCurrentToBefore();	// currentImage를 beforeImage로 복사
};