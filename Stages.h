/*
Stages.h는 모든 스테이지들에 대한 정보들을 포함한다. 
*/
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

#define HIT_MOUSE_TIMEOUT 10 //마우스를 괴롭혀라 게임 시간 제한(초)

#define Idx_AvoidObstacle1P 0 // 운석을 피해라 1P에 대한 게임 인덱스
#define Idx_AvoidObstacle2P 1 // 운석을 피해라 1P에 대한 게임 인덱스
#define Idx_HitTheMouse 2 // 마우스를 괴롭혀라에 대한 게임 인덱스

int MouseStatus = 0;

void enableMouseInput() {// 마우스 입력을 활성화 시킨다. 
	HWND CONSOLE_INPUT = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(CONSOLE_INPUT, &mode);
	SetConsoleMode(CONSOLE_INPUT, mode | ENABLE_MOUSE_INPUT);
}

DWORD __stdcall mouseInput(void* param) // 마우스 입력을 받아서 MouseStatus에 저장하는 함수 Thread로 돌아간다. 
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
		else if (ButtonState & MOUSE_WHEELED) {
			MouseStatus = 5;
			PlayNewMusic("BGM_WhenSelect");
		}
		else
			MouseStatus = 0;


	}
}

void MouseInputActivate() { // 마우스 입력을 활성화 시킨다. 
	HWND hWnd = GetConsoleWindow();
	CreateThread(NULL, 8 * 128 * 128, mouseInput, &hWnd, NULL, NULL);
}

