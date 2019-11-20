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
#include<stdio.h> // 여러 형의 입출력 함수가 포함된 헤더 파일
#include<stdlib.h> // C 언어의 표준 라이브러리
#include<Windows.h> // 윈도우 API 함수를 제공하는 헤더 파일
#include<conio.h> // 콘솔 입출력 함수를 제공하는 헤더 파일
#include<time.h> // 시간 관련 함수를 제공하는 헤더 파일

#include "ConsoleControl.h" //콘솔 관련 제어 함수 헤더 파일
#include "ImageLayer.h" //1409민승현 이미지 제어 함수 헤더 파일
#include "Stages.h" // 게임 및 화면, 스테이지들이 표현되어 있는 함수 헤더 파일
#include "fileIO.h" // 파일 입출력을 제어하는 함수 헤더 파일
#include "SoundControl.h" // 사운드 출력 관련 제어 함수 헤더 파일


static HWND hWnd;
static HINSTANCE hlnst;

#define MOUSE_NONE 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define SAME_BUTTON 3
#define SCROLLED 4
#define SELECT_BUTTON 5

#define Idx_AvoidObstacle1P 0  // 행성 피하기 1P 게임에 대한 인덱스
#define Idx_AvoidObstacle2P 1 // 행성 피하기 2P 게임에 대한 인덱스
#define Idx_HitTheMouse 2 // 마우스를 괴롭혀라 게임에 대한 인덱스

#define CONSOLE_WIDTH 180
#define CONSOLE_HEIGHT 50 

int main() { // main 함수는 여러가지 스테이지들의 관계가 goto문으로 연결되어 있습니다.
	setDisplaySize(CONSOLE_WIDTH, CONSOLE_HEIGHT); // 콘솔창 크기 설정
	CursorView(FALSE); // 커서 숨기기
	MouseInputActivate(); // 마우스 입력 활성화
startPage: // 시작 화면
	switch(StartStage()){ // 시작 페이지에서 1(게임 시작)을 선택하면 게임 선택화면으로 , 2(게임 설명)를 선택하면 게임 설명 페이지로 갑니다.
		case 1:goto selectGame; break; // 시작 페이지 → 게임 선택 화면
		case 2:goto gameExplain; break; // 시작 페이지 → 게임 설명 화면 
	}
gameExplain: // 게임 설명 화면
	switch (GameExplainStage()) { // 게임 설명 페이지에서 1(게임 시작)을 선택하면 다시 시작 화면으로 , 2(게임 선택)를 선택하면 게임 선택 페이지로 갑니다.
		case 1:goto startPage; break; // 게임 설명 화면 → 시작 페이지
		case 2:goto selectGame; break; // 게임 설명 화면 → 게임 선택 화면
	}
selectGame: // 게임 선택 화면
	switch (SelectGameStage()) { // 게임 선택 페이지에서 0(되돌아 가기)을 선택하면 게임 선택화면으로 , n(각 게임들)를 선택하면 그 게임이 실행됩니다.
		case 0:goto startPage; break; // 게임 선택 화면 → 시작 페이지 
		case 1:goto avoidObstacle_1p; break; // 게임 선택 화면 → 운석을 피해라 1P
		case 2:goto avoidObstacle_2p; break; // 게임 선택 화면 → 운석을 피해라 2P
		case 3:goto hit_the_mouse; break; // 게임 선택 화면 → 마우스를 괴롭혀라
	}
avoidObstacle_1p: // 운석을 피해라 1P
	switch (ShowScoreStage(AvoidObstacle_1P(), Idx_AvoidObstacle1P)) { // ShowScoreStage는 게임에서 return 하는 스코어를 바탕으로 실행됩니다. 
		case 1: goto startPage; break; // 운석을 피해라 1P → 시작 페이지
		case 2: goto avoidObstacle_1p; break; // 운석을 피해라 1P → 운석을 피해라 1P (다시하기)
	}
avoidObstacle_2p: // 운석을 피해라 2P
	switch (ShowScoreStage(AvoidObstacle_2P(), Idx_AvoidObstacle2P)) { // ShowScoreStage는 게임에서 return 하는 스코어를 바탕으로 실행됩니다.
		case 1: goto startPage; break; // 운석을 피해라 2P → 시작 페이지
		case 2: goto avoidObstacle_2p; break; // 운석을 피해라 2P → 운석을 피해라 2P (다시하기)
	}
hit_the_mouse: // 마우스를 괴롭혀라
	switch (ShowScoreStage(HitTheMouseGameStage(),Idx_HitTheMouse)) {// ShowScoreStage는 게임에서 return 하는 스코어를 바탕으로 실행됩니다.
		case 1: goto startPage; break; // 마우스를 괴롭혀라 → 시작 페이지
		case 2: goto hit_the_mouse; break; // 마우스를 괴롭혀라 → 마우스를 괴롭혀라 (다시하기) 
	}
}
