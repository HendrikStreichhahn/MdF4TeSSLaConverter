#pragma once

#include "stdafx.h"
#include "CMdf4TeSSLaConverter.h"
#include "CMdf4Reader.h"
#include "CTeSSLaCANStream.h"

#include <io.h>
#include <atlsafe.h>
//#include <vld.h>


#include <afxwin.h>

CMdf4TeSSLaConverter::~CMdf4TeSSLaConverter()
{
	if (mTrace != NULL)
		delete mTrace;
}

bool CMdf4TeSSLaConverter::readMdf4File(std::string strPathToFile, long lTimeFactor)
{
	
	// find and init DLL
	TCHAR szDLLPath[_MAX_PATH];
	if (!FindCOMLib(szDLLPath, TRUE))
	{
		perror("DLL not found!\n");
		return false;
	}
	CMDF4ReaderLib* mdf4Reader = new CMDF4ReaderLib(szDLLPath);
	if (!mdf4Reader->IsValid())
	{
		perror("DLL not loaded!\n");
		return false;
	}
	mdf4Reader->InitDll();
	//load the file
	if (!mdf4Reader->OpenMDF4(_bstr_t(strPathToFile.c_str())))
	{
		perror("Could not open MDF4 File!");
		return false;
	}

	long fileVersion = mdf4Reader->GetVersion();

	if (mTrace != NULL)
		delete mTrace;
	//init tessla trace
	mTrace = new CTeSSLaTrace();

	// get number of groups
	long nGroups = mdf4Reader->GetNGroups();
	if (nGroups == 0)
	{
		perror("No data groups found!");
		return false;
	}
	for (long iGrp = 0; iGrp < nGroups; iGrp++)
	{
		CString buff;
		buff= mdf4Reader->GetGroupName(iGrp);
		//activate current group
		if (!mdf4Reader->LoadGroup(iGrp))
		{
			perror("Error while reading data groups!\n");
			return false;
		}
		//get signal and sample count
		long nSignals = mdf4Reader->GetNSignals();

		unsigned __int64 nSamples = mdf4Reader->GetNSamples();
		//Time Range
		mdf4Reader->LoadTimeSignal();
		double xmin = mdf4Reader->GetFirstTimestamp();
		double xmax = mdf4Reader->GetLastTimestamp();
		//get time signal
		LONG bIsVirtual = FALSE, mon;
		double rmin, rmax, raster;
		wchar_t szTextBuffer[256];
		*szTextBuffer = 0;
		mdf4Reader->GetTimeSignal(szTextBuffer, &mon, &rmin, &rmax, &raster, &bIsVirtual);
		//get indices of time range
		__int64 idx1, idx2;
		idx1 = mdf4Reader->TimeToIndex(xmin, 0);
		idx2 = mdf4Reader->TimeToIndex(xmax, idx1);
		// calc the number of values
		long nValues = idx2 - idx1 + 1; // Calc number of data points
		std::cout << "No. of Values: " << nValues << std::endl;
		// go through signals
		for (long iSig = 0; iSig < nSignals; iSig++)
		{

			long lDataType = 0;
			long nFirstBit = 0;
			long nBits = 0;
			double Factor = 0.0;
			double Offset = 0.0;
			double RawMin = 0.0;
			double RawMax = 0.0;
			long bHasNoValues = 0;
			long invalPos = 0;
			mdf4Reader->GetSignalDetail(&lDataType, &nFirstBit, &nBits, &Factor, &Offset, &RawMin, &RawMax, &bHasNoValues, &invalPos);

			std::cout << "lDataType: " << lDataType << std::endl;
			std::cout << "nFirstBit: " << nFirstBit << std::endl;
			std::cout << "nBits: " << nBits << std::endl;
			std::cout << "Factor: " << Factor << std::endl;
			std::cout << "Offset: " << Offset << std::endl;
			std::cout << "RawMin: " << RawMin << std::endl;
			std::cout << "RawMax: " << RawMax << std::endl;
			std::cout << "bHasNoValues: " << bHasNoValues << std::endl;
			std::cout << "invalPos: " << invalPos << std::endl;

			readSignalAuto(mdf4Reader, mTrace, iSig, nValues, idx1, idx2, lTimeFactor);

		}
	}
	// Release the DLL object and free memory
	mdf4Reader->ExitDll();
	delete mdf4Reader;
	return true;
}

