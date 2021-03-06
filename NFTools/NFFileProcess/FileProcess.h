#pragma once
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "tinyxml2.h"
#include "MiniExcelReader.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <map>


#if NF_PLATFORM == NF_PLATFORM_WIN
#include <io.h>
#include <windows.h>
#include <conio.h>
#else
#include <iconv.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#endif



class FileProcess
{
public:
	FileProcess();
	~FileProcess();

	void CreateProtoFile();
	void CreateNameFile();
	void OnCreateXMLFile();
	void CreateStructThreadFunc();
	void CreateIniThreadFunc();
	bool CreateStructXML(std::string strFile, std::string strFileName);
	bool CreateIniXML(std::string strFile);
	void OnCreateMysqlFile();
	bool LoadLogicClass(std::string strFile);
	bool LoadClass(std::string strFile, std::string strTable);

	bool bConvertIntoUTF8;

private:
	int nCipher;
	std::string strCipherCfg;

	std::string strExecutePath;
	std::string strToolBasePath;
	std::string strRelativePath;
	std::string strExcelStructPath;
	std::string strXMLStructPath;

	std::string strExcelIniPath;
	std::string strXMLIniPath;

	std::string strLogicClassFile;
	std::string strMySQLFile;
	std::string strMySQLClassFile;

	std::string strProtoFile;

	std::string strHPPFile;
	std::string strJavaFile;
	std::string strCSFile;

	FILE* mysqlWriter;
	FILE* mysqlClassWriter;
	FILE* protoWriter;
	FILE* hppWriter;
	FILE* javaWriter;
	FILE* csWriter;

	std::string strHppIObjectInfo;
	std::string strJavaIObjectInfo;
	std::string strCSIObjectInfo;

	int nRecordStart;

	std::vector<std::string> GetFileListInFolder(std::string folderPath, int depth)
	{
		std::vector<std::string> result;
#if NF_PLATFORM == NF_PLATFORM_WIN
		_finddata_t FileInfo;
		std::string strfind = folderPath + "\\*";
		long long Handle = _findfirst(strfind.c_str(), &FileInfo);


		if (Handle == -1L)
		{
			std::cerr << "can not match the folder path" << std::endl;
			exit(-1);
		}
		do {
			
			if (FileInfo.attrib & _A_SUBDIR)
			{
				
				if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
				{
					std::string newPath = folderPath + "\\" + FileInfo.name;
					//dfsFolder(newPath, depth);
				}
			}
			else
			{

				std::string filename = (folderPath + "\\" + FileInfo.name);
				result.push_back(filename);
			}
		} while (_findnext(Handle, &FileInfo) == 0);


		_findclose(Handle);
#else
	DIR *pDir;
	struct dirent *ent;
	char childpath[512];
	char absolutepath[512];
	pDir = opendir(folderPath.c_str());
	memset(childpath, 0, sizeof(childpath));
	while ((ent = readdir(pDir)) != NULL)
	{
		if (ent->d_type & DT_DIR)
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				continue;
			}
		}
		else
		{
			sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);
			result.push_back(absolutepath);
		}
	}

	sort(result.begin(), result.end());
#endif
		return result;
	}

	void StringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
	{
		std::string::size_type pos = 0;
		std::string::size_type srclen = strsrc.size();
		std::string::size_type dstlen = strdst.size();

		while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
		{
			strBig.replace(pos, srclen, strdst);
			pos += dstlen;
		}
	}
};
