#include "stdafx.h"
#include "CMDF4WriterLib.h"


CMDF4WriterLib::CMDF4WriterLib(const char* pszDLLPathName)
{
	m_hMod = LoadLibraryEx(pszDLLPathName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (m_hMod)
	{
		(FARPROC&)m_pfInitDll = GetProcAddress(m_hMod, "M4WRInitDll");
		(FARPROC&)m_pfExitDll = GetProcAddress(m_hMod, "M4WRExitDll");
		(FARPROC&)m_pfCreateMDF4 = GetProcAddress(m_hMod, "M4WRCreateMDF4");
		(FARPROC&)m_pfSetPathName = GetProcAddress(m_hMod, "M4WRSetPathName");
		(FARPROC&)m_pfAddGroup = GetProcAddress(m_hMod, "M4WRAddGroup");
		(FARPROC&)m_pfAddGroup64 = GetProcAddress(m_hMod, "M4WRAddGroup64");
		(FARPROC&)m_pfAddSignal = GetProcAddress(m_hMod, "M4WRAddSignal");
		(FARPROC&)m_pfAddTimeInfo = GetProcAddress(m_hMod, "M4WRAddTimeInfo");
		(FARPROC&)m_pfAddTimeSignal = GetProcAddress(m_hMod, "M4WRAddTimeSignal");
		(FARPROC&)m_pfMakeGroups = GetProcAddress(m_hMod, "M4WRMakeGroups");
		(FARPROC&)m_pfSetSignalValue = GetProcAddress(m_hMod, "M4WRSetSignalValue");
		(FARPROC&)m_pfWriteRecord = GetProcAddress(m_hMod, "M4WRWriteRecord");
		(FARPROC&)m_pfFlushGroup = GetProcAddress(m_hMod, "M4WRFlushGroup");
		(FARPROC&)m_pfClose = GetProcAddress(m_hMod, "M4WRClose");
		(FARPROC&)m_pfCreateSRBlock = GetProcAddress(m_hMod, "M4WRCreateSRBlock");
		(FARPROC&)m_pfSetFileTime = GetProcAddress(m_hMod, "M4WRSetFileTime");
		(FARPROC&)m_pfGroupDescription = GetProcAddress(m_hMod, "M4WRGroupDescription");
		(FARPROC&)m_pfSignalDescription = GetProcAddress(m_hMod, "M4WRSignalDescription");
		(FARPROC&)m_pfGetRecordSize = GetProcAddress(m_hMod, "M4WRGetRecordSize");
		(FARPROC&)m_pfSetRecord = GetProcAddress(m_hMod, "M4WRSetRecord");
		(FARPROC&)m_pfSetMinMax = GetProcAddress(m_hMod, "M4WRSetMinMax");
		(FARPROC&)m_pfGetInvalidBytes = GetProcAddress(m_hMod, "M4WRGetInvalidBytes");
		(FARPROC&)m_pfFileDescription = GetProcAddress(m_hMod, "M4WRFileDescription");
		(FARPROC&)m_pfSetSignalDiscrete = GetProcAddress(m_hMod, "M4WRSetSignalDiscrete");
		(FARPROC&)m_pfSetTimeStat = GetProcAddress(m_hMod, "M4WRSetTimeStat");
		(FARPROC&)m_pfSetRecordValues = GetProcAddress(m_hMod, "M4WRSetRecordValues");
		(FARPROC&)m_pfCreateSRBlocks = GetProcAddress(m_hMod, "M4WRCreateSRBlocks");
		(FARPROC&)m_pfSetFileTimeFraction = GetProcAddress(m_hMod, "M4WRSetFileTimeFraction");
		(FARPROC&)m_pfSetNanoTimeUTC = GetProcAddress(m_hMod, "M4WRSetNanoTimeUTC");
		(FARPROC&)m_pfWriteRecords = GetProcAddress(m_hMod, "M4WRWriteRecords");
	}	
}		

CMDF4WriterLib::~CMDF4WriterLib()
{
	if (m_hMod)
		FreeLibrary(m_hMod);
	m_hMod = NULL;
}

INT_PTR CMDF4WriterLib::InitDll(void)
{
	m_hHandle = (*m_pfInitDll)();
	return m_hHandle;
}
void CMDF4WriterLib::ExitDll()
{
	(*m_pfExitDll)(m_hHandle);
}


BOOL CMDF4WriterLib::CreateMDF4(const wchar_t* strToolName, const wchar_t* strToolVendor, const wchar_t* strToolVersion, const wchar_t* strComment, INT_PTR lVersion)
{
	return (*m_pfCreateMDF4)(m_hHandle, strToolName, strToolVendor, strToolVersion, strComment, lVersion);
}
void CMDF4WriterLib::SetPathName(const wchar_t* strName)
{
	(*m_pfSetPathName)(m_hHandle, strName);
}
void CMDF4WriterLib::AddGroup(const wchar_t* strComment, DWORD_PTR ExpNValues, INT_PTR* iNo)
{
	(*m_pfAddGroup)(m_hHandle, strComment, ExpNValues, iNo);
}
void CMDF4WriterLib::AddGroup64(const wchar_t* strComment, __int64 ExpNValues, INT_PTR* iNo)
{
	(*m_pfAddGroup64)(m_hHandle, strComment, ExpNValues, iNo);
}
BOOL CMDF4WriterLib::AddSignal(LONG iGroupNo, const wchar_t* strName, const wchar_t* strLabel, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE yFactor, DOUBLE yOffset, LONG bHasNovalues, DOUBLE Novalue, LONG invalPos, LONG* iNo)
{
	return (*m_pfAddSignal)(m_hHandle, iGroupNo, strName, strLabel, lDataType, lnFirstBit, lnBits, strUnit, yFactor, yOffset, bHasNovalues,  Novalue, invalPos, iNo);
}
BOOL CMDF4WriterLib::AddTimeInfo(LONG iGroupNo, const wchar_t* strName, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset)
{
	return (*m_pfAddTimeInfo)(m_hHandle, iGroupNo, strName, strUnit, Factor, Offset);
}
BOOL CMDF4WriterLib::AddTimeSignal(LONG iGroupNo, const wchar_t* strName, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset, LONG* iNo)
{
	return (*m_pfAddTimeSignal)(m_hHandle, iGroupNo, strName, lDataType, lnFirstBit, lnBits, strUnit, Factor, Offset, iNo);
}
BOOL CMDF4WriterLib::MakeGroups()
{
	return (*m_pfMakeGroups)(m_hHandle);
}
BOOL CMDF4WriterLib::SetSignalValue(LONG iGroupNo, LONG iSignalNo, DOUBLE Value, LONG bIsNovalue)
{
	return (*m_pfSetSignalValue)(m_hHandle, iGroupNo, iSignalNo, Value, bIsNovalue);
}
BOOL CMDF4WriterLib::WriteRecord(LONG iGroupNo)
{
	return (*m_pfWriteRecord)(m_hHandle, iGroupNo);
}
BOOL CMDF4WriterLib::FlushGroup(LONG iGroupNo)
{
	return (*m_pfFlushGroup)(m_hHandle, iGroupNo);
}
void CMDF4WriterLib::Close()
{
	(*m_pfClose)(m_hHandle);
}
BOOL CMDF4WriterLib::CreateSRBlock(LONG lGroupNo, DOUBLE dt, DOUBLE xrange, DOUBLE OldXFactor, DOUBLE OldXOffset)
{
	return (*m_pfCreateSRBlock)(m_hHandle, lGroupNo, dt, xrange, OldXFactor, OldXOffset);
}
void CMDF4WriterLib::SetFileTime(LONGLONG FileTime)
{
	(*m_pfSetFileTime)(m_hHandle, FileTime);
}
BOOL CMDF4WriterLib::GroupDescription(LONG lGroupNo, const wchar_t* strAcqName, const wchar_t* strCGComment, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment)
{
	return (*m_pfGroupDescription)(m_hHandle, lGroupNo, strAcqName, strCGComment, strSIName, strSIPath, strSIComment);
}
BOOL CMDF4WriterLib::SignalDescription(LONG iGroupNo, LONG iSignalNo, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment)
{
	return (*m_pfSignalDescription)(m_hHandle, iGroupNo, iSignalNo, strSIName, strSIPath, strSIComment);
}
DWORD CMDF4WriterLib::GetRecordSize(LONG lGroupID)
{
	return (*m_pfGetRecordSize)(m_hHandle, lGroupID);
}
void CMDF4WriterLib::SetRecord(LONG lGroupNo, BYTE* pBuffer)
{
	(*m_pfSetRecord)(m_hHandle, lGroupNo, pBuffer);
}
BOOL CMDF4WriterLib::SetMinMax(LONG iGroupNo, LONG iSignalNo, double Min, double Max)
{
	return (*m_pfSetMinMax)(m_hHandle, iGroupNo, iSignalNo, Min, Max);
}
LONG CMDF4WriterLib::GetInvalidBytes(LONG lGroupID)
{
	return (*m_pfGetInvalidBytes)(m_hHandle, lGroupID);
}
BOOL CMDF4WriterLib::FileDescription(const wchar_t* strHDComment, LONG lTimerQualityClass, LONGLONG start_time_ns, WORD tz_offset_min, WORD dst_offset_min, BYTE time_flags)
{
	return (*m_pfFileDescription)(m_hHandle, strHDComment, lTimerQualityClass, start_time_ns, tz_offset_min, dst_offset_min, time_flags);
}
BOOL CMDF4WriterLib::SetSignalDiscrete(LONG iGroupNo, LONG iSignalNo, LONG bDiscrete, const wchar_t* strN2T)
{
	return (*m_pfSetSignalDiscrete)(m_hHandle, iGroupNo, iSignalNo, bDiscrete, strN2T);
}
BOOL CMDF4WriterLib::SetTimeStat(LONG iGroupNo, DOUBLE xMean, DOUBLE xMin, DOUBLE xMax, DOUBLE xMinDelta, DOUBLE xMaxDelta)
{
	return (*m_pfSetTimeStat)(m_hHandle, iGroupNo, xMean, xMin, xMax, xMinDelta, xMaxDelta);
}
BOOL CMDF4WriterLib::SetRecordValues(LONG iGroupNo, DOUBLE *pValues, LONG nRecords)
{
	return (*m_pfSetRecordValues)(m_hHandle, iGroupNo, pValues, nRecords);
}
void CMDF4WriterLib::CreateSRBlocks()
{
	(*m_pfCreateSRBlocks)(m_hHandle);
}
void CMDF4WriterLib::SetFileTimeFraction(DOUBLE FileTimeFraction)
{
	(*m_pfSetFileTimeFraction)(m_hHandle, FileTimeFraction);
}
void CMDF4WriterLib::SetNanoTimeUTC(LONGLONG NanoTimeUTC)
{
	(*m_pfSetNanoTimeUTC)(m_hHandle, NanoTimeUTC);
}
BOOL CMDF4WriterLib::WriteRecords(LONG iGroupNo, void *pBuffer, LONG nRecords)
{
	return (*m_pfWriteRecords)(m_hHandle, iGroupNo, pBuffer, nRecords);
}
