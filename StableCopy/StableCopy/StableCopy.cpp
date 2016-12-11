// StableCopy.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

#ifndef UNICODE
#define UNICODE
#endif
using std::cerr;
using std::endl;
using std::vector;

class CFile;
class CFileList;
class CGetFileInDir;
class CFileOperation;

class ErrorHandle;

class CFile
{
public:
private:
	LPCTSTR lpFileName;
	HANDLE hFile;
};

/*class CFileList
{
public:
	bool SetFindData(LPCTSTR lpFileName);
	vector <WIN32_FIND_DATA> GetFindData() { return FindData; }
private:
	vector <WIN32_FIND_DATA> FindData;
};*/

class ErrorHandle
{
public:
	ErrorHandle();
	~ErrorHandle();

	void Error_Log(TCHAR * Text);
	
private:
	HANDLE hLog;
};

ErrorHandle::ErrorHandle()
{
	hLog = CreateFile(_T("log.txt"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}
ErrorHandle::~ErrorHandle()
{
	CloseHandle(hLog);
}
void ErrorHandle::Error_Log(TCHAR * Text)
{
	WriteFile(hLog, Text, _tcslen(Text), NULL, NULL);
}

std::wfstream Err("Log.txt", std::ios_base::out);

FILE * fp;
void ReportError();
bool GetFileListInDir(LPCTSTR lpFileName, vector <WIN32_FIND_DATA> &FindData);
void StableCopy();


int _tmain(int argc, TCHAR * argv[])
{
	fopen_s(&fp, "LLog.cpp", "w+");

	TCHAR input_path[256] = { '\0' }, output_path[256] = { '\0' }
	, tmp_input[256] = { '\0' }, tmp_output[256] = { '\0' };
	vector <WIN32_FIND_DATA> FindData;
	
	_tscanf_s(_T("%ls"), input_path, sizeof(input_path) / sizeof(TCHAR));
	_tscanf_s(_T("%ls"), output_path, sizeof(output_path) / sizeof(TCHAR));

	_tcscpy_s(tmp_input, input_path);
	_tcscat_s(tmp_input, _T("*"));
	GetFileListInDir(tmp_input, FindData);

	// Copy
	for (auto v : FindData)
	{
		_tcscpy_s(tmp_input, input_path);
		_tcscat_s(tmp_input, v.cFileName);

		_tcscpy_s(tmp_output, output_path);
		_tcscat_s(tmp_output, v.cFileName);

		if (CopyFile(tmp_input, tmp_output, 0) == 0)
		{
			Err << tmp_input << " copy failed." << std::endl;
			ReportError();
		}
	}

	Err.close();
    return 0;
}

bool GetFileListInDir(LPCTSTR lpFileName, vector <WIN32_FIND_DATA> &FindData)
{
	WIN32_FIND_DATA lpFD;
	HANDLE hFile = FindFirstFile(lpFileName, &lpFD);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_ftprintf(stderr, _T("\"%s\" file open failed."), lpFileName);
		return false;
	}

	do
	{
		FindData.push_back(lpFD);
		_ftprintf(fp, _T("%ls\n"), lpFD.cFileName);
		Err << "file" << lpFD.cFileName << std::endl;
	} while (FindNextFile(hFile, &lpFD));

	return true;
}

void ReportError()
{
	DWORD errMesLen = 0, errNum = GetLastError();
	LPTSTR lpErrMesBuffer = NULL;

	errMesLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errNum,
		0,
		lpErrMesBuffer,
		0,
		NULL
	);

	if (errMesLen > 0)
		_ftprintf(stderr, _T("%s\n"), lpErrMesBuffer);
	else
		_ftprintf(stderr, _T("Error Number: %d.\n"), errNum);
	if (lpErrMesBuffer != NULL)
		LocalFree(lpErrMesBuffer);

}

void StableCopy()
{
}

//bool CFileList::SetFindData(LPCTSTR lpFileName)
//{
//	WIN32_FIND_DATA lpFD;
//	HANDLE hFile = FindFirstFile(lpFileName, &lpFD);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		cerr << "file open failed." << endl;
//		return false;
//	}
//
//	do
//	{
//		FindData.push_back(lpFD);
//		_tprintf(TEXT("%s", lpFD.cFileName));
//	} while (FindNextFile(hFile, &lpFD));
//	
//	return true;
//}


