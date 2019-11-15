#include "Csv.h"	
#include <fstream>
#include <iostream>

Csv::Csv(const char* filePath, const char* fileName)
{
	nRow = 0;
	nColumn = 0;
	mContent = 0;
	mFilePath = filePath;
	mFileName = fileName;
	mData = 0;
}

Csv::~Csv()
{
	delete[] mData;
	delete mContent;
	mContent = 0;
}

bool Csv::Init()
{
	if (!mFileName || *mFileName == 0)
		return false;

	return LoadCsv(mFilePath, mFileName);
}

bool Csv::LoadData(char* szLine)
{
	if (!szLine || *szLine == 0)
		return false;

	char* szTempLine = szLine;
	if (!CalcRowAndColumn(szTempLine))
		return false;

	int state = 0;//0ÎÞÌØÊâ×Ö·û 1º¬ÓÐÌØÊâ×Ö·û
	int nCurRow = 0;
	int nCurColumn = 0;
	int nCharCnt = 0;
	char* szWord = 0;

	mContent = new char[strlen(szLine)];
	memset(mContent, 0, strlen(szLine));
	szWord = mContent;
	mData = new char* [nRow*nColumn];
	while (*szLine != '\0')
	{
		if (state == 0)
		{
			if (*szLine == '"')
			{
				state = 1;
				szLine++;
			}
			else if (*szLine == 0x0D || *szLine == 0x0A)
			{
				*(szWord + nCharCnt) = 0;
				if (*(szLine + 1) == 0x0D || *(szLine + 1) == 0x0A)
					szLine += 2;
				else
					szLine++;
				nCharCnt++;
				mData[nCurRow * nColumn + nCurColumn] = szWord;
				szWord += nCharCnt;
				nCharCnt = 0;
				nCurColumn = 0;
				nCurRow++;
			}
			else if (*szLine == ',')
			{
				*(szWord + nCharCnt) = 0;
				szLine++;
				nCharCnt++;
				mData[nCurRow * nColumn + nCurColumn] = szWord;
				szWord += nCharCnt;
				nCharCnt = 0;
				nCurColumn++;
			}
			else
			{
				*(szWord + nCharCnt) = *szLine;
				szLine++;
				nCharCnt++;
			}
		}
		else if (state == 1)
		{
			if (*szLine == '"')
			{
				if (*(szLine + 1) == ',')
				{
					*(szWord + nCharCnt) = 0;
					szLine += 2;
					nCharCnt ++;
					mData[nCurRow * nColumn + nCurColumn] = szWord;
					szWord += nCharCnt;

					nCharCnt = 0;
					nCurColumn++;
					state = 0;
				}
				else if (*(szLine + 1) == '"')
				{
					*(szWord + nCharCnt) = *szLine;
					szLine += 2;
					nCharCnt++;
				}
			}
			else
			{
				*(szWord + nCharCnt) = *szLine;
				szLine++;
				nCharCnt++;
			}
		}
	}

	return true;
}

bool Csv::CalcRowAndColumn(char* szLine)
{
	if (!szLine || *szLine == 0)
		return false;

	int state = 0;//0ÎÞÌØÊâ×Ö·û 1º¬ÓÐÌØÊâ×Ö·û
	int nCurColumn = 0;
	while (*szLine != '\0')
	{
		if (state == 0)
		{
			if (*szLine == '"')
			{
				state = 1;
				szLine++;
			}
			else if (*szLine == 0x0D || *szLine == 0x0A)
			{
				if (*(szLine + 1) == 0x0D || *(szLine + 1) == 0x0A)
					szLine += 2;
				else
					szLine++;

				nCurColumn++;
				if (nCurColumn > nColumn)
					nColumn = nCurColumn;
				nCurColumn = 0;
				nRow++;
			}
			else if (*szLine == ',')
			{
				nCurColumn++;
				szLine++;
			}
			else
				szLine++;
		}
		else if (state == 1)
		{
			if (*szLine == '"')
			{
				if (*(szLine + 1) == ',')
				{
					szLine += 2;
					nCurColumn++;
					state = 0;
				}
				else if (*(szLine + 1) == '"')
					szLine += 2;
			}
			else
				szLine++;
		}
	}

	return true;
}

bool Csv::LoadCsv(const char* filePath, const char* fileName)
{
	int nChar = 0;
	char file[256] = { 0 };
	strcat_s(file, 256, filePath);
	strcat_s(file, 256, fileName);
	std::fstream fileStream(file, std::ios::in | std::ios::binary);
	if (!fileStream.is_open())
	{
		printf("OpenFile [%s] Err!", file);
		return false;
	}
	fileStream.seekg(0, fileStream.end);
	nChar = (int)fileStream.tellg();
	fileStream.seekg(0, fileStream.beg);
	char* buff = new char[nChar + 1];
	memset(buff, 0, nChar + 1);
	fileStream.read(buff, nChar);
	LoadData(buff);
	delete[] buff;
	fileStream.close();
	//for (int i = 0; i < nRow; i++)
	//{
	//	for (int j = 0; j < nColumn; j++)
	//	{
	//		if (mData[i * nColumn + j] == NULL)
	//			printf(",");
	//		else
	//			printf("%s,", mData[i * nColumn + j]);
	//	}
	//	printf("\r\n");
	//}

	return true;
}