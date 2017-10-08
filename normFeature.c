#include "normFeature.h"

void SingleMatCal(Matrix m,Matrix sumtemp)
{
	int numFrames = NumRows(m);
	int numDims = NumCols(m);
	int i = 0, j = 0;

	if (NumCols(m) != NumCols(sumtemp)) { printf("dims not match in SingleMatCal\n"); exit(-1); }
	for (i = 1; i <= numFrames; i++) {
		for (j = 1; j <= numDims; j++) {
			sumtemp[1][j] += m[i][j];
			sumtemp[2][j] += m[i][j] * m[i][j];
		}
	}
}

void GlobalNorm(FILE* fileList, const char* dir1, const char* dir2,int regressOrder,int delwin)
{
	char fileLineBuf[maxFileNameLength];
	char filePathBuf[maxFileNameLength];
	char* fileName1 = NULL, *fileName2 = NULL;
	char deli[2] = { '\\','\0' };
	FILE* fbin = NULL; BinaryFileStruct bFile = { 0,0,0,0,NULL };
	Matrix meanAndVar = NULL;
	int numRowsInTotal = 0; int i = 0, j = 0; int numDims = 0;
	FILE* fmeanAndVar = fopen("meanAndVar.dat", "w");

	printf("Step 1. Calculate the global mean and variance\n\n");
	while (fgets(fileLineBuf, maxFileNameLength, fileList)) {
		//	printf("%s", fileLineBuf);
		if (fileLineBuf[0] == '\n')break;
		fileName1 = strtok(fileLineBuf, "\t ");
		fileName2 = strtok(NULL, "\n\t ");
		strcpy(filePathBuf, dir1);
		strcat(filePathBuf, deli); strcat(filePathBuf, fileName1);

		printf("%s\n", filePathBuf);
		fbin = fopen(filePathBuf, "rb");
		bFile = readBinaryFile(fbin);
		fclose(fbin);

		if (meanAndVar == NULL) {
			meanAndVar = CreateMatrix(2, NumCols(bFile.data)*(1+regressOrder)); 
			ZeroMatrix(meanAndVar);
			numDims = NumCols(bFile.data)*(1 + regressOrder);
		}

		RegressMat(&(bFile.data), delwin, regressOrder);
		SingleMatCal(bFile.data, meanAndVar);
		numRowsInTotal += NumRows(bFile.data);

		FreeBinaryFileStruct(&bFile);
	}

	for (i = 1; i <= numDims; i++) {
		meanAndVar[1][i] /= numRowsInTotal;
		meanAndVar[2][i] = sqrt(meanAndVar[2][i] / (double)numRowsInTotal - pow(meanAndVar[1][i], 2.0));
	}
	WriteMatrix(fmeanAndVar, meanAndVar);
	fclose(fmeanAndVar);


	printf("Step 2. Transform the target files\n\n");
	printf("<INPUT FILE\tOUTPUT FILE>\n\n");
	fseek(fileList, 0, SEEK_SET);
	while (fgets(fileLineBuf, maxFileNameLength, fileList)) {
		//	printf("%s", fileLineBuf);
		if (fileLineBuf[0] == '\n')break;
		fileName1 = strtok(fileLineBuf, "\t ");
		fileName2 = strtok(NULL, "\n\t ");
		strcpy(filePathBuf, dir1);
		strcat(filePathBuf, deli); strcat(filePathBuf, fileName1);

		printf("%s\t", filePathBuf);
		fbin = fopen(filePathBuf, "rb");
		bFile = readBinaryFile(fbin);
		fclose(fbin);

		RegressMat(&(bFile.data), delwin, regressOrder);
		bFile.sizeFrameInByte *= (regressOrder + 1);

		for (i = 1; i <= NumRows(bFile.data); i++)
			for (j = 1; j <= NumCols(bFile.data); j++)
				bFile.data[i][j] = (bFile.data[i][j] - meanAndVar[1][j]) / meanAndVar[2][j];
		strcpy(filePathBuf, dir2); strcat(filePathBuf, deli); strcat(filePathBuf, fileName2);
		printf("%s\n", filePathBuf);
		fbin = fopen(filePathBuf, "wb");
		writeBinaryFile(fbin, bFile);
		fclose(fbin);

		FreeBinaryFileStruct(&bFile);
	}

	FreeMatrix(meanAndVar);
}

