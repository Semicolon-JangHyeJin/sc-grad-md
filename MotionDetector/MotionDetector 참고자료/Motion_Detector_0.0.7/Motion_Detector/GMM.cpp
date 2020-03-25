#include "stdAfx.h"
#include "GMM.h"

GMM::GMM(int _learnCnt)
{
	learnCnt = _learnCnt;
	frameCnt = 0;
	bgModel = NULL;
	fgclone = NULL;
}


GMM::~GMM(void)
{
	if(bgModel != NULL){
		cvReleaseBGStatModel(&bgModel);
	}
	if(fgclone != NULL){
		cvReleaseImage(&fgclone);
	}
}

// update Gaussian Mixture Model
void GMM::update(IplImage* curr, int mDiffLevel)
{
	if(bgModel == NULL){
		bgModel = cvCreateGaussianBGModel(curr, NULL);
	}else{

		// chk channels
		IplImage* curr_gray;
		if(curr->nChannels == 3){
			curr_gray = cvCreateImage(cvGetSize(curr), IPL_DEPTH_8U, 1);
			cvCvtColor(curr, curr_gray, CV_RGB2GRAY);

		}else if(curr->nChannels == 1){
			curr_gray = curr;
		}else{
			// exception
			return;
		}

		// learning rate
		if(frameCnt++ <= learnCnt){
			cvUpdateBGStatModel(curr_gray, bgModel, -1);// learn
		}else{
			cvUpdateBGStatModel(curr_gray, bgModel, 0);
		}

		if(curr->nChannels == 3){
			cvReleaseImage(&curr_gray);
		}
	}

	if(fgclone){
		cvCopy(bgModel->foreground, fgclone, NULL);
	
	} else {
		fgclone = cvCloneImage(bgModel->foreground);
	}

	// remove salt & pepper noise
	cvSmooth(fgclone, fgclone, CV_MEDIAN, 5);

	// 기본 foreground 이미지를 침식/팽창을 통해 작은건 없애고 필요한건 크게 부각시킴
	cvDilate(fgclone, fgclone, NULL, 15);
	cvErode(fgclone, fgclone, NULL, 19 - mDiffLevel*2);
}

// getter method , foreground image
IplImage* GMM::foreground(void)
{
	return fgclone;
}


// 이것들은 안쓰는게 좋다.
IplImage* GMM::foregroundOri(void)
{
	return bgModel->foreground; 
}

IplImage* GMM::backgroundOri(void)
{
	return bgModel->background;
}