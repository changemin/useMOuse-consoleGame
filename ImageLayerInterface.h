#pragma once
#include <Windows.h>

typedef struct { //이미지 구조체
	wchar_t* fileName;
	int x, y;
	double scale;
}Image;

typedef struct _ImageLayer { // 이미지 레이어 구조체
	//멤버 변수
	Image* images;
	int imageCount;
	UINT transparentColor;

	HWND _windowHandle;
	HDC _consoleDC;

	//함수
	void (*initialize)(struct _ImageLayer*);
	void (*renderAll)(struct _ImageLayer*);
	void (*fadeIn)(struct _ImageLayer*);
	void (*fadeOut)(struct _ImageLayer*);
}ImageLayer;