bool CMdf4TeSSLaConverter::readMdf4FileCAN(std::string strPathToFile, long lTimeFactor)
{

	// find and init DLL
	TCHAR szDLLPath[_MAX_PATH];
	if (!FindCOMLib(szDLLPath, TRUE))
	{
		perror("DLL not found!\n");
		return false;
	}
	CMDF4ReaderLib* mdf4Reader = new CMDF4ReaderLib(szDLLPath);
	if (!mdf4Reader->IsValid())
	{
		perror("DLL not loaded!\n");
		return false;
	}
	mdf4Reader->InitDll();
	//load the file
	if (!mdf4Reader->OpenMDF4(_bstr_t(strPathToFile.c_str())))
	{
		perror("Could not open MDF4 File!");
		return false;
	}

	long fileVersion = mdf4Reader->GetVersion();

	if (mTrace != NULL)
		delete mTrace;
	//init tessla trace
	mTrace = new CTeSSLaTrace();

	// get number of groups
	long nGroups = mdf4Reader->GetNGroups();
	if (nGroups == 0)
	{
		perror("No data groups found!");
		return false;
	}
	int dataTypeCounter = 0;
	for (long iGrp = 0; iGrp < nGroups; iGrp++)
	{
		CString buff;
		buff = mdf4Reader->GetGroupName(iGrp);
		//activate current group
		if (!mdf4Reader->LoadGroup(iGrp))
		{
			perror("Error while reading data groups!\n");
			return false;
		}
		//get signal and sample count
		long nSignals = mdf4Reader->GetNSignals();

		unsigned __int64 nSamples = mdf4Reader->GetNSamples();
		//Time Range
		mdf4Reader->LoadTimeSignal();
		double xmin = mdf4Reader->GetFirstTimestamp();
		double xmax = mdf4Reader->GetLastTimestamp();
		//get time signal
		LONG bIsVirtual = FALSE, mon;
		double rmin, rmax, raster;
		wchar_t szTextBuffer[256];
		*szTextBuffer = 0;
		mdf4Reader->GetTimeSignal(szTextBuffer, &mon, &rmin, &rmax, &raster, &bIsVirtual);
		//get indices of time range
		__int64 idx1, idx2;
		idx1 = mdf4Reader->TimeToIndex(xmin, 0);
		idx2 = mdf4Reader->TimeToIndex(xmax, idx1);
		// calc the number of values
		long nValues = idx2 - idx1 + 1; // Calc number of data points
		std::cout << "No. of Values: " << nValues << std::endl;
		// go through signals
		for (long iSig = 0; iSig < nSignals; iSig++)
		{

			long lDataType = 0;
			long nFirstBit = 0;
			long nBits = 0;
			double Factor = 0.0;
			double Offset = 0.0;
			double RawMin = 0.0;
			double RawMax = 0.0;
			long bHasNoValues = 0;
			long invalPos = 0;
			mdf4Reader->GetSignalDetail(&lDataType, &nFirstBit, &nBits, &Factor, &Offset, &RawMin, &RawMax, &bHasNoValues, &invalPos);

			std::cout << "lDataType: " << lDataType << std::endl;
			std::cout << "nFirstBit: " << nFirstBit << std::endl;
			std::cout << "nBits: " << nBits << std::endl;
			std::cout << "Factor: " << Factor << std::endl;
			std::cout << "Offset: " << Offset << std::endl;
			std::cout << "RawMin: " << RawMin << std::endl;
			std::cout << "RawMax: " << RawMax << std::endl;
			std::cout << "bHasNoValues: " << bHasNoValues << std::endl;
			std::cout << "invalPos: " << invalPos << std::endl;


			readSignalCANFrames(mdf4Reader, mTrace, iSig, nValues, idx1, idx2, lTimeFactor);

		}
	}
	// Release the DLL object and free memory
	mdf4Reader->ExitDll();
	delete mdf4Reader;
	return true;
}

