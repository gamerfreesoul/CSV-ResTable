#pragma once

class Csv
{
public:
	Csv(const char* filePath, const char* fileName);
	~Csv();

	bool Init();
private:
	bool LoadCsv(const char* filePath, const char* fileName);
	bool LoadData(char* szLine);
	bool CalcRowAndColumn(char* szLine);

	char* mContent;
public:
	int nRow;
	int nColumn;
	char** mData;
	const char* mFilePath;
	const char* mFileName;
};
