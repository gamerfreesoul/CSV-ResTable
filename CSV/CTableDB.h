#pragma once
#include "string.h"
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

typedef std::map<std::string, const char*> SLine;
typedef std::vector<const char*> vecKey;
template <typename T>
class CResTable
{
public:
	CResTable();
	~CResTable();

	virtual bool LoadLine(SLine line, T& data) = 0;
	bool LoadTableBy_StringId(std::string filePath, std::string file);
	bool LoadTableBy_Id(std::string filePath, std::string file);
	T* Find(int nId);
	T* Find(std::string strId);
public:
	std::map<int, T> m_dataId;
	std::map<std::string, T> m_dataStr;
	SLine Line;
};


template <typename T>
CResTable<T>::CResTable()
{
}

template <typename T>
CResTable<T>::~CResTable()
{
	m_dataId.clear();
	m_dataStr.clear();
	Line.clear();
}

template <typename T>
bool CResTable<T>::LoadTableBy_StringId(std::string filePath, std::string fileName)
{
	Csv csv(filePath, fileName);
	if (!csv.Init())
		return false;
	
	// first line is Des
	// second line is Key
	// info......
	vecKey strkey;
	if (strcmp(csv.mData[csv.nColumn], "STRID") == 0)
	{
		for (int i = 0; i < csv.nColumn; i++)
			strkey.push_back(csv.mData[i]);
	}
	else
		return false;

	std::string key;
	for (int i = 2; i < csv.nRow; i++)
	{
		Line.clear();
		for (int j = 0; j < csv.nColumn; j++)
		{
			if (j == 0)
				key = csv.mData[i * csv.nColumn + j];
			Line.insert(std::make_pair(strkey[j], csv.mData[i * csv.nColumn + j]));
		}
		T data;
		if (!LoadLine(Line, data))
			return false;

		m_dataStr.insert(std::make_pair(key, data));
	}
	Line.clear();

	return true;
}

template <typename T>
bool CResTable<T>::LoadTableBy_Id(std::string filePath, std::string file)
{
	Csv csv(filePath.c_str(), file.c_str());
	if (!csv.Init())
		return false;

	// first line is Des
	// second line is Key
	// info......
	vecKey strkey;
	if (strcmp(csv.mData[csv.nColumn], "ID") == 0)
	{
		for (int i = 0; i < csv.nColumn; i++)
			strkey.push_back(csv.mData[csv.nColumn+i]);
	}
	else
		return false;

	int key;
	for (int i = 2; i < csv.nRow; i++)
	{
		Line.clear();
		for (int j = 0; j < csv.nColumn; j++)
		{
			if (j == 0)
				key = atoi(csv.mData[i * csv.nColumn + j]);
			Line.insert(std::make_pair(strkey[j], csv.mData[i * csv.nColumn + j]));
		}
		T data;
		if (!LoadLine(Line, data))
			return false;

		m_dataId.insert(std::make_pair(key, data));
	}
	Line.clear();

	return true;
}

template<typename T>
T* CResTable<T>::Find(int nId)
{
	if (!m_dataId.find(nId))
		return false;

	return &m_dataID[nId];
}

template<typename T>
T* CResTable<T>::Find(std::string strId)
{
	if (!m_dataStr.find(strId))
		return false;

	return m_dataStr[strId];
}

