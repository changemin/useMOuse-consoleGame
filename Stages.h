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
		switch (isMouseClicked())
		{
		case LEFT_BUTTON:popUp.images[2].y = 0; goto exit; break;
		case RIGHT_BUTTON:popUp.images[2].y = 500; popUp.fadeOut(&popUp); StartStage(); break;
		}
		popUp.renderAll(&popUp);
	exit:
		return 0;
	}
}

int SelectGameStage() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	Image startPage[6] = {
		{"GameSelectPage.bmp", 0, 0},
		{"thumbnail_avoidObstacle1P.bmp",0,0},
		{"thumbnail_avoidObstacle2P.bmp",0,0},
		{"thumbnail_avoidStar.bmp",0,0},
		{"thumbnail_goGoSled.bmp",0,0},
		{"thumbnail_hitTheMouse.bmp",0,0}
	};
	imageLayer.imageCount = 6;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {
		Sleep(100);
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 5);
		switch (selectCursor)
		{
		case 1:
			imageLayer.images[1].x = 0;
			imageLayer.images[2].x = 700;
			imageLayer.images[3].x = 1400;
			imageLayer.images[4].x = 2100;
			imageLayer.images[5].x = 2800;
			break;
		case 2:
			imageLayer.images[1].x = -700;
			imageLayer.images[2].x = 0;
			imageLayer.images[3].x = 700;
			imageLayer.images[4].x = 1400;
			imageLayer.images[5].x = 2100;
			break;
		case 3:
			imageLayer.images[1].x = -1400;
			imageLayer.images[2].x = -700;
			imageLayer.images[3].x = 0;
			imageLayer.images[4].x = 700;
			imageLayer.images[5].x = 1400;
			break;
		case 4:
			imageLayer.images[1].x = -2100;
			imageLayer.images[2].x = -1400;
			imageLayer.images[3].x = -700;
			imageLayer.images[4].x = 0;
			imageLayer.images[5].x = 700;
			break;
		case 5:
			imageLayer.images[1].x = -2800;
			imageLayer.images[2].x = -2100;
			imageLayer.images[3].x = -1400;
			imageLayer.images[4].x = -700;
			imageLayer.images[5].x = 0;
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
void printScore(int x, int y, int score, int bestScore) {
	gotoxy(x, y);
	printf("┌────────────────────┐\n");
	printf("│ Your Score : %d\n", score);
	printf("│ best Score : %d\n", bestScore);
	printf("└────────────────────┘");
}
int AvoidObstacle_1P() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[4] = {
		{"AvoidObstacle1P_BG.bmp", 0, 0},
		{"AvoidObstacle1P_char.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 500},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 800}
	};
	imageLayer.imageCount = 4;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int score = 0;
	int BestScore = 100;
	while (1) {
		printScore(0, 20, score, BestScore);
		int MouseStatus = isMouseClicked();
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		gotoxy(20, 20);
		switch (selectCursor)
		{
		case 1:imageLayer.images[1].x = 0; break;
		case 2:imageLayer.images[1].x = 500; break;
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return 0;
		}
		imageLayer.renderAll(&imageLayer);
	}
	
}

