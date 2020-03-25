#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager() 
{
	// opencv �� ���� ���� �ʱ�ȭ
	mCapture = NULL;
	currentImage = beforeImage = resultImage = NULL;
	isRunning = FALSE;

	// GMM �� BBox ��ü ���� (default)
	mGMM = new GMM(GMM_LEARN_CNT);
	mBBox = new BBox(BBOX_PERIM_SCALE);
}

ImageManager::~ImageManager()
{
	// CAM ���� ���� �� ���ҽ� ���� 
	if( mCapture != NULL ) cvReleaseCapture( &mCapture );
	if( beforeImage != NULL ) cvReleaseImage( &beforeImage );
	if( resultImage != NULL ) cvReleaseImage( &resultImage );

	// ��ü ����
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
	// CAM ����
	mCapture = cvCaptureFromCAM(0);
	if(mCapture == NULL) {
		AfxMessageBox(L"ī�޶� ������� �ʾҽ��ϴ�.");
		isRunning = FALSE;
	} else {
		// �ʱ�ȭ
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
		// �� ������ �޾ƿ���
		currentImage = cvQueryFrame(mCapture);

		// GMM update
		mGMM->update(currentImage, mDiffLevel);

		// BBox update
		mBBox->update(mGMM->foreground());
#if DEBUG_GMM_BBOX
		cvShowImage("FG", mGMM->foreground());
		//cvShowImage("BG", mGMM->backgroundOri());

		// BBox �׸���
		mBBox->drawBBox(currentImage);
#endif
	}

	return mBBox->length();
}

void ImageManager::CopyCurrentToBefore()
{
	cvCopy(currentImage, beforeImage);
}