#pragma once

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>

// 콘솔창 색상 전처리 선언
#define BLACK "0"
#define BLUE "1"
#define GREEN "2"
#define BLUE_GREEN "3"
#define RED "4"
#define WINE "5"
#define YELLOW "6"
#define WHITE "7"
#define GRAY "8"
#define SKY_BLUE "9"
#define LIGHT_GREEN "A"
#define LIGHT_BLUE_GREEN "B"
#define LIGHT_RED "C"
#define LIGHT_WINE "D"
#define LIGHT_YELLOW "E"
#define LIGHT_WHITE "F"

#define FALSE 0
#define TRUE 1

INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

int setDisplayColor(char BGColor, char TextColor) //setDisplayColor(배경색, 글자색)으로 사용한다.
{
	char chTemp[100];
	sprintf(chTemp, "COLOR %c%c", BGColor, TextColor);
	system(chTemp);
}

void setDisplaySize(int width, int height) // setDisplaySize(가로, 세로)로 사용, 콘솔창의 크기를 지정해준다.
{
	char chTemp[100];
	sprintf(chTemp, "mode con cols=%d lines=%d", width, height);
	system(chTemp);
}

void clean() //콘솔창 리셋
{
	system("cls");
}

void FullScreen() //콘솔창 FullScreen
{
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
}

void displayAllColor(int count, int interval)  // displayAllColor(반복 횟수, 색상 바뀌는 사이시간), 표현할 수 있는 16개의 모든 색상을 순차적으로 보여준다.
{
	char chTemp[100];
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			sprintf(chTemp, "COLOR %x7", j);
			system(chTemp);
			Sleep(interval);
			clean();
		}
	}
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show) //커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

