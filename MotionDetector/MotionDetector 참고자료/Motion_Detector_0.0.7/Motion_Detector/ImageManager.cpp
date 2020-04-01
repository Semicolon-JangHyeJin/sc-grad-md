#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager() 
{
	// opencv 용 각종 변수 초기화
	mCapture = NULL;
	currentImage = beforeImage = resultImage = NULL;
	isRunning = FALSE;

	// GMM 및 BBox 객체 생성 (default)
	mGMM = new GMM(GMM_LEARN_CNT);
	mBBox = new BBox(BBOX_PERIM_SCALE);
}

ImageManager::~ImageManager()
{
	// CAM 연결 해제 및 리소스 정리 
	if( mCapture != NULL ) cvReleaseCapture( &mCapture );
	if( beforeImage != NULL ) cvReleaseImage( &beforeImage );
	if( resultImage != NULL ) cvReleaseImage( &resultImage );

	// 객체 제거
	if( mGMM != NULL ) {
		delete mGMM;
		mGMM = NULL;
	}
	if( mBBox != NULL ) {
		delete mBBox;
		mBBox = NULL;
	}
}

BOOL ImageManager::Start()
{
	// CAM 연결
	mCapture = cvCaptureFromCAM(0);
	if(mCapture == NULL) {
		AfxMessageBox(L"카메라가 연결되지 않았습니다.");
		isRunning = FALSE;
	} else {
		// 초기화
		currentImage = cvQueryFrame(mCapture);

		if(beforeImage == NULL && resultImage == NULL) {
			beforeImage = cvCreateImage(cvGetSize(currentImage), 8, 3);
			resultImage = cvCreateImage(cvGetSize(currentImage), 8, 3);
		}

		isRunning = TRUE;
	}

	return isRunning;
}

void ImageManager::Stop()
{
	if( mCapture != NULL ) cvReleaseCapture(&mCapture);

	isRunning = FALSE;
}

int ImageManager::GrabFrame(int mDiffLevel)
{
	if( mCapture != NULL ) {
		// 한 프레임 받아오기
		currentImage = cvQueryFrame(mCapture);

		// GMM update
		mGMM->update(currentImage, mDiffLevel);

		// BBox update
		mBBox->update(mGMM->foreground());
#if DEBUG_GMM_BBOX
		cvShowImage("FG", mGMM->foreground());
		//cvShowImage("BG", mGMM->backgroundOri());

		// BBox 그리기
		mBBox->drawBBox(currentImage);
#endif
	}

	return mBBox->length();
}

void ImageManager::CopyCurrentToBefore()
{
	cvCopy(currentImage, beforeImage);
}