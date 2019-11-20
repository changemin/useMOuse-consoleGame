/*
fileIO.h는 게임 데이터를 외부로 부터 불러오고, 새로운 정보를 기록할 때 사용된다. 
*/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

int ReadBestScore(int GameIndex) { //게임 인덱스를 바탕으로 저장된 최고 기록을 불러 온다. 
	FILE* fp;
	int bestScore[3];

	fp = fopen("GameData.txt", "rt");
	if (fp == NULL) {
		printf("DATA 불러오기 실패");
		return 0;
	}
	fscanf(fp, "%d %d %d", &bestScore[0], &bestScore[1], &bestScore[2]);
	fclose(fp);
	return bestScore[GameIndex];
}

int RecordIfBestScore(int GameIndex, int BestScore) { // 현재 최고 기록을 불러오고, 최근의 한 게임의 점수가 더 높으면 최고기록으로 기록한다. 
	int BestScores[3];
	for (int i = 0; i < 3; i++) {
		BestScores[i] = ReadBestScore(i);
	}
	if (BestScores[GameIndex] <= BestScore)
		BestScores[GameIndex] = BestScore;

	FILE* fp;

	fp = fopen("GameData.txt", "w");
	if (fp == NULL) {
		printf("DATA 불러오기 실패");
		return 0;
	}
	fprintf(fp, "%d %d %d", BestScores[0], BestScores[1], BestScores[2]);
	fclose(fp);
}