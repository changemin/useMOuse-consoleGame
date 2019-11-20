#pragma once
#pragma comment (lib, "Msimg32.lib")
#include <Windows.h>
#include "ImageLayer.h"

#define CONSOLE_WIDTH 180
#define CONSOLE_HEIGHT 50
#define IMAGE_SCALE 2

#define RESOLUTION_SCALE 16
#define WINDOW_WIDTH (CONSOLE_WIDTH * RESOLUTION_SCALE)
#define WINDOW_HEIGHT (CONSOLE_HEIGHT * RESOLUTION_SCALE*2)

typedef struct{ // 가로 세로 구조체 선언
	int width, height;
}Size;

inline Size getBitmapSize(HBITMAP bitmap) { //콘솔창의 비트맵 사이즈를 리턴한다. 
	BITMAP tmpBitmap;
	GetObject(bitmap, sizeof(BITMAP), &tmpBitmap);
	const Size bitmapSize = { tmpBitmap.bmWidth, tmpBitmap.bmHeight };
	return bitmapSize;
}

inline HDC createNewBackDC(HDC compatibleDC) { //뒷배경을 생성한다. 
	const HDC backDC = CreateCompatibleDC(compatibleDC);
	const HBITMAP backBitmap = CreateCompatibleBitmap(compatibleDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(backDC, backBitmap);
	DeleteObject(backBitmap);
	return backDC;
}

inline void putBitmapToBackDC(HDC backDC, Image image, UINT transparentColor) { // 비트맵을 뒷배경에 복사한다. 
	const HDC bitmapDC = CreateCompatibleDC(backDC);
	const HBITMAP bitmap = (HBITMAP)LoadImage(NULL, (LPCWSTR)image.fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(bitmapDC, bitmap);

	double scale = image.scale;
	if (scale == 0) scale = IMAGE_SCALE;

	const Size bitmapSize = getBitmapSize(bitmap);
	TransparentBlt(backDC, image.x, image.y, bitmapSize.width * scale, bitmapSize.height * scale,
		bitmapDC, 0, 0, bitmapSize.width, bitmapSize.height, transparentColor);

	DeleteObject(bitmap);
	DeleteDC(bitmapDC);
}

inline void applyToConsoleDC(HDC consoleDC, HDC srcDC) { // 콘솔창에 적용한다. 
	BitBlt(consoleDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		srcDC, 0, 0, SRCCOPY);
}

inline void _initialize(ImageLayer* self) { // init, 선언
	self->_windowHandle = GetConsoleWindow();
	self->_consoleDC = GetDC(self->_windowHandle);
}

inline HDC getRenderedBackDC(ImageLayer* self) { // 렌더가 된 뒷 배경을 불러온다. 
	const HDC backDC = createNewBackDC(self->_consoleDC);

	for (int i = 0; i < self->imageCount; i++) {
		putBitmapToBackDC(backDC, self->images[i], self->transparentColor);
	}
	return backDC;
}

inline void _renderAll(ImageLayer* self) { // 변경사항을 렌더링 해서 보여준다. 
	const HDC backDC = getRenderedBackDC(self);
	applyToConsoleDC(self->_consoleDC, backDC);
	DeleteDC(backDC);
}

inline BLENDFUNCTION getBlendFunction() { // 투명도 처리를 하기 위한 함수
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 0;
	return bf;
}

inline void _renderAndFadeIn(ImageLayer* self) { // 어두운 화면부터 부드럽게 들어오는 효과를 낸다. 
	const HDC consoleDC = self->_consoleDC;
	const HDC backDC = getRenderedBackDC(self);

	const HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(consoleDC, blackBrush);

	BLENDFUNCTION bf = getBlendFunction();
	bf.SourceConstantAlpha = 12;

	Rectangle(consoleDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	for (int i = 0; i < 20; i++) {
		AlphaBlend(consoleDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			backDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bf);
	}
	applyToConsoleDC(consoleDC, backDC);
	DeleteObject(blackBrush);
	DeleteDC(backDC);
}

inline void applyToDC(HDC dstDC, HDC srcDC) { // DC에 적용한다. 
	BitBlt(dstDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		srcDC, 0, 0, SRCCOPY);
}

inline void _renderAndFadeOut(ImageLayer* self) { // 검정 화면과 함께 부드럽게 화면이 꺼진다. 
	const HDC consoleDC = self->_consoleDC;
	const HDC backDC = getRenderedBackDC(self);
	if (applyToDC != NULL) applyToDC(consoleDC, backDC);
	applyToDC(consoleDC, backDC);

	const HDC blackDC = createNewBackDC(consoleDC);
	BLENDFUNCTION bf = getBlendFunction();

	for (int i = 255; i >= 0; i -= 20) {
		bf.SourceConstantAlpha = i;
		applyToDC(consoleDC, blackDC);
		AlphaBlend(consoleDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			backDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bf);
		//Sleep(60);
	}
	applyToDC(consoleDC, blackDC);

	DeleteDC(backDC);
	DeleteDC(blackDC);
}