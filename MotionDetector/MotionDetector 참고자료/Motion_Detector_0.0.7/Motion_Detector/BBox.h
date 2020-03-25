#pragma once
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/background_segm.hpp>
#include "configs.h"

/*
	바운딩 박스 객체

	CvRect 배열 형태구조이다.

	참조: Learning OpenCV, Countour, find connected component


	void	update(IplImage* foreground_clone) 
		: 내부적으로 이미지를 변형하므로 반드시 clone 이미지를 사용한다.

	int		length()
	operator [] : 배열처럼 접근한다.

	reference site : http://skkuassa.tistory.com/234
*/

class BBox
{
private:
	enum{MAX_BBOX_SIZE=256}; 

	CvMemStorage* storage;
	int num;
	float perimScale;
	CvRect bbs[MAX_BBOX_SIZE];
public:
	BBox(float _perimScale = 12.f);
	~BBox(void);
	void update(IplImage* foreground_clone);
	int length(void);
	CvRect& operator[](int idx);
	void drawBBox(IplImage* showimg);
};