#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#include "ConsoleControl.h"
#include "ImageLayer.h"
#include "MouseInput.h"
#include "Stages.h"

static HWND hWnd;
static HINSTANCE hlnst;

#define MOUSE_NONE 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define SAME_BUTTON 3
#define SCROLLED 4
#define SELECT_BUTTON 5

int main() {
	setDisplaySize(180, 49);
	enableMouseInput();
startPage:
	switch(StartStage()){
		case 1:goto selectGame; break;
		case 2:goto gameExplain; break;
	}
gameExplain:
	switch (GameExplainStage()) {
		case 1:goto startPage; break;
		case 2:goto selectGame; break;
	}
selectGame:
	switch (SelectGameStage()) {
		case 0:goto startPage; break;
		case 1:goto avoidObstacle_solo; break;
	}
avoidObstacle_solo:
	switch (AvoidObstacle_SOLO()) {
		case 0: goto startPage; break;
	}
}