int CMdf4TeSSLaConverter::printDataHex(CMDF4ReaderLib* mdf4Reader, long indexSignal, long nValues, long idx1, long idx2, long lTimeFactor)
{
	CString signalName;
	signalName = mdf4Reader->LoadSignal(indexSignal);
	LONG discrete;
	wchar_t tDisplayName[256], tAliasName[256], tUnit[256], tComment[256];
	*tDisplayName = *tAliasName = *tUnit = *tComment = 0;
	mdf4Reader->GetSignal(tDisplayName, tAliasName, tUnit, tComment, &discrete);

	std::wstring wStr = std::wstring(tDisplayName);
	std::cout << "DisplayName: " << std::string(wStr.begin(), wStr.end()) << std::endl;
	wStr = std::wstring(tAliasName);
	std::cout << "\ttAliasName: " << std::string(wStr.begin(), wStr.end()) << std::endl;
	wStr = std::wstring(tUnit);
	std::cout << "\ttUnit: " << std::string(wStr.begin(), wStr.end()) << std::endl;
	wStr = std::wstring(tComment);
	std::cout << "\ttComment: " << std::string(wStr.begin(), wStr.end()) << std::endl;
	std::cout << "\tdiscrete: " << discrete << std::endl;

	long recordSize = mdf4Reader->GetRecordSize();
	unsigned char* buff = (unsigned char*) malloc(1024);
	//unsigned char* buff = (unsigned char*) malloc(recordSize+1);
	if (buff == NULL)
		return -1;
	buff[1024] = 0;
	//buff[recordSize] = 0;
	for (long i = 0; i < nValues; i++) {
		mdf4Reader->GetRecord(i, ((long) i) + 1, (BYTE*)buff);
		for (int j = 0; j < recordSize; j++)
			std::cout << ByteToHexString(buff[j]) << " ";
		std::cout << std::endl;
	}
	free(buff);
	return 0;
}

int CMdf4TeSSLaConverter::readSignalCANFrames(CMDF4ReaderLib* mdf4Reader, CTeSSLaTrace* trace, long indexSignal, long nValues, long idx1, long idx2, long lTimeFactor)
{
	CString signalName;
	signalName = mdf4Reader->LoadSignal(indexSignal);
	signalName = convertName(std::string(signalName)).c_str();
	LONG discrete;
	wchar_t tDisplayName[256], tAliasName[256], tUnit[256], tComment[256];
	*tDisplayName = *tAliasName = *tUnit = *tComment = 0;
	mdf4Reader->GetSignal(tDisplayName, tAliasName, tUnit, tComment, &discrete);
	double* pData = (double*)calloc(nValues, sizeof(double));
	double* pTimeData = (double*)calloc(nValues, sizeof(double));
	// Get the data from the time signal
	LONG countTimeData = mdf4Reader->GetData(TRUE, idx1, idx2, pTimeData);
	// Get the data form the signal
	LONG countData = mdf4Reader->GetData(FALSE, idx1, idx2, pData);

	//some error checks
	if (countTimeData != countData)
	{
		std::cerr << "Error in MDF4-File! Different numbers of data and time signals in " << signalName << std::endl;
		return -1;
	}
	if (pData == NULL || pTimeData == NULL)
	{
		std::cerr << "Error while reading MDF4-File at signal" << signalName << std::endl;
		return -1;
	}

	//buffer for signal records
	long recordSize = mdf4Reader->GetRecordSize();
	unsigned char* buff = new unsigned char[1024];
	if (buff == NULL)
		return -1;

	CCAN_FRAMETeSSLaStreamSet* streams = new CCAN_FRAMETeSSLaStreamSet();

	for (long i = 0; i < nValues; i++)
	{
		//get record
		mdf4Reader->GetRecord(i, i + 1, (BYTE*)buff);

		double timeStamp = ((double*)buff)[0];
		unsigned char* CANBytes = buff + 8;

		CCANFrame* canFrame = new CCANFrame(CANBytes, 19);

		streams->insertCANFrame(std::string(signalName), (long)(timeStamp * lTimeFactor), canFrame);

	}
	streams->addToTrace(trace);

	free(pData);
	free(pTimeData);
	delete[] buff;
	delete streams;
}


