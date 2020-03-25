#pragma once
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/background_segm.hpp>
#include "configs.h"

/*
	�ٿ�� �ڽ� ��ü

	CvRect �迭 ���±����̴�.

	����: Learning OpenCV, Countour, find connected component


	void	update(IplImage* foreground_clone) 
		: ���������� �̹����� �����ϹǷ� �ݵ�� clone �̹����� ����Ѵ�.

	int		length()
	operator [] : �迭ó�� �����Ѵ�.

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