int SelectFromN(int ButtonStatus, int LastChoice, int maxChoice) { // 게임과 같은 느낌을 내기 위해서 마우스 만을 사용하여 여러가지 선택지 중에서 한가지를 선택할 수 있게 해준다. 
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

int StartStage() { // 시작 페이지
	PlayMusic("BGM_StartPage.mp3", 1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[3] = {
		{"StartPage_BG.bmp", 0, 0},
		{"StartPage_GameStart.bmp", 0,0},
		{"StartPage_GameExplain.bmp", 0, 0}
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
				imageLayer.images[2].y = 700;
				break;
			}
				
			case 2: {
				imageLayer.images[1].y = 700;
				imageLayer.images[2].y = 0;
				break;
			}	
		}
		if (MouseStatus == 5) {
			imageLayer.fadeOut(&imageLayer);
			PlayMusic("BGM_StartPage.mp3", 0);
			return selectCursor;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int GameExplainStage() { // 게임 설명 페이지 
	PlayMusic("BGM_GameExplainPage.mp3", 1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	Image startPage[3] = {
		{"ExplainPage_BG.bmp", 0, 0},
		{"ExplainPage_Home.bmp",0,0},
		{"ExplainPage_GameSelect.bmp",0,0}
	};
	imageLayer.imageCount = 3;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {
		selectCursor = SelectFromN(MouseStatus, selectCursor, 2);
		switch (selectCursor) 
		{
			case 1: {
				imageLayer.images[1].y = 0;
				imageLayer.images[2].y = 1400;
				break;
			}
			case 2: {
				imageLayer.images[1].y = 1400;
				imageLayer.images[2].y = 0;
				break;
			}
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			PlayMusic("BGM_GameExplainPage.mp3", 0);
			return selectCursor;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int gamePause() { // 게임 멈춤
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

int SelectGameStage() { // 게임 선택 페이지 
	PlayMusic("BGM_GameSelect.mp3", 1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	Image startPage[4] = {
		{"GameSelectStage_BG.bmp", 0, 0},
		{"thumbnail_avoidObstacle1P.bmp",0,0},
		{"thumbnail_avoidObstacle2P.bmp",0,0},
		{"thumbnail_hitTheMouse.bmp",0,0}
	};
	imageLayer.imageCount = 4;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int selectCursor = 1;

	while (1) {

		selectCursor = SelectFromN(MouseStatus, selectCursor, 3);
		switch (selectCursor)
		{
			
			case 1: {
				imageLayer.images[1].x = 0;
				imageLayer.images[2].x = 1100;
				imageLayer.images[3].x = 2200;
				gotoxy(93, 17);
				printf("%d    ", ReadBestScore(Idx_AvoidObstacle1P));
				break;
			}
			case 2: {
				imageLayer.images[1].x = -1100;
				imageLayer.images[2].x = 0;
				imageLayer.images[3].x = 1100;
				gotoxy(93, 17);
				printf("%d    ", ReadBestScore(Idx_AvoidObstacle2P));
				break;
			}
			case 3: {
				imageLayer.images[1].x = -2200;
				imageLayer.images[2].x = -1100;
				imageLayer.images[3].x = 0;
				gotoxy(93, 17);
				printf("%d    ", ReadBestScore(Idx_HitTheMouse));
				break;
			}
		}
		if (MouseStatus == SELECT_BUTTON) {
			imageLayer.fadeOut(&imageLayer);
			PlayMusic("BGM_GameSelect.mp3", 0);
			return selectCursor;
		}
		if (MouseStatus == SAME_BUTTON) {
			PlayMusic("BGM_GameSelect.mp3", 0);
			return 0;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int AvoidObstacle_1P() { // 운석을 피해라 1P 게임 
	PlayMusic("BGM_AvoidObstacleGame.mp3", 1);
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
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, -400},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, -400},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, -400},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0, -400},
		{"AvoidObstacle1P_Obstacle_left.bmp", 0, -400},
		{"AvoidObstacle1P_Obstacle_right.bmp", 0,-400},
		{"AvoidObstacle1P_char.bmp", 0, 0},
		{"CountDown_3.bmp", 2000, 0},
		{"CountDown_2.bmp", 2000, 0},
		{"CountDown_1.bmp", 2000, 0}
	};
	imageLayer.imageCount = 11;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int score = 100;
	int BestScore = 100;

	Sleep(1000); // CountDown
	imageLayer.images[CountDown_3].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_3].x = 2000;
	imageLayer.images[CountDown_2].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_2].x = 2000;
	imageLayer.images[CountDown_1].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_1].x = 2000;
	imageLayer.renderAll(&imageLayer);

	Thorn thorn[thorn_n];
	main_char mainChar[1];
	Avoid1PInitObjects(thorn, mainChar);
	clock_t start_time, current_time;
	start_time = clock();
	while (1) {
		current_time = clock();
		score = (current_time - start_time) / CLOCKS_PER_SEC * 100;
		Avoid1PMoveObjects(thorn, mainChar, MouseStatus);

		if (mainChar->direction == 1) //Move Main Char
			imageLayer.images[mainCharIndex].x = 0;
		else if (mainChar->direction == 2)
			imageLayer.images[mainCharIndex].x = 650;
		for (int i = 1; i <= thorn_n; i++) { // paint thorns(가시)
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
			PlayMusic("BGM_AvoidObstacleGame.mp3", 0);
			imageLayer.fadeOut(&imageLayer);
			return 0;
		}
		if (Avoid1PCollisionCheck(thorn, mainChar->direction) == 1) {
			PlayMusic("BGM_AvoidObstacleGame.mp3", 0);
			return score;
		}
		MouseStatus = 0;
		gotoxy(155, 47);
		printf("%d", score);
		imageLayer.renderAll(&imageLayer);
	}
}

int AvoidObstacle_2P() { // 운석을 피해라 2P 게임 
	PlayMusic("BGM_AvoidObstacleGame.mp3", 1);
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
		{"AvoidObstacle2P_BG.bmp", 0, 0},
		{"AvoidObstacle2P_LeftLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightLeftObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_RightRightObstacle.bmp", 0, -300},
		{"AvoidObstacle2P_LeftChar.bmp", 0, 0},
		{"AvoidObstacle2P_RightChar.bmp", 0, 0},
		{"CountDown_3.bmp", 2000, 0},
		{"CountDown_2.bmp", 2000, 0},
		{"CountDown_1.bmp", 2000, 0}
	};
	imageLayer.imageCount = 18;
	imageLayer.images = startPage;
	imageLayer.fadeIn(&imageLayer);

	int score = 100;
	int BestScore = 100;

	Sleep(1000); // CountDown
	imageLayer.images[CountDown_3].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_3].x = 2000;
	imageLayer.images[CountDown_2].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_2].x = 2000;
	imageLayer.images[CountDown_1].x = 0;
	imageLayer.renderAll(&imageLayer);
	Sleep(1000);
	imageLayer.images[CountDown_1].x = 2000;
	imageLayer.renderAll(&imageLayer);

	Thorn thorn2P[thorn_n*2];
	main_char mainChar[2];
	Avoid2PInitObjects(thorn2P, mainChar);
	clock_t start_time, current_time;
	start_time = clock();
	while (1) {
		current_time = clock();
		score = (current_time - start_time) / CLOCKS_PER_SEC * 100;
		Avoid2PMoveObjects(thorn2P, mainChar, MouseStatus);

		if (mainChar[0].direction == 1) //Move Main Char
			imageLayer.images[mainCharLeftIndex].x = mainChar[0].x;
		else if (mainChar[0].direction == 2)
			imageLayer.images[mainCharLeftIndex].x = mainChar[0].x;
		if (mainChar[1].direction == 1)
			imageLayer.images[mainCharRightIndex].x = mainChar[1].x;
		else if (mainChar[1].direction == 2)
			imageLayer.images[mainCharRightIndex].x = mainChar[1].x;
		
		for (int i = 1; i <= thorn_n; i++) { // paint LEFT thorns
			if (thorn2P[i - 1].direction == 1) {
				imageLayer.images[i * 2 - 1].x = 0;
				imageLayer.images[i * 2 - 1].y = thorn2P[i - 1].y;
				imageLayer.images[i * 2].x = 2000;
				imageLayer.images[i * 2].y = thorn2P[i - 1].y;
			}
			else if (thorn2P[i - 1].direction == 2) {
				imageLayer.images[i * 2 - 1].x = 2000;
				imageLayer.images[i * 2 - 1].y = thorn2P[i - 1].y;
				imageLayer.images[i * 2].x = 0;
				imageLayer.images[i * 2].y = thorn2P[i - 1].y;
			}
		}
		for (int i = thorn_n; i <= thorn_n*2; i++) { // paint RIGHT thorns
			if (thorn2P[i - 1].direction == 1) {
				imageLayer.images[i * 2 - 1].x = 0;
				imageLayer.images[i * 2 - 1].y = thorn2P[i - 1].y;
				imageLayer.images[i * 2].x = 2000;
				imageLayer.images[i * 2].y = thorn2P[i - 1].y;
			}
			else if (thorn2P[i - 1].direction == 2) {
				imageLayer.images[i * 2 - 1].x =2000;
				imageLayer.images[i * 2 - 1].y = thorn2P[i - 1].y;
				imageLayer.images[i * 2].x = 0;
				imageLayer.images[i * 2].y = thorn2P[i - 1].y;
			}
		}

		if (MouseStatus == SELECT_BUTTON) {
			PlayMusic("BGM_AvoidObstacleGame.mp3", 0);
			imageLayer.fadeOut(&imageLayer);
			return 1;
		}
		if (Avoid2PCollisionCheck(thorn2P, mainChar) == 1) {
			PlayMusic("BGM_AvoidObstacleGame.mp3", 0);
			return score;
		}
		gotoxy(167, 47);
		printf("%d", score);
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
	}
}

int HitTheMouseGameStage() { // 마우스를 괴롭혀라 게임 
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화
	int selectCursor = 1;
	Image startPage[5] = {
		{"HitTheMouse_BG.bmp", 0, 0},
		{"CountDown_3.bmp", 2000, 0},
		{"CountDown_2.bmp", 2000, 0},
		{"CountDown_1.bmp", 2000, 0},
		{"HitTheMouse_TimeOut.bmp", 2000,0}
	};
	imageLayer.imageCount = 5;
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
			gotoxy(95, 33);
			printf("%d", score);
			if (MouseStatus == SELECT_BUTTON) {
				imageLayer.fadeOut(&imageLayer);
				return 0;
			}
			MouseStatus = 0;
		}
		else {
			imageLayer.images[4].x = 0;
			Sleep(1000);

			imageLayer.fadeOut(&imageLayer);
			return score;
		}
		imageLayer.renderAll(&imageLayer);
	}
}

int ShowScoreStage(int score, int GameIdx) { // 게임 오버 후 , 점수를 출력해주는 함수 .
	PlayMusic("BGM_GameOverBG.mp3", 1);
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
	RecordIfBestScore(GameIdx, score);
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
			PlayMusic("BGM_GameOverBG.mp3", 0);
			imageLayer.fadeOut(&imageLayer);
			return selectCursor;
		}
		MouseStatus = 0;
		imageLayer.renderAll(&imageLayer);
		gotoxy(100, 34);
		printf("%d", score);
	}
}
