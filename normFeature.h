#pragma once
#ifndef _NORMFEATURE_H_
#define _NORMFEATURE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include<string.h>
#include<math.h>
#include"fileIO.h"
#include"hmath.h"
#include"sigProcess.h"
#define maxFileNameLength 500

/*Calculate over a matrix. Each row in matrix represents a frame in audio,each column is a feature dim*/
/*The sumtemp matrix is for storing sum and square sum by suming over frames*/
void SingleMatCal(Matrix m, Matrix sumtemp);

/*Global norm the fileList*/
void GlobalNorm(FILE* fileList, const char* dir1, const char* dir2,int regressOrder,int delwin);

/*Not finished*/
void OnlineNorm();

/*The function to norm in a binary file, that is, norm is offline and limited in a single audio*/
void OfflineNorm0(BinaryFileStruct* bfP, int regressOrder, int delwin);

/*pack OfflineNorm0*/
void OfflineNorm(FILE* fileList, const char* dir1, const char* dir2, int regressOrder, int delwin);

/*Only do regression, no cmvn*/
void RegressBinaryFile(FILE * fileList, const char * dir1, const char * dir2, int regressOrder, int delwin);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_SIGPROCESS_H_