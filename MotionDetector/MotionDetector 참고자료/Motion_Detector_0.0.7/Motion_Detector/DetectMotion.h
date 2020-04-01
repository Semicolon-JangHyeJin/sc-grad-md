#pragma once
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

// �������� ���� �� ����� �ȼ� ����
//UINT sub_image(IplImage *img1, IplImage *img2, IplImage *dst);

// IplImage -> HBITMAP
HBITMAP iplimage2hbitmap(const IplImage* Image);