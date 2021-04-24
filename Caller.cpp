// Caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Caller.h"
#include "CMdf4Reader.h"
#include "CMDF4ReaderLib.h"
#include <io.h>
#include <atlsafe.h>
//#include <vld.h>


#include <afxwin.h>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

void ReadMDF4Example(void); // forward
void DLLReadMDF4Example(void);  // forward

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	cout << "main a" << endl;

	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	cout << "main b" << endl;

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			cout << "main Error 1" << endl;
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// Test MDF4Writer (COM)
			// WriteMDF4Example();

			// Test MDF4Writer (DLL)
			cout << "main c" << endl;
			//DLLWriteMDF4Example();

			// Test MDF4Reader
			//ReadMDF4Example();

			// Test MDF4Reader (DLL)
			cout << "main b" << endl;
			DLLReadMDF4Example();
		}
	}
	else
	{
		cout << "main error 2" << endl;
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	cout << "main end with code: " << nRetCode << endl;
	return nRetCode;
}



// Find the DLL if it was registered as a COM object:

static TCHAR *WriterDLLKEY = _T("CLSID\\{891BCB49-095B-417C-9235-564194E85533}\\InprocServer32");
static TCHAR *ReaderDLLKEY = _T("CLSID\\{A5D406EA-0508-415E-B5E2-E868370D3721}\\InprocServer32");

