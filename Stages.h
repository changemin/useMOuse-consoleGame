#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#include "ConsoleControl.h" // 콘솔 제어 헤더
#include "ImageLayer.h" // 이미지 관련 헤더
#include "Objects.h"

#define MOUSE_NONE 0 // 아무것도 누르지 않았을 때
#define LEFT_BUTTON 1 // 왼쪽 버튼
#define RIGHT_BUTTON 2 // 오른쪽 버튼
#define SAME_BUTTON 3 // 좌, 우 함께 눌렀을 때
#define SCROLLED 4 // 스크롤 움직임
#define SELECT_BUTTON 5 // 마우스 휠 버튼 

#define HIT_MOUSE_TIMEOUT 3 //hit the mouse 게임 시간 제한(초)

int MouseStatus = 0;

void enableMouseInput() {
	HWND CONSOLE_INPUT = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(CONSOLE_INPUT, &mode);
	SetConsoleMode(CONSOLE_INPUT, mode | ENABLE_MOUSE_INPUT);
}

DWORD __stdcall mouseInput(void* param)
{
	INPUT_RECORD rec;
	DWORD dwNOER;
	DWORD mode;
	HWND hWnd = *(HWND*)param;

	enableMouseInput();
	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);
		const DWORD ButtonState = rec.Event.MouseEvent.dwButtonState;
		const DWORD EventFlags = rec.Event.MouseEvent.dwEventFlags;

		if (ButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			MouseStatus = 1;
		else if (ButtonState & RIGHTMOST_BUTTON_PRESSED)
			MouseStatus = 2;
		else if ((ButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && (ButtonState & RIGHTMOST_BUTTON_PRESSED))
			MouseStatus = 3;
		else if (EventFlags & MOUSE_WHEELED)
			MouseStatus = 4;
		else if (ButtonState & MOUSE_WHEELED)
			MouseStatus = 5;
		else
			MouseStatus = 0;


	}
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode);
}

