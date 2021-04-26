#pragma once

#include "stdafx.h"
#include "CMdf4TeSSLaConverter.h"
#include "CMdf4Reader.h"

#include "CMDF4ReaderLib.h"
#include <io.h>
#include <atlsafe.h>
//#include <vld.h>


#include <afxwin.h>

bool CMdf4TeSSLaConverter::readMdf4File(std::string strPathToFile, long lTimeFactor)
{
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
	// find and init DLL
	TCHAR szDLLPath[_MAX_PATH];
	if (!FindCOMLib(szDLLPath, TRUE))
	{
		perror("DLL not found!\n");
		return false;
	}
	CMDF4ReaderLib mdf4Reader(szDLLPath);
	if (!mdf4Reader.IsValid())
	{
		perror("DLL not loaded!\n");
		return false;
	}
	mdf4Reader.InitDll();
	//load the file
	mdf4Reader.OpenMDF4(_bstr_t(strPathToFile.c_str()));


	if (mTrace != NULL)
		free(mTrace);
	//init tessla trace
	mTrace = new CTeSSLaTrace();

	// get number of groups
	long nGroups = mdf4Reader.GetNGroups();
	for (long iGrp = 0; iGrp < nGroups; iGrp++)
	{
		CString buff;
		buff= mdf4Reader.GetGroupName(iGrp);
		//activate current group
		if (!mdf4Reader.LoadGroup(iGrp))
		{
			perror("Error while reading data groups!\n");
			return false;
		}
		//get signal and sample count
		long nSignals = mdf4Reader.GetNSignals();
		unsigned __int64 nSamples = mdf4Reader.GetNSamples();
		//Time Range
		mdf4Reader.LoadTimeSignal();
		double xmin = mdf4Reader.GetFirstTimestamp();
		double xmax = mdf4Reader.GetLastTimestamp();
		//get time signal
		LONG bIsVirtual = FALSE, mon;
		double rmin, rmax, raster;
		wchar_t szTextBuffer[256];
		*szTextBuffer = 0;
		mdf4Reader.GetTimeSignal(szTextBuffer, &mon, &rmin, &rmax, &raster, &bIsVirtual);
		//get indices of time range
		long idx1, idx2;
		idx1 = mdf4Reader.TimeToIndex(xmin, 0);
		idx2 = mdf4Reader.TimeToIndex(xmax, idx1);
		// calc the number of values
		long nValues = idx2 - idx1 + 1; // Calc number of data points 
		// go through signals
		for (long iSig = 0; iSig < nSignals; iSig++)
		{
			CString signalName;
			signalName = mdf4Reader.LoadSignal(iSig);
			LONG discrete;
			wchar_t tDisplayName[256], tAliasName[256], tUnit[256], tComment[256];
			*tDisplayName = *tAliasName = *tUnit = *tComment = 0;
			mdf4Reader.GetSignal(tDisplayName, tAliasName, tUnit, tComment, &discrete);
			double* pData = (double*)calloc(nValues, sizeof(double));
			double* pTimeData = (double*)calloc(nValues, sizeof(double));
			// Get the data from the time signal
			LONG countTimeData = mdf4Reader.GetData(TRUE, idx1, idx2, pTimeData);
			// Get the data form the signal
			LONG countData = mdf4Reader.GetData(FALSE, idx1, idx2, pData);

			//init new Stream
			CTeSSLaStreamFloat* stream;
			stream = new CTeSSLaStreamFloat(std::string(signalName));

			if (countTimeData != countData || pData == NULL || pTimeData == NULL)
			{
				perror("Error in MDF4-File! Different numbers of data and time signals in " + signalName + "\n");
				return false;
			}
			if (pData == NULL || pTimeData == NULL)
			{
				perror("Error while reading MDF4-File at signal" + signalName + "\n");
				return false;
			}
			// Access the data:
			for (int i = 0; i < countTimeData; i++)
			{
				stream->addEntry(new CTeSSLaStreamEntryFloat(stream, (pTimeData[i]* lTimeFactor), pData[i]));
			}
			// free memory
			free(pTimeData);
			free(pData);
			//add the stream to the trace
			mTrace->addStream(stream);
		}
	}
	// Release the DLL object and free memory
	mdf4Reader.ExitDll();
	return true;
}

bool CMdf4TeSSLaConverter::printMdf4FileInfo(std::string strPathToFile)
{
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
	long mon, nValues = 10, idx1, idx2;
	double xmin, xmax, rmin, rmax, raster;

	if (!m4.IsValid())
	{
		perror("DLL not loaded!\n");
		return false;
	}
	m4.InitDll();

	// Get an MDF4 file
	m4.OpenMDF4(_bstr_t(strPathToFile.c_str()));
	lVersion = m4.GetVersion();
	bIsMDF4 = lVersion >= 400;

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
	printf("No. of groups = %ld\n", nGroups);
	// walk through all groups
	for (iGrp = 0; iGrp < nGroups; iGrp++)
	{
		if (bIsMDF4) // MDF3 has no group name
		{
			str = m4.GetGroupName(iGrp);
			printf("Group %ld = %s\n", iGrp + 1, (char*)_bstr_t(str));
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
		nValues = idx2 - idx1 + 1; // Calc number of data points 

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
	return FALSE;
}

