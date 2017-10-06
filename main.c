#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hmath.h"
#include"sigProcess.h"
#include"fileIO.h"
#include"normFeature.h"

#define regressWin 2

int main(int argc, char** argv) {
	printf("The programs calculates CMVN in different modes. It is a project for ASR\n");

	FILE* fileList = fopen(argv[3],"r");
	OfflineNorm(fileList, argv[1], argv[2],atoi(argv[4]),regressWin);
	fclose(fileList);

	fileList = fopen(argv[3], "r");
	GlobalNorm(fileList, argv[1], argv[2],atoi(argv[4]),regressWin);
	fclose(fileList);

	system("pause");
	return 0;
}