void MouseInputActivate() {
	HWND hWnd = GetConsoleWindow();
	CreateThread(NULL, 8 * 1024 * 1024, mouseInput, &hWnd, NULL, NULL);
}

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
	Image startPage[3] = {
		{"Intro_BG.bmp", 0, 0},
		{"Intro_game_description.bmp", 0,0},
		{"Intro_game_start.bmp", 0, 0}
	};
	imageLayer.imageCount = 3;
	imageLayer.images = startPage;
	imageLayer.renderAll(&imageLayer);
	while (1) {
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor)
		{
			case 1: {
				imageLayer.images[1].y = 600;
				imageLayer.images[2].y = 0;
				break;
			}
				
			case 2: {
				imageLayer.images[1].y = 0;
				imageLayer.images[2].y = 600;
				break;
			}	
		}
		if (MouseStatus == 5) {
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		MouseStatus = 0;
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
		MouseStatus = 0;
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
		switch (MouseStatus)
		{
		case LEFT_BUTTON:popUp.images[2].y = 0; goto exit; break;
		case RIGHT_BUTTON:popUp.images[2].y = 500; popUp.fadeOut(&popUp); StartStage(); break;
		}
		popUp.renderAll(&popUp);
	exit:
		return 0;
		MouseStatus = 0;
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
		{"thumbnail_hitTheMouse.bmp",0,0},
		{"thumbnail_goGoSled.bmp",0,0}
	};
	imageLayer.imageCount = 6;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {
		Sleep(100);
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
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int AvoidObstacle_1P() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	const int mainCharIndex = 7;
	const int BGIndex = 0;
	const int CountDown_1 = 10;
	const int CountDown_2 = 9;
	const int CountDown_3 = 8;
	Image startPage[11] = {
		{"AvoidObstacle1P_BG.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_char.bmp", 0, 0},
		{"HitTheMouseGame_3.bmp", 2000, 0},
		{"HitTheMouseGame_2.bmp", 2000, 0},
		{"HitTheMouseGame_1.bmp", 2000, 0}
	};
	imageLayer.imageCount = 11;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int score = 100;
	int BestScore = 100;
	
	/*Sleep(1000); // CountDown
	imageLayer.images[2].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[2].x = 2000;
	imageLayer.images[3].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[3].x = 2000;
	imageLayer.images[4].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[4].x = 2000;
	imageLayer.renderAll(&imageLayer);*/
	
	Thorn thorn[thorn_n];
	main_char mainChar[1];
	Avoid1PInitObjects(thorn, mainChar);
	clock_t start_time, current_time;
	start_time = clock();
	while (1) {
		current_time = clock();
		score = (current_time - start_time)/CLOCKS_PER_SEC*100;
		Avoid1PMoveObjects(thorn, mainChar, MouseStatus);
		
		if (mainChar->direction == 1) //Move Main Char
			imageLayer.images[mainCharIndex].x = 0;
		else if (mainChar->direction == 2)
			imageLayer.images[mainCharIndex].x = 500;
		for (int i = 1; i <= thorn_n; i++) { // paint thorns(가시)
			if (thorn[i-1].direction == 1) {
				imageLayer.images[i*2 - 1].x = 0;
				imageLayer.images[i*2 - 1].y = thorn[i-1].y;
				imageLayer.images[i*2].x = 2000;
				imageLayer.images[i*2].y = thorn[i-1].y;
			}
			else if (thorn[i-1].direction == 2) {
				imageLayer.images[i*2-1].x = 2000;
				imageLayer.images[i*2-1].y = thorn[i-1].y;
				imageLayer.images[i*2].x = 0;
				imageLayer.images[i*2].y = thorn[i-1].y;
			}
		}

		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return 0;
		}
		if (Avoid1PCollisionCheck(thorn, mainChar->direction) == 1) {
			return score;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int AvoidObstacle_2P() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	const int mainCharLeftIndex = 13;
	const int mainCharRightIndex = 14;
	const int BGIndex = 0;
	const int CountDown_1 = 17;
	const int CountDown_2 = 16;
	const int CountDown_3 = 15;
	Image startPage[18] = {
		{"AvoidObstacle1P_BG.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, 0},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, 0},
		{"AvoidObstacle1P_char.bmp", 0, 0},
		{"AvoidObstacle1P_char.bmp", 0, 0},
		{"HitTheMouseGame_3.bmp", 2000, 0},
		{"HitTheMouseGame_2.bmp", 2000, 0},
		{"HitTheMouseGame_1.bmp", 2000, 0}
	};
	imageLayer.imageCount = 18;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int score = 100;
	int BestScore = 100;

	/*Sleep(1000); // CountDown
	imageLayer.images[2].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[2].x = 2000;
	imageLayer.images[3].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[3].x = 2000;
	imageLayer.images[4].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[4].x = 2000;
	imageLayer.renderAll(&imageLayer);*/

	Thorn thorn[thorn_n*2];
	main_char mainChar[2];
	Avoid2PInitObjects(thorn, mainChar);
	clock_t start_time, current_time;
	start_time = clock();
	while (1) {
		current_time = clock();
		score = (current_time - start_time) / CLOCKS_PER_SEC * 100;
		Avoid2PMoveObjects(thorn, mainChar, MouseStatus);

		if (mainChar[0].direction == 1) //Move Main Char
			imageLayer.images[mainCharLeftIndex].x = -500;
		else if (mainChar[0].direction == 2)
			imageLayer.images[mainCharLeftIndex].x = 100;
		if (mainChar[1].direction == 1)
			imageLayer.images[mainCharRightIndex].x = 0;
		else if (mainChar[1].direction == 2)
			imageLayer.images[mainCharRightIndex].x = 600;
		
		for (int i = 1; i <= thorn_n*2; i++) { // paint thorns(가시)
			if (thorn[i - 1].direction == 1) {
				imageLayer.images[i * 2 - 1].x = 0;
				imageLayer.images[i * 2 - 1].y = thorn[i - 1].y;
				imageLayer.images[i * 2].x = 2000;
				imageLayer.images[i * 2].y = thorn[i - 1].y;
			}
			else if (thorn[i - 1].direction == 2) {
				imageLayer.images[i * 2 - 1].x = 2000;
				imageLayer.images[i * 2 - 1].y = thorn[i - 1].y;
				imageLayer.images[i * 2].x = 0;
				imageLayer.images[i * 2].y = thorn[i - 1].y;
			}
		}

		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			return 0;
		}
		/*if (Avoid2PCollisionCheck(thorn, mainChar->direction) == 1) {
			return score;
		}*/
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int HitTheMouseGameStage() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[4] = {
		{"HitTheMouseGame_BG.bmp", 0, 0},
		{"HitTheMouseGame_3.bmp", 2000, 0},
		{"HitTheMouseGame_2.bmp", 2000, 0},
		{"HitTheMouseGame_1.bmp", 2000, 0}
	};
	imageLayer.imageCount = 4;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	Sleep(1000);
	imageLayer.images[1].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[1].x = 2000;
	imageLayer.images[2].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[2].x = 2000;
	imageLayer.images[3].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[3].x = 2000;
	imageLayer.renderAll(&imageLayer);
	
	int score = 0;
	int BestScore = 100;
	clock_t start_time, current_time;
	start_time = clock();
	while (1) {
		current_time = clock();
		if ((current_time - start_time) < (HIT_MOUSE_TIMEOUT * CLOCKS_PER_SEC)) {
			if (MouseStatus == LEFT_BUTTON || MouseStatus == RIGHT_BUTTON) {
				score++;
			}
			gotoxy(90, 24);
			printf("%d", score);
			if (MouseStatus == SELECT_BUTTON) {
				imageLayer.fadeOut(&imageLayer);
				return 0;
			}
			MouseStatus = 0;
		}
		else {
			imageLayer.images[3].x = 0;
			Sleep(1000);
			gotoxy(0, 0);
			imageLayer.fadeOut(&imageLayer);
			return score;
		}
		imageLayer.renderAll(&imageLayer);
	}
}

int ShowScoreStage(int score) {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[3] = {
		{"showScore_BG.bmp", 0, 0},
		{"showScore_home.bmp", 0,0},
		{"showScore_retry.bmp", 0, 0}
	};
	imageLayer.imageCount = 3;
	imageLayer.images = startPage;
	imageLayer.renderAll(&imageLayer);
	while (1) {
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor)
		{
			case 1: {
				imageLayer.images[1].y = 0;
				imageLayer.images[2].y = 600;
				break;
			}
			case 2: {
				imageLayer.images[1].y = 600;
				imageLayer.images[2].y = 0;
				break;
			}
		}
		if (MouseStatus == 5) {
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
		gotoxy(80, 25);
		printf("%d", score);
	}
}
