#pragma once
#include"stdio.h"
#include"stdint.h"

void GetFileSize(int* puFileSize, FILE* pFile)
{
	int	 nRetCode = 0;
	long	nOrgPos = 0;
	int64_t nFileLen = 0;

	nOrgPos = ftell(pFile);
	nRetCode = fseek(pFile, 0, SEEK_END);
	nFileLen = ftell(pFile);
	nRetCode = fseek(pFile, nOrgPos, SEEK_SET);

	*puFileSize = (int)nFileLen;
}


unsigned char* LoadFile(const char* pszFileName, int* pFileSize)
{
	FILE* pFile;
	unsigned char* pFileBuffer;

	pFile = fopen(pszFileName, "rb");
	GetFileSize(pFileSize, pFile);
	pFileBuffer = new unsigned char[*pFileSize];
	fread(pFileBuffer, 1, *pFileSize, pFile);
	fclose(pFile);

	return pFileBuffer;
}