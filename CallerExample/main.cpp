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



BOOL initHModule();
BOOL converter(TCHAR* inpPath, TCHAR* outPath);
BOOL converterCAN(TCHAR* inpPath, TCHAR* outPath);
BOOL printFileInfo(TCHAR* inpPath);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!initHModule())
		return -1;

	char* inpPath = NULL;
	char* outPath = NULL;

	bool printMF4Info = false;
	bool readAsCAN = false;

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
				case 'C':
					readAsCAN = true;
					break;
				case 'd':
					printMF4Info = true;
					break;
			}
	}
	if (inpPath == NULL || outPath == NULL)
	{
		std::cout << "Invalid Parameters! Usage: -i inputFile -o outputFile [-C] [-d]" << std::endl;
		return -1;
	}
	if (printMF4Info)
		printFileInfo(inpPath);
	else
		if (readAsCAN)
			converterCAN(inpPath, outPath);
		else
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

BOOL printFileInfo(TCHAR* inpPath)
{
	CMdf4TeSSLaConverter* converter = new CMdf4TeSSLaConverter();
	return converter->printMdf4FileInfo(inpPath);
}

BOOL converter(TCHAR* inpPath, TCHAR* outPath)
{
	CMdf4TeSSLaConverter* converter = new CMdf4TeSSLaConverter();
	if (converter->readMdf4File(inpPath, 1000))
		std::cout << "read file Successfully!" << std::endl;
	else
	{
		std::cout << "converter->readMdf4File returned false!" << std::endl;
		return false;
	}
	if (!converter->exportTeSSLaFile(outPath))
	{
		std::cout << "Cannot export to " << outPath << std::endl;
		return false;
	}
	return true;
}

BOOL converterCAN(TCHAR* inpPath, TCHAR* outPath)
{
	CMdf4TeSSLaConverter* converter = new CMdf4TeSSLaConverter();
	if (converter->readMdf4FileCAN(inpPath, 1000))
		std::cout << "read file Successfully!" << std::endl;
	else
	{
		std::cout << "converter->readMdf4File returned false!" << std::endl;
		return false;
	}
	if (!converter->exportTeSSLaFile(outPath))
	{
		std::cout << "Cannot export to " << outPath << std::endl;
		return false;
	}
	return true;
}
