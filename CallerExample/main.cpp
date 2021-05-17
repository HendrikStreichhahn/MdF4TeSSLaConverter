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
BOOL converter(TCHAR* inpPath, TCHAR* outPath, std::vector<SignalDataTypes> types);
BOOL printFileInfo(TCHAR* inpPath);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!initHModule())
		return -1;

	char* inpPath = NULL;
	char* outPath = NULL;

	std::vector<SignalDataTypes> types;

	bool printMF4Info = false;

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
				case 't':
					if (argc > i + 1)
					{
						if (std::string("double").compare(argv[i + 1]) == 0)
						{
							types.push_back(SignalDataTypes::floatingPoint8);
							i++;
							break;
						}
						if (std::string("CAN").compare(argv[i + 1]) == 0)
						{
							types.push_back(SignalDataTypes::CAN_Frames);
							i++;
							break;
						}
						std::cout << "Unknown Datatype " << argv[i + 1] << std::endl;
						return -1;
					}
					i++;
					break;
				case 'd':
					printMF4Info = true;
					break;
			}
	}
	if (inpPath == NULL || outPath == NULL || types.size() == 0)
	{
		std::cout << "Invalid Parameters! Usage: -i inputFile -o outputFile -t [signalType1][SignalType2]... [-d]" << std::endl;
		return -1;
	}
	if (printMF4Info)
		printFileInfo(inpPath);
	else
		converter(inpPath, outPath, types);
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

BOOL converter(TCHAR* inpPath, TCHAR* outPath, std::vector<SignalDataTypes> types)
{
	CMdf4TeSSLaConverter* converter = new CMdf4TeSSLaConverter();
	if (converter->readMdf4File(inpPath, types[0], types.size(), 1000))
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