int CMdf4TeSSLaConverter::readSignalAuto(CMDF4ReaderLib* mdf4Reader, CTeSSLaTrace* trace, long indexSignal, long nValues, long idx1, long idx2, long lTimeFactor)
{
	CString signalName;
	signalName = mdf4Reader->LoadSignal(indexSignal);
	signalName = convertName(std::string(signalName)).c_str();
	LONG discrete;
	wchar_t tDisplayName[256], tAliasName[256], tUnit[256], tComment[256];
	*tDisplayName = *tAliasName = *tUnit = *tComment = 0;
	mdf4Reader->GetSignal(tDisplayName, tAliasName, tUnit, tComment, &discrete);
	double* pData = (double*)calloc(nValues, sizeof(double));
	double* pTimeData = (double*)calloc(nValues, sizeof(double));
	// Get the data from the time signal
	LONG countTimeData = mdf4Reader->GetData(TRUE, idx1, idx2, pTimeData);
	// Get the data form the signal
	LONG countData = mdf4Reader->GetData(FALSE, idx1, idx2, pData);

	//init new Stream
	CTeSSLaStreamFloat* stream;
	stream = new CTeSSLaStreamFloat(std::string(signalName));

	if (countTimeData != countData)
	{
		std::cerr << "Error in MDF4-File! Different numbers of data and time signals in " << signalName << std::endl;
		return -1;
	}
	if (pData == NULL || pTimeData == NULL)
	{
		std::cerr << "Error while reading MDF4-File at signal" << signalName << std::endl;
		return -1;
	}
	// Access the data:
	for (int i = 0; i < countTimeData; i++)
	{
		double value = pData[i];
		stream->addEntry(new CTeSSLaStreamEventFloat(stream, (long)(pTimeData[i] * lTimeFactor), pData[i]));
	}
	// free memory
	free(pTimeData);
	free(pData);
	//add the stream to the trace
	trace->addStream(stream);
	return 0;
}

