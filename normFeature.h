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

void SingleMatCal(Matrix m, Matrix sumtemp);

void GlobalNorm(FILE* fileList, const char* dir1, const char* dir2,int regressOrder,int delwin);

void OnlineNorm();

void OfflineNorm0(BinaryFileStruct* bfP, int regressOrder, int delwin);

void OfflineNorm(FILE* fileList, const char* dir1, const char* dir2, int regressOrder, int delwin);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_SIGPROCESS_H_