BOOL FindCOMLib(TCHAR *pszPath, BOOL bReader = FALSE) // size of pszPath is _MAX_PATH !
{
	HKEY hk;
	DWORD ulOptions = 0;
	REGSAM samDesired = KEY_QUERY_VALUE; // security access mask
	TCHAR *pszKey = WriterDLLKEY;
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



// Block types
#define ID_HEADER       1
#define ID_FILEHISTORY  2
#define ID_DATAGROUP    3
#define ID_CHANNELGROUP 4
#define ID_CHANNEL      5
#define ID_SI_GROUP     6
#define ID_SI_CHANNEL   7
#define ID_TEXTBLOCK    8

// Header Contents MDF3
#define ID_AUTHOR  0
#define ID_ORG     1
#define ID_PROJECT 2
#define ID_SUBJECT 3
#define ID_DATE    4
#define ID_TIME    5

// enumeration for text/comment members
// Header
#define hd_md_comment 5
// File History
#define fh_md_comment 1
// Data Group
#define dg_md_comment 3
// Channel Group
#define cg_tx_acq_name 2
#define cg_md_comment  5
// Channel 
#define cn_tx_name     2
#define cn_md_comment  7
// Channel / Channel Group: SI Block
#define si_tx_name    0
#define si_tx_path    1
#define si_md_comment 2

void ReadMDF4Example(void)
{
	CMdf4Reader m4; // The COM object
	CString str;
	int iGrp, nGroups, iSig, nSignals;
	LONG l,n, lVersion;
  BOOL bIsMDF4;
	double val;
	long mon, nValues=10, idx1, idx2;
	double xmin, xmax, rmin, rmax, raster;
	// Some BSTRs
	BSTR t,tTime;

	CoInitializeEx(NULL, 0); // don't forget this
	// Create the object
	//if (!m4.CreateDispatch(_T("{A5D406EA-0508-415E-B5E2-E868370D3721}")))
	if (!m4.CreateDispatch(_T("MDF4ReaderLib.1")))
	{
		DWORD dwErr = GetLastError();
		_tprintf(_T("Cannot create dispatch interface\n"));
		return;
	}

	// Get an MDF4 file
	CFileDialog fdlg(TRUE,_T(".mf4"));
	if (fdlg.DoModal() != IDOK)
		return;
	printf("File %s\n", fdlg.GetPathName());
	m4.OpenMDF4(fdlg.GetPathName());
  lVersion = m4.get_Version();
  bIsMDF4 = lVersion >= 400;

	// Get file time and other infos
	CTime ti(m4.get_FileTime());
	val = m4.get_TimeFraction()*1000; // ms
	str = ti.Format("%d:%m:%Y %H:%M:%S");
	printf("FileTime = %s.%03ld\n",str,(int)val);

  if(bIsMDF4)
  {
    str = m4.get_Comment(ID_HEADER, hd_md_comment);
    if(!str.IsEmpty())
      printf("Header comment = %s\n", str);

    str = m4.get_Comment(ID_FILEHISTORY, fh_md_comment);
    if(!str.IsEmpty())
      printf("File history = %s\n", str);
  }
  else
  {
    str = m4.get_Comment(ID_HEADER, ID_AUTHOR);
    if(!str.IsEmpty())
      printf("Author = %s\n", str);
    str = m4.get_Comment(ID_HEADER, ID_ORG);
    if(!str.IsEmpty())
      printf("Organisation = %s\n", str);
    str = m4.get_Comment(ID_HEADER, ID_PROJECT);
    if(!str.IsEmpty())
      printf("Project = %s\n", str);
    str = m4.get_Comment(ID_HEADER, ID_SUBJECT);
    if(!str.IsEmpty())
      printf("Sunject = %s\n", str);
    str = m4.get_Comment(ID_HEADER, ID_DATE);
    if(!str.IsEmpty())
      printf("Date = %s\n", str);
    str = m4.get_Comment(ID_HEADER, ID_TIME);
    if(!str.IsEmpty())
      printf("Time = %s\n", str);
    
    str = m4.get_Comment(ID_TEXTBLOCK, 0);
    if(!str.IsEmpty())
      printf("Text block = %s\n", str);
  }
  nGroups = m4.get_NGroups();
	printf("No. of groups = %ld\n",nGroups);
	// walk through all groups
	for (iGrp=0; iGrp<nGroups; iGrp++)
	{
    if(bIsMDF4) // MDF3 has no group name
    {
      // Calling a COM function which uses BSTR:
      t=NULL; tTime=NULL; // BSTR must be NULL 
      m4.GetGroupName(&t, iGrp); // allocates t
      printf("Group %ld = %s\n", iGrp+1, (char *)_bstr_t(t));
      ::SysFreeString(t); // we must free this string
    }
		// Load the current group
		m4.LoadGroup(iGrp);
		tTime=NULL;
		
    if(bIsMDF4)
    {
      // Comment from data group:
      str = m4.get_Comment(ID_DATAGROUP, dg_md_comment);
      if(!str.IsEmpty())
        printf("  Comment = %s\n", str);
      // Comments from channel group:
      str = m4.get_Comment(ID_CHANNELGROUP, cg_tx_acq_name);
      if(!str.IsEmpty())
        printf("  CG name = %s\n", str);
      str = m4.get_Comment(ID_CHANNELGROUP, cg_md_comment);
      if(!str.IsEmpty())
        printf("  CG comment = %s\n", str);
      // Comments from channel groups SI block :
      str = m4.get_Comment(ID_SI_GROUP, si_tx_name);
      if(!str.IsEmpty())
        printf("  CG SI name = %s\n", str);
      str = m4.get_Comment(ID_SI_GROUP, si_tx_path);
      if(!str.IsEmpty())
        printf("  CG SI path = %s\n", str);
      str = m4.get_Comment(ID_SI_GROUP, si_md_comment);
      if(!str.IsEmpty())
        printf("  CG SI comment = %s\n", str);
    }

		// Load the time signal of the current group
		m4.LoadTimeSignal(&tTime);

		nSignals = m4.get_NSignals();
		unsigned __int64 ll = m4.get_NSamples64();
		l = m4.get_NSamples();
		printf("  No. of signals = %ld, %ld samples\n",nSignals,l);

		// Time range of group measurement:
		xmin = m4.get_FirstTimestamp();
		xmax = m4.get_LastTimestamp();
		printf("  Time %lf to %lf\n",xmin,xmax);
		printf("  Time signal %s\n", (char *)_bstr_t(tTime));
	  ::SysFreeString(tTime);

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
		t = NULL;
		LONG bIsVirtual = FALSE;
		m4.GetTimeSignal(&t, &mon, &rmin, &rmax, &raster, &bIsVirtual);
		printf("    Unit = %s  Raster = %lf (%lf - %lf)  %ld\n", (char *)_bstr_t(t),raster,rmin,rmax,mon);
	  ::SysFreeString(t);

		// Get indexes of time range xmin + 8s, xmin + 8.5s
		if (xmax >= xmin+8.5)
		{
			m4.TimeToIndex(xmin+8.0, 0, &idx1);
			m4.TimeToIndex(xmin+8.5, idx1, &idx2);
		}
		else // other time range
		{
			m4.TimeToIndex(xmin, 0, &idx1);
			m4.TimeToIndex(xmax, idx1, &idx2);
		}
		nValues = idx2-idx1+1; // Calc number of data points 

		// walk thru signals
		for (iSig=0; iSig<nSignals; iSig++)
		{
			t = NULL;
			m4.LoadSignal(&t, iSig);
			printf("  Signal %s:\n", (char *)_bstr_t(t));
		  ::SysFreeString(t);
			// Get signal description
			LONG discrete; // signal has discrete values (logical signal)
			BSTR tDisplayName=NULL, tAliasName=NULL, tUnit=NULL, tComment=NULL;
			m4.GetSignal(&tDisplayName, &tAliasName, &tUnit, &tComment, &discrete);
			printf("    Displayname %s\n", (char *)_bstr_t(tDisplayName));
			printf("    Aliasname %s\n", (char *)_bstr_t(tAliasName));
			printf("    Unit %s\n", (char *)_bstr_t(tUnit));
			printf("    Comment %s\n", (char *)_bstr_t(tComment));
		  ::SysFreeString(tDisplayName);
		  ::SysFreeString(tAliasName);
		  ::SysFreeString(tUnit);
		  ::SysFreeString(tComment);

			// Comments from channel (same as above):
			str = m4.get_Comment(ID_CHANNEL, cn_tx_name);
			if (!str.IsEmpty())
				printf("  CN name = %s\n",str);
			str = m4.get_Comment(ID_CHANNEL, cn_md_comment);
			if (!str.IsEmpty())
				printf("  CN comment = %s\n",str);
			// Comments from channel SI source
			str = m4.get_Comment(ID_SI_CHANNEL, si_tx_name);
			if (!str.IsEmpty())
				printf("  CN SI name = %s\n",str);
			str = m4.get_Comment(ID_SI_CHANNEL, si_tx_path);
			if (!str.IsEmpty())
				printf("  CN SI path = %s\n",str);
			str = m4.get_Comment(ID_SI_CHANNEL, si_md_comment);
			if (!str.IsEmpty())
				printf("  CN SI comment = %s\n",str);

			// Read data from time signal and the signal itself:
			// Method 1: Use a SafeArray
			// Create 2 SafeArrays
			CComSafeArray<double> *pData;
			CComSafeArray<double> *pTimeData;
			CComSafeArrayBound bound;
			bound.SetCount(nValues); // nValues has been claculated above
			bound.SetLowerBound(0);
			pData = new CComSafeArray<double>(&bound,1); 
			pTimeData = new CComSafeArray<double>(&bound,1); 
			// Wrap safearrays by a VARIANT
			VARIANT vData, vTimeData;
			vData.parray = *pData->GetSafeArrayPtr();
			vData.vt = VT_ARRAY;
			pData->Detach();
			vTimeData.parray = *pTimeData->GetSafeArrayPtr();
			vTimeData.vt = VT_ARRAY;
			pTimeData->Detach(); // do not lock before call
			// Get the data from the time signal
			m4.GetData(TRUE, idx1, idx2, &vTimeData, &n);
			// Get the data form the signal
			m4.GetData(FALSE, idx1, idx2, &vData, &n);
			pData->Attach(vData.parray);
			pTimeData->Attach(vTimeData.parray);
			// Access the data:
		  for (int i=0; i<n; i++)
			{
				printf("    %.3lf  %.3lf\n",pTimeData->GetAt(i), pData->GetAt(i));
			}
			// free memory
			delete pTimeData;
			delete pData;
			printf("=============================================\n");

			// Method 2: Use cached data and values one-by-one
			m4.CacheData(TRUE, idx1, idx2, &n);
			m4.CacheData(FALSE, idx1, idx2, &n);
			// Access the data:
		  for (int i=idx1; i<idx1+n; i++)
			{
				printf("    %.3lf  %.3lf\n",m4.get_CachedValue(TRUE, i), m4.get_CachedValue(FALSE, i));
			}

			// SR Blocks
			double Min,Max,Mean;
			int isr, nsr = m4.get_NoOfSRBlocks();
			printf("    %d SR Blocks\n",nsr);
			for (isr=0; isr<nsr; isr++)
			{
				double dt = m4.get_SRdt(isr);
				long lValues = m4.get_SRCycleCount(isr);
				printf("      SR Block %d\n        N = %ld, dt = %lf\n",isr+1,lValues,dt);
				if (lValues > 10)
					lValues = 10;
				m4.CacheSRData(isr, 1, 0, lValues-1, &lValues);
				m4.CacheSRData(isr, 0, 0, lValues-1, &lValues);
				for (long iv=0; iv<lValues; iv++)
				{
					m4.CachedSRValues(1, iv, &Min, &Max, &Mean);
					printf("        %lf | %lf | %lf ||",Min,Max,Mean);
					m4.CachedSRValues(0, iv, &Min, &Max, &Mean);
					printf(" %lf | %lf | %lf\n",Min,Max,Mean);
				}
			}
		}
	}
	// Release the COM object and free memory
	m4.ReleaseDispatch();
}

void DLLReadMDF4Example(void)
{
	// Load the tool as ordinary DLL rather than a COM object.
	// However, we use the COM registration to find the DLL:
	TCHAR szDLLPath[_MAX_PATH];
	if (!FindCOMLib(szDLLPath, TRUE))
	{
		printf("DLL not found\n");
		return;
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
		_tprintf(_T("DLL not loaded\n"));
		return;
	}
	m4.InitDll();

	// Get an MDF4 file
	static LPCTSTR fileFilter = _T("MDF4 Files (*.mf4)|*.mf4||");
	CFileDialog fdlg(TRUE, NULL, NULL, 6UL | OFN_ENABLESIZING, fileFilter);
	if (fdlg.DoModal() != IDOK)
		return;
	printf("File %s\n", fdlg.GetPathName());
	m4.OpenMDF4(_bstr_t(fdlg.GetPathName()));
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
			printf("Group %ld = %s\n", iGrp + 1, (char *)_bstr_t(str));
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
			double *pData = (double*)calloc(nValues, sizeof(double));
			double *pTimeData = (double*)calloc(nValues, sizeof(double));
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
			printf("=============================================\n");

			// Method 2: Use cached data and values one-by-one
			n = m4.CacheData(TRUE, idx1, idx2);
			n = m4.CacheData(FALSE, idx1, idx2);
			// Access the data:
			for (int i = idx1; i < idx1 + n; i++)
			{
				printf("    %.3lf  %.3lf\n", m4.GetCachedValue(TRUE, i), m4.GetCachedValue(FALSE, i));
			}

			// SR Blocks
			double Min, Max, Mean;
			int isr, nsr = m4.GetNoOfSRBlocks();
			printf("    %d SR Blocks\n", nsr);
			for (isr = 0; isr < nsr; isr++)
			{
				double dt = m4.GetSRdt(isr);
				long lValues = m4.GetSRCycleCount(isr);
				printf("      SR Block %d\n        N = %ld, dt = %lf\n", isr + 1, lValues, dt);
				if (lValues > 10)
					lValues = 10;
				lValues = m4.CacheSRData(isr, 1, 0, lValues - 1);
				lValues = m4.CacheSRData(isr, 0, 0, lValues - 1);
				for (long iv = 0; iv < lValues; iv++)
				{
					m4.GetCachedSRValues(1, iv, &Min, &Max, &Mean);
					printf("        %lf | %lf | %lf ||", Min, Max, Mean);
					m4.GetCachedSRValues(0, iv, &Min, &Max, &Mean);
					printf(" %lf | %lf | %lf\n", Min, Max, Mean);
				}
			}
		}
	}
	// Release the DLL object and free memory
	m4.ExitDll();
}

