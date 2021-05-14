#pragma once

#include "CTeSSLaTrace.h"

#include "CMDF4ReaderLib.h"

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

class CMdf4TeSSLaConverter
{
public:
	//reads and MDF4 File
	//	lTimeFactor-> mutliplying factor to convert float time to ints. Factor 1,000-> 1 <=> 1ms
	bool readMdf4File(std::string strPathToFile, long lTimeFactor = 1000);
	bool CMdf4TeSSLaConverter::printMdf4FileInfo(std::string strPathToFile);

	bool exportTeSSLaFile(std::string strPathToFile);

	void testOutput();
protected:
	CTeSSLaTrace* mTrace;
private: 
	BOOL FindCOMLib(TCHAR* pszPath, BOOL bReader = FALSE);
	int readSignalFloat(CMDF4ReaderLib* mdf4Reader, CTeSSLaTrace* trace, long indexSignal, long nValues, long idx1, long idx2, long lTimeFactor);

	int printDataHex(CMDF4ReaderLib* mdf4Reader, long indexSignal, long nValues, long idx1, long idx2, long lTimeFactor);
};

