/*
				  ___  ___ _____
				  |  \/  ||  _  |
 _   _  ___   ___ | .  . || | | | _   _  ___   ___
| | | |/ __| / _ \| |\/| || | | || | | |/ __| / _ \
| |_| |\__ \|  __/| |  | |\ \_/ /| |_| |\__ \|  __/
 \__,_||___/ \___|\_|  |_/ \___/  \__,_||___/ \___|

 ______  _   _____  ___       ___  _   _  _    _
| ___ \| | / /|  \/  |      |_  || | | || |  | |
| |_/ /| |/ / | .  . |        | || |_| || |  | |
| ___ \|    \ | |\/| |        | ||  _  || |/\| |
| |_/ /| |\  \| |  | | _  /\__/ /| | | |\  /\  /
\____/ \_| \_/\_|  |_/( ) \____/ \_| |_/ \/  \/
					  |/
useMOuse는 마우스만을 사용하여서 게임을 플레이 할 수 있도록 만든 게임입니다.
*/
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

int main() { //main 함수는 여러가지 스테이지들의 관계가 goto문으로 관계되어 있습니다.
	setDisplaySize(180, 49);
	enableMouseInput();
startPage:
	switch(StartStage()){ // 시작 페이지에서 1(게임 시작)을 선택하면 게임 선택화면으로 , 2(게임 설명)를 선택하면 게임 설명 페이지로 갑니다.
		case 1:goto selectGame; break;
		case 2:goto gameExplain; break;
	}
gameExplain:
	switch (GameExplainStage()) { // 게임 설명 페이지에서 1(게임 시작)을 선택하면 다시 시작 화면으로 , 2(게임 선택)를 선택하면 게임 선택 페이지로 갑니다.
		case 1:goto startPage; break;
		case 2:goto selectGame; break;
	}
selectGame:
	switch (SelectGameStage()) { // 게임 선택 페이지에서 0(되돌아 가기)을 선택하면 게임 선택화면으로 , n(각 게임들)를 선택하면 그 게임이 실행됩니다.
		case 0:goto startPage; break;
		case 1:goto avoidObstacle_1p; break;
		case 2:goto avoidObstacle_2p; break;
		case 3:goto avoid_star; break;
		case 4:goto hit_the_mouse; break;
		case 5:goto go_go_sled; break;
	}
avoidObstacle_1p:
	switch (AvoidObstacle_1P()) { // 게임 플레이 화면에서 0을 선택하면 다시 첫 화면으로 갑니다.
		case 0: goto startPage; break;
	}
avoidObstacle_2p:
	return 0;
avoid_star:
	return 0;
hit_the_mouse:
	return 0;
go_go_sled:
	return 0;
}
