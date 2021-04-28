// main.cpp : Defines the entry point for the console application.
//
#pragma once

#include "stdafx.h"
#include "Caller.h"
#include "CMdf4Reader.h"
#include "CMDF4ReaderLib.h"
#include <io.h>
#include <atlsafe.h>
//#include <vld.h>


#include <afxwin.h>
#include <iostream>

#include "CMdf4TeSSLaConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

//CWinApp theApp;

BOOL initHModule();
BOOL converter(TCHAR* inpPath, TCHAR* outPath);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!initHModule())
		return -1;

	char* inpPath = NULL;
	char* outPath = NULL;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
			switch (argv[i][1])
			{
				case 'i':
					if (argc > i+1)
						inpPath = argv[i + 1];
					i++;
					break;
				case 'o':
					if (argc > i + 1)
						outPath = argv[i + 1];
					i++;
					break;
			}
	}
	if (inpPath == NULL || outPath == NULL)
	{
		std::cout << "Invalid Parameters! Usage: -i inputFile -o outputFile" << std::endl;
		return 0;
	}

	converter(inpPath, outPath);
	return 0;
}

BOOL initHModule() {
	//init afx/mfc module
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule == NULL)
	{
		return false;
	}
	if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
	{
		perror("Could not init Module Handle!\n");
		return false;
	}
	return true;
}

BOOL converter(TCHAR* inpPath, TCHAR* outPath) 
{
	CMdf4TeSSLaConverter* converter = new CMdf4TeSSLaConverter();
	if (converter->readMdf4File(inpPath))
		std::cout << "read file Successfully!" << std::endl;
	else
	{
		std::cout << "converter->readMdf4File returned false!" << std::endl;
		return false;
	}
	if (!converter->exportTeSSLaFile(outPath))
	{
		std::cout << "Cannot export to " << outPath << std::endl;
	}
	return true;
}