bool CMdf4TeSSLaConverter::printMdf4FileInfo(std::string strPathToFile)
{
	// Load the tool as ordinary DLL rather than a COM object.
		// However, we use the COM registration to find the DLL:
	TCHAR szDLLPath[_MAX_PATH];
	if (!FindCOMLib(szDLLPath, TRUE))
	{
		perror("DLL not found!\n");
		return false;
	}
	// Create the object
	CMDF4ReaderLib m4(szDLLPath);	// The DLL object
	CString str, strTime;
	int iGrp, nGroups, iSig, nSignals;
	wchar_t szTextBuffer[256];
	LONG n, lVersion;
	BOOL bIsMDF4;
	double val;
	long mon, nValues = 10;
	__int64 idx1, idx2;
	double xmin, xmax, rmin, rmax, raster;

	if (!m4.IsValid())
	{
		perror("DLL not loaded!\n");
		return false;
	}
	m4.InitDll();

	std::cout << "Reading File: " << strPathToFile << std::endl;

	// Get an MDF4 file
	BOOL test = m4.OpenMDF4(_bstr_t(strPathToFile.c_str()));

	lVersion = m4.GetVersion();
	bIsMDF4 = lVersion >= 400;

	std::cout << "MF version: " << lVersion << std::endl;

	long invalidNume = m4.GetInvalidBytes();

	// Get file time and other infos
	CTime ti(m4.GetFileTime());
	val = m4.GetTimeFraction() * 1000; // ms
	str = ti.Format("%d:%m:%Y %H:%M:%S");
	printf("FileTime = %s.%03ld\n", str, (int)val);

	if (bIsMDF4)
	{
		str = m4.GetComment(ID_HEADER, hd_md_comment);
		if (!str.IsEmpty())
			printf("Header comment = %s\n", str);

		str = m4.GetComment(ID_FILEHISTORY, fh_md_comment);
		if (!str.IsEmpty())
			printf("File history = %s\n", str);
	}
	else
	{
		str = m4.GetComment(ID_HEADER, ID_AUTHOR);
		if (!str.IsEmpty())
			printf("Author = %s\n", str);
		str = m4.GetComment(ID_HEADER, ID_ORG);
		if (!str.IsEmpty())
			printf("Organisation = %s\n", str);
		str = m4.GetComment(ID_HEADER, ID_PROJECT);
		if (!str.IsEmpty())
			printf("Project = %s\n", str);
		str = m4.GetComment(ID_HEADER, ID_SUBJECT);
		if (!str.IsEmpty())
			printf("Sunject = %s\n", str);
		str = m4.GetComment(ID_HEADER, ID_DATE);
		if (!str.IsEmpty())
			printf("Date = %s\n", str);
		str = m4.GetComment(ID_HEADER, ID_TIME);
		if (!str.IsEmpty())
			printf("Time = %s\n", str);

		str = m4.GetComment(ID_TEXTBLOCK, 0);
		if (!str.IsEmpty())
			printf("Text block = %s\n", str);
	}
	nGroups = m4.GetNGroups();
	std::cout << "No. of valid data groups: " << nGroups << std::endl;
	if (bIsMDF4) // MDF3 has no group name
	{
		for (int i = 0; i < nGroups; i++)
		{
			str = m4.GetGroupName(i);
			std::cout << "\tGroup " << i << ": " << str << std::endl;
		}
	}
	std::cout << "Invalid Bytes: " << m4.GetInvalidBytes() << std::endl;

		
	// walk through all groups
	for (iGrp = 0; iGrp < nGroups; iGrp++)
	{
		if (bIsMDF4) // MDF3 has no group name
		{
			str = m4.GetGroupName(iGrp);
			printf("Group %ld = %s\n", iGrp, (char*)_bstr_t(str));
		}
		// Load the current group
		m4.LoadGroup(iGrp);
		strTime.Empty();

		if (bIsMDF4)
		{
			// Comment from data group:
			str = m4.GetComment(ID_DATAGROUP, dg_md_comment);
			if (!str.IsEmpty())
				printf("  Comment = %s\n", str);
			// Comments from channel group:
			str = m4.GetComment(ID_CHANNELGROUP, cg_tx_acq_name);
			if (!str.IsEmpty())
				printf("  CG name = %s\n", str);
			str = m4.GetComment(ID_CHANNELGROUP, cg_md_comment);
			if (!str.IsEmpty())
				printf("  CG comment = %s\n", str);
			// Comments from channel groups SI block :
			str = m4.GetComment(ID_SI_GROUP, si_tx_name);
			if (!str.IsEmpty())
				printf("  CG SI name = %s\n", str);
			str = m4.GetComment(ID_SI_GROUP, si_tx_path);
			if (!str.IsEmpty())
				printf("  CG SI path = %s\n", str);
			str = m4.GetComment(ID_SI_GROUP, si_md_comment);
			if (!str.IsEmpty())
				printf("  CG SI comment = %s\n", str);
		}

		// Load the time signal of the current group
		strTime = m4.LoadTimeSignal();

		nSignals = m4.GetNSignals();
		unsigned __int64 ll = m4.GetNSamples();
		printf("  No. of signals = %ld, %lu samples\n", nSignals, (unsigned long)ll);

		// Time range of group measurement:
		xmin = m4.GetFirstTimestamp();
		xmax = m4.GetLastTimestamp();
		printf("  Time %lf to %lf\n", xmin, xmax);
		printf("  Time signal %s\n", strTime);

		// Get time raster information
		// raster = delta t between data points
		// rmin   = minimal delta
		// rmax   = maximal delta, same as rmin for equidistant data
		// mon    = Monotony, defined by MDF4 as 
		// #define CN_MON_NOTDEFINED       0
		// #define CN_MON_DECREASE         1
		// #define CN_MON_INCREASE         2
		// #define CN_MON_STRICT_DECREASE  3
		// #define CN_MON_STRICT_INCREASE  4
		// #define CN_MONOTONOUS           5
		// #define CN_STRICT_MON           6
		// #define CN_NOT_MON              7
		LONG bIsVirtual = FALSE;
		*szTextBuffer = 0;
		m4.GetTimeSignal(szTextBuffer, &mon, &rmin, &rmax, &raster, &bIsVirtual);
		printf("    Unit = %S  Raster = %lf (%lf - %lf)  %ld\n", szTextBuffer, raster, rmin, rmax, mon);

		// Get indexes of time range xmin + 8s, xmin + 8.5s
		if (xmax >= xmin + 8.5)
		{
			idx1 = m4.TimeToIndex(xmin + 8.0, 0);
			idx2 = m4.TimeToIndex(xmin + 8.5, idx1);
		}
		else // other time range
		{
			idx1 = m4.TimeToIndex(xmin, 0);
			idx2 = m4.TimeToIndex(xmax, idx1);
		}
		nValues = (LONG) idx2 - idx1 + 1; // Calc number of data points 

		// walk thru signals
		for (iSig = 0; iSig < nSignals; iSig++)
		{
			str = m4.LoadSignal(iSig);
			printf("  Signal %s:\n", str);
			// Get signal description
			LONG discrete; // signal has discrete values (logical signal)
			wchar_t tDisplayName[256], tAliasName[256], tUnit[256], tComment[256];
			*tDisplayName = *tAliasName = *tUnit = *tComment = 0;
			m4.GetSignal(tDisplayName, tAliasName, tUnit, tComment, &discrete);
			printf("    Displayname %S\n", tDisplayName);
			printf("    Aliasname %S\n", tAliasName);
			printf("    Unit %S\n", tUnit);
			printf("    Comment %S\n", tComment);

			// Comments from channel (same as above):
			str = m4.GetComment(ID_CHANNEL, cn_tx_name);
			if (!str.IsEmpty())
				printf("  CN name = %s\n", str);
			str = m4.GetComment(ID_CHANNEL, cn_md_comment);
			if (!str.IsEmpty())
				printf("  CN comment = %s\n", str);
			// Comments from channel SI source
			str = m4.GetComment(ID_SI_CHANNEL, si_tx_name);
			if (!str.IsEmpty())
				printf("  CN SI name = %s\n", str);
			str = m4.GetComment(ID_SI_CHANNEL, si_tx_path);
			if (!str.IsEmpty())
				printf("  CN SI path = %s\n", str);
			str = m4.GetComment(ID_SI_CHANNEL, si_md_comment);
			if (!str.IsEmpty())
				printf("  CN SI comment = %s\n", str);

			// Read data from time signal and the signal itself:
			// Method 1: 
			// Create 2 SafeArrays
			double* pData = (double*)calloc(nValues, sizeof(double));
			double* pTimeData = (double*)calloc(nValues, sizeof(double));
			if (pData == NULL || pTimeData == NULL)
				throw new std::bad_alloc();
			// Get the data from the time signal
			n = m4.GetData(TRUE, idx1, idx2, pTimeData);
			// Get the data form the signal
			n = m4.GetData(FALSE, idx1, idx2, pData);
			// Access the data:
			for (int i = 0; i < n; i++)
			{
				printf("    %.3lf  %.3lf\n", pTimeData[i], pData[i]);
			}
			// free memory
			free(pTimeData);
			free(pData);
			
		}
	}
	// Release the DLL object and free memory
	m4.ExitDll();
	return true;
}

