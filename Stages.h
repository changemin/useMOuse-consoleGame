#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#include "ConsoleControl.h"
#include "ImageLayer.h"
#include "MouseInput.h"

#define MOUSE_NONE 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define SAME_BUTTON 3
#define SCROLLED 4
#define SELECT_BUTTON 5

int SelectFromN(int ButtonStatus, int LastChoice, int maxChoice) {
	int Current = LastChoice;
	switch (ButtonStatus) {
		case RIGHT_BUTTON:Current++; break;
		case LEFT_BUTTON:Current--; break;
		case SCROLLED:Current++; break;
	}
	if (Current > maxChoice) {
		Current = 1;
	}
	if (Current < 1) {
		Current = maxChoice;
	}
	return Current;
}

int StartStage() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[2] = {
		{"StartPage.bmp", 0, 0},
		{"selecter.bmp", 0,0}
	};
	imageLayer.imageCount = 2;
	imageLayer.images = startPage;
	imageLayer.renderAll(&imageLayer);

	while (1) {
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor)
		{
			case 1:imageLayer.images[1].y = 0; break;
			case 2:imageLayer.images[1].y = 300; break;
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		imageLayer.renderAll(&imageLayer);
	}
}
int GameExplainStage() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	Image startPage[2] = {
		{"ExplainPage.bmp", 0, 0},
		{"mini_Selector.bmp",0,0}
	};
	imageLayer.imageCount = 2;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor) 
		{
			case 1:imageLayer.images[1].x = 0; break;
			case 2:imageLayer.images[1].x = 1400; break;
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		imageLayer.renderAll(&imageLayer);
	}
}

int gamePause() {
	ImageLayer popUp = DEFAULT_IMAGE_LAYER;
	popUp.initialize(&popUp); //초기화
	Image startPage[3] = {
		{"StartPage.bmp",0,0},
		{"PausePOPUP.bmp", 0, 0},
		{"popUp_selector.bmp",0,0}
	};
	popUp.imageCount = 3;
	popUp.images = startPage;
	popUp.renderAll(&popUp);

	int selectCursor = 1;

	while (1) {
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor)
		{
		case 1:popUp.images[2].y = 0; break;
		case 2:popUp.images[2].y = 500; break;
		}
		if (MouseStatus == SELECT_BUTTON) {
			switch (selectCursor) {
			case 1:popUp.fadeOut(&popUp); break;break;
			case 2:popUp.fadeOut(&popUp); StartStage(); break;
			}
		}
		popUp.renderAll(&popUp);
	}
}

int SelectGameStage() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	Image startPage[3] = {
		{"GameSelectPage.bmp", 0, 0},
		{"thumbnail_avoidObstacleSOLOE.bmp",0,0},
		{"thumbnail_avoidObstacleDOUBLE.bmp",0,0}
	};
	imageLayer.imageCount = 3;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor)
		{
		case 1:
			imageLayer.images[1].x = 0;
			imageLayer.images[2].x = 700;
			break;
		case 2:
			imageLayer.images[1].x = -700; 
			imageLayer.images[2].x = 0;
			break;
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		if (MouseStatus == SAME_BUTTON) {
			return 0;
		}
		imageLayer.renderAll(&imageLayer);
	}
}

int AvoidObstacle_SOLO() {

}
