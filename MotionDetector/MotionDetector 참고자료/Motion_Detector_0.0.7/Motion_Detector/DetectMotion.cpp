#include "stdafx.h"
#include "DetectMotion.h"

#define TEST 0

/*UINT sub_image(IplImage *img1, IplImage *img2, IplImage *dst) {
	int value1, value2, value3;
	int cnt=0, ratio=0, level=0;

	for(int x=0; x<img1->height; x++) { //이미지 차연산
		for(int y=0; y <img1->width; y++) { 
			value1 = abs( ((BYTE)img1->imageData[x*img1->widthStep + 3*y ]) -  ((BYTE)img2->imageData[x*img2->widthStep + 3*y] ) ) ;
			value2 = abs( ((BYTE)img1->imageData[x*img1->widthStep + 3*y  + 1]) -  ((BYTE)img2->imageData[x*img2->widthStep + 3*y] +1 ) )  ;
			value3 = abs( ((BYTE)img1->imageData[x*img1->widthStep + 3*y  + 2]) -  ((BYTE)img2->imageData[x*img2->widthStep + 3*y] + 2) ) ;
			
			if(value1>40 && value2>40 && value3>40) { // RGB 가 모두 40이상 차이날 경우
				dst->imageData[x*dst->widthStep + 3*y  + 0] =  (BYTE)img1->imageData[x*img1->widthStep + 3*y  + 0];
				dst->imageData[x*dst->widthStep + 3*y  + 1] =  (BYTE)img1->imageData[x*img1->widthStep + 3*y  + 1];
				dst->imageData[x*dst->widthStep + 3*y  + 2] =  (BYTE)img1->imageData[x*img1->widthStep + 3*y  + 2];
				cnt++; //카운트 함
			} else {
				dst->imageData[x*dst->widthStep + 3*y  + 0] =  0;
				dst->imageData[x*dst->widthStep + 3*y  + 1] =  0;
				dst->imageData[x*dst->widthStep + 3*y  + 2] =  0;
			}
		} 
	}
	
	ratio = cnt==0?0:(cnt*100/(img1->width*img1->height));

	if(ratio >= 5 && ratio < 10) {
		level = 9;
	} else if(ratio >= 10 && ratio < 15) {
		level = 8;
	} else if(ratio >= 15 && ratio < 20) {
		level = 7;
	} else if(ratio >= 20 && ratio < 25) {
		level = 6;
	} else if(ratio >= 25 && ratio < 30) {
		level = 5;
	} else if(ratio >= 30 && ratio < 35) {
		level = 4;
	} else if(ratio >= 35 && ratio < 40) {
		level = 3;
	} else if(ratio >= 40 && ratio < 45) {
		level = 2;
	} else if(ratio == 100) {
		level = 1;
	}

	if(ratio > 0 && ratio < 10) {
		level = 9;
	} else if(ratio >= 10 && ratio < 20) {
		level = 8;
	} else if(ratio >= 20 && ratio < 30) {
		level = 7;
	} else if(ratio >= 30 && ratio < 40) {
		level = 6;
	} else if(ratio >= 40 && ratio < 50) {
		level = 5;
	} else if(ratio >= 50 && ratio < 60) {
		level = 4;
	} else if(ratio >= 60 && ratio < 70) {
		level = 3;
	} else if(ratio >= 70 && ratio < 80) {
		level = 2;
	} else if(ratio == 100) {
		level = 1;
	}
	//IplImage *temp = NULL;
	IplImage *grey_image = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
	
	cvSmooth(img1, img1, CV_GAUSSIAN, 3, 1); // remove false positive
	if(!dst) {
		dst = cvCloneImage(img1);
		temp = cvCloneImage(img1);
		cvConvertScale(img1, img2, 1.0, 0.0);
	} else {
		cvRunningAvg(img1, img2, 0.020, NULL);
	}

	//cvConvertScale(img2, temp, 1.0, 0.0);
	cvAbsDiff(img1, img2, dst);
	cvCvtColor(dst, grey_image, CV_RGB2GRAY);
	cvThreshold(grey_image, grey_image, 70, 255, CV_THRESH_BINARY);
	cvDilate(grey_image, grey_image, NULL, 18);
	cvErode(grey_image, grey_image, NULL, 10);

#if TEST
	//char text[30];
	//CvFont font;

	// ratio 표시
	//sprintf (text,"Ratio=%d%%,Level=%d", ratio, level);
	//cvInitFont (&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 1,1);
	//cvPutText (img1, text, cvPoint (4, img1->height - 10), &font, CV_RGB (255, 255, 255));
	cvShowImage("diff", grey_image);
#endif

	if(grey_image != NULL) cvReleaseImage( &grey_image );
	//if(temp != NULL) cvReleaseImage( &temp );

	return 1;
}*/

HBITMAP iplimage2hbitmap(const IplImage* Image) {
   int bpp = Image->nChannels * 8;
   assert(Image->width >= 0 && Image->height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
   CvMat dst;
   void* dst_ptr = 0;
   HBITMAP hbmp = NULL;
   unsigned char buffer[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
   BITMAPINFO* bmi = (BITMAPINFO*)buffer;
   BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

   ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
   bmih->biSize = sizeof(BITMAPINFOHEADER);
   bmih->biWidth = Image->width;
   bmih->biHeight = Image->origin ? abs(Image->height) : -abs(Image->height);
   bmih->biPlanes = 1;
   bmih->biBitCount = bpp;
   bmih->biCompression = BI_RGB;

   if (bpp == 8) {
     RGBQUAD* palette = bmi->bmiColors;
     int i;
     for (i = 0; i < 256; i++) {
       palette[i].rgbRed = palette[i].rgbGreen = palette[i].rgbBlue = (BYTE)i;
       palette[i].rgbReserved = 0;
     }
   }

   hbmp = CreateDIBSection(NULL,bmi,DIB_RGB_COLORS,(void **) &dst_ptr,NULL,0 );
   cvInitMatHeader(&dst, Image->height, Image->width, CV_8UC3, dst_ptr, (Image->width * Image->nChannels + 3) & -4);
   cvConvertImage(Image, &dst, Image->origin ? CV_CVTIMG_FLIP : 0);

   return hbmp;
}