bool CMdf4TeSSLaConverter::exportTeSSLaFile(std::string strPathToFile)
{
	if (mTrace != NULL)
		return mTrace->exportTrace(strPathToFile);
	else
		return false;
}

void CMdf4TeSSLaConverter::testOutput()
{
	if (mTrace != NULL)
		mTrace->printTrace();
}

BOOL CMdf4TeSSLaConverter::FindCOMLib(TCHAR* pszPath, BOOL bReader)
{
	static TCHAR* WriterDLLKEY = _T("CLSID\\{891BCB49-095B-417C-9235-564194E85533}\\InprocServer32");
	static TCHAR* ReaderDLLKEY = _T("CLSID\\{A5D406EA-0508-415E-B5E2-E868370D3721}\\InprocServer32");
	HKEY hk;
	DWORD ulOptions = 0;
	REGSAM samDesired = KEY_QUERY_VALUE; // security access mask
	TCHAR* pszKey = WriterDLLKEY;
	if (bReader)
		pszKey = ReaderDLLKEY;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszKey, ulOptions, samDesired, &hk) == ERROR_SUCCESS)
	{
		DWORD res = 0, type = 0;
		DWORD cb = _MAX_PATH;
		res = RegQueryValueEx(hk, _T(""), 0L, &type, (LPBYTE)pszPath, &cb);
		RegCloseKey(hk);
		if (res == ERROR_SUCCESS)
			return TRUE;
	}
	free(WriterDLLKEY);
	free(ReaderDLLKEY);
	return FALSE;
}

std::string CMdf4TeSSLaConverter::convertName(std::string exisitingName)
{
	const int forbiddenCharCount = 6;
	char forbiddenChars[forbiddenCharCount] = { '.', '-', ':', '=', ' ', '\n' };
	char replaceWith = '_';
	for (int i = 0; i < forbiddenCharCount; i++)
	{
		while (exisitingName.find(forbiddenChars[i]) != std::string::npos)
		{
			std::cout << "Invalid char (" << forbiddenChars[i] << ") found in signal name. Replaced by '_'" << std::endl;
			exisitingName.replace(exisitingName.find(forbiddenChars[i]), 1, &replaceWith, 1);
		}
	}
	return exisitingName;
}