void OfflineNorm0(BinaryFileStruct* bfP, int regressOrder, int delwin)
{
	Matrix m = bfP->data;
	int numFrames = NumRows(m);
	int numDims = NumCols(m)*(1+regressOrder);
	int i = 0, j = 0;
	Matrix meanAndVar = CreateMatrix(2, numDims);

	ZeroMatrix(meanAndVar);
	/*regress matrix in each frame*/
	RegressMat(&m, delwin, regressOrder);
	SingleMatCal(m, meanAndVar);
	
	bfP->sizeFrameInByte *= (regressOrder + 1);
	bfP->data = m;
	//calculate the mean and variance
	for (i = 1; i <= numDims; i++) {
		meanAndVar[1][i] /= numFrames;
		meanAndVar[2][i] = sqrt(meanAndVar[2][i] / (double)numFrames - pow(meanAndVar[1][i], 2.0));
	}
	//transform the original signal
	for (i = 1; i <= numFrames; i++)
		for (j = 1; j <= numDims; j++)
			m[i][j] = (m[i][j] - meanAndVar[1][j]) / meanAndVar[2][j];
//	printf("%d\t%d\n", NumRows(m), NumCols(m));
	FreeMatrix(meanAndVar);
}

void OfflineNorm(FILE * fileList, const char * dir1, const char * dir2, int regressOrder, int delwin)
{
	char fileLineBuf[maxFileNameLength];
	char filePathBuf[maxFileNameLength];
	char* fileName1 = NULL, *fileName2 = NULL;
	char deli[2] = { '\\','\0' };
	FILE* fbin = NULL; BinaryFileStruct bFile = { 0,0,0,0,NULL };

	printf("<INPUT FILE\tOUTPUT FILE>\n\n");
	while (fgets(fileLineBuf, maxFileNameLength, fileList)) {
	//	printf("%s", fileLineBuf);
		if (fileLineBuf[0] == '\n')break;
		fileName1 = strtok(fileLineBuf, "\t ");
		fileName2 = strtok(NULL, "\n\t ");
		strcpy(filePathBuf, dir1);
		strcat(filePathBuf, deli); strcat(filePathBuf, fileName1);

		printf("%s\t", filePathBuf);
		fbin = fopen(filePathBuf, "rb");
		bFile = readBinaryFile(fbin);
//		printf("%d\t%d\n", NumCols(bFile.data), NumRows(bFile.data));
		fclose(fbin);

		OfflineNorm0(&bFile,regressOrder,delwin);
//		printf("%d\t%d\n", NumCols(bFile.data), NumRows(bFile.data));
		strcpy(filePathBuf, dir2); strcat(filePathBuf, deli); strcat(filePathBuf, fileName2);
		printf("%s\n", filePathBuf);
		fbin = fopen(filePathBuf, "wb");
		writeBinaryFile(fbin, bFile);
		fclose(fbin);

		FreeBinaryFileStruct(&bFile);
	}
}


void RegressBinaryFile(FILE * fileList, const char * dir1, const char * dir2, int regressOrder, int delwin)
{
	char fileLineBuf[maxFileNameLength];
	char filePathBuf[maxFileNameLength];
	char* fileName1 = NULL, *fileName2 = NULL;
	char deli[2] = { '\\','\0' };
	FILE* fbin = NULL; BinaryFileStruct bFile = { 0,0,0,0,NULL };
	Matrix m = NULL;

	printf("<INPUT FILE\tOUTPUT FILE>\n\n");
	while (fgets(fileLineBuf, maxFileNameLength, fileList)) {
		//	printf("%s", fileLineBuf);
		if (fileLineBuf[0] == '\n')break;
		fileName1 = strtok(fileLineBuf, "\t ");
		fileName2 = strtok(NULL, "\n\t ");
		strcpy(filePathBuf, dir1);
		strcat(filePathBuf, deli); strcat(filePathBuf, fileName1);

		printf("%s\t", filePathBuf);
		fbin = fopen(filePathBuf, "rb");
		bFile = readBinaryFile(fbin);
		//		printf("%d\t%d\n", NumCols(bFile.data), NumRows(bFile.data));
		fclose(fbin);

		m = bFile.data;
		RegressMat(&m, delwin, regressOrder);
		bFile.sizeFrameInByte*= (regressOrder + 1);
		bFile.data = m;

		//		printf("%d\t%d\n", NumCols(bFile.data), NumRows(bFile.data));
		strcpy(filePathBuf, dir2); strcat(filePathBuf, deli); strcat(filePathBuf, fileName2);
		printf("%s\n", filePathBuf);
		fbin = fopen(filePathBuf, "wb");
		writeBinaryFile(fbin, bFile);
		fclose(fbin);

		FreeBinaryFileStruct(&bFile);
	}
}