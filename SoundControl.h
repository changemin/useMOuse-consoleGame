/*
SoundControl.h는 음악 재생 관련 라이브러리 이다. 
*/
#include "Mmsystem.h"
#include "Digitalv.h"

#pragma comment(lib, "winmm.lib")

#define PLAY_MUSIC 1
#define STOP_MUSIC 0
#define PAUSE_MUSIC -1
#define RESUME_MUSIC 2

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int PlayMusic(char* FileName, int action) //파일 이름과 action(play, pause, stop, resume)을 지정해준다. 
{
	char FILENAME[100];
	sprintf(FILENAME, FileName);
	
	int dwID;

	mciOpen.lpstrElementName = FILENAME; // 파일 경로 입력
	mciOpen.lpstrDeviceType = "mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)& mciOpen);

	dwID = mciOpen.wDeviceID;
	switch (action) {
		case -1:mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)& m_mciPlayParms); break;
		case 0:mciSendCommandW(dwID, MCI_CLOSE, 0, NULL); break;
		case 1:mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms); break;
		case 2:mciSendCommandW(dwID, MCI_RESUME, 0, NULL);
	}

}

MCI_OPEN_PARMS mciOpenParms;
MCI_PLAY_PARMS mciPlayParms;
DWORD dwDeviceID;
MCI_OPEN_PARMS MciOpen;
MCI_PLAY_PARMS MciPlay;

int PlayNewMusic(char* FileName) // 파일 이름으로 효과음을 출력한다. 
{
	
	char FILENAME[100];
	sprintf(FILENAME, FileName);

	int dwID;

	mciOpen.lpstrElementName = FILENAME; // 파일 경로 입력
	mciOpen.lpstrDeviceType = "mpegvideo";

	dwID = MciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& mciPlayParms);
}