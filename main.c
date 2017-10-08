#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hmath.h"
#include"sigProcess.h"
#include"fileIO.h"
#include"normFeature.h"

#define regressWin 2

int main(int argc, char** argv) {
	FILE* fileList = NULL;
	int diffOrder = -1; 
	int mode = -1;

	printf("The programs calculates CMVN in different modes. It is a project for ASR\n\n");
	if (argc != 6 ) {
		printf("%d\n", argc);
		printf("ERROR INPUT!!!\n");
		printf("Usage : CMVN.exe\t<input dir>\t<output dir>\t<fileList>\t<Orders of delta>\t<MODE>\n\n");
		printf("For exmaple,open CMD in the dir containing .exe file and executive\n");
		printf("\tCMVN.exe\tFeatureDim15\tnew\tfileList.txt\t3\t2\n");
		system("pause");
		return 1;
	}

	diffOrder = atoi(argv[4]);
	mode =  atoi(argv[5]);
	if (mode > 2 || mode < 0 || diffOrder < 0) {
		printf("ERROR INPUT!!!\n");
		printf("<Orders of delta> >=0 \n");
		printf("<MODE> >=0 and <=2\n");
		system("pause");
		return 1;
	}

	{
		printf("Config : \n");
		printf("<input dir>\t%s\n",argv[1]);
		printf("<output dir>\t%s\n", argv[2]);
		printf("<fileList>\t%s\n", argv[3]);
		printf("<regress order>\t%d\n", diffOrder);
	}

	if (mode == 0) {
		printf("<MODE>\tRegress and no CMVN\n\n");
		fileList = fopen(argv[3], "r");
		RegressBinaryFile(fileList, argv[1], argv[2], diffOrder, regressWin);
		fclose(fileList);
		system("pause");
		return 0;
	}

	if (mode == 1) {
		printf("<MODE>\tOffline Norm\n\n");
		fileList = fopen(argv[3], "r");
		OfflineNorm(fileList, argv[1], argv[2], diffOrder, regressWin);
		fclose(fileList);
		system("pause");
		return 0;
	}

	if (mode == 2) {
		printf("<MODE>\tGlobal Norm\n\n");
		fileList = fopen(argv[3], "r");
		GlobalNorm(fileList, argv[1], argv[2], diffOrder, regressWin);
		fclose(fileList);
		system("pause");
		return 0;
	}
}