#pragma once
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

// 차영상을 만든 후 변경된 픽셀 측정
//UINT sub_image(IplImage *img1, IplImage *img2, IplImage *dst);

// IplImage -> HBITMAP
HBITMAP iplimage2hbitmap(const IplImage* Image);