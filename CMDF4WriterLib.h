#pragma once
class CMDF4WriterLib
{
public:
	CMDF4WriterLib(const char* pszDLLPathName);
	~CMDF4WriterLib();

	BOOL IsValid(void)
	{
		return m_hMod != 0;
	}

	INT_PTR InitDll(void);
	void ExitDll();
	BOOL CreateMDF4(const wchar_t* strToolName, const wchar_t* strToolVendor, const wchar_t* strToolVersion, const wchar_t* strComment, INT_PTR lVersion);
	void SetPathName(const wchar_t* strName);
	void AddGroup(const wchar_t* strComment, DWORD_PTR ExpNValues, INT_PTR* iNo);
	void AddGroup64(const wchar_t* strComment, __int64 ExpNValues, INT_PTR* iNo);
	BOOL AddSignal(LONG iGroupNo, const wchar_t* strName, const wchar_t* strLabel, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE yFactor, DOUBLE yOffset, LONG bHasNovalues, DOUBLE Novalue, LONG invalPos, LONG* iNo);
	BOOL AddTimeInfo(LONG iGroupNo, const wchar_t* strName, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset);
	BOOL AddTimeSignal(LONG iGroupNo, const wchar_t* strName, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset, LONG* iNo);
	BOOL MakeGroups();
	BOOL SetSignalValue(LONG iGroupNo, LONG iSignalNo, DOUBLE Value, LONG bIsNovalue);
	BOOL WriteRecord(LONG iGroupNo);
	BOOL FlushGroup(LONG iGroupNo);
	void Close();
	BOOL CreateSRBlock(LONG lGroupNo, DOUBLE dt, DOUBLE xrange, DOUBLE OldXFactor, DOUBLE OldXOffset);
	void SetFileTime(LONGLONG FileTime);
	BOOL GroupDescription(LONG lGroupNo, const wchar_t* strAcqName, const wchar_t* strCGComment, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment);
	BOOL SignalDescription(LONG iGroupNo, LONG iSignalNo, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment);
	DWORD GetRecordSize(LONG lGroupID);
	void SetRecord(LONG lGroupNo, BYTE* pBuffer);
	BOOL SetMinMax(LONG iGroupNo, LONG iSignalNo, double Min, double Max);
	LONG GetInvalidBytes(LONG lGroupID);
	BOOL FileDescription(const wchar_t* strHDComment, LONG lTimerQualityClass, LONGLONG start_time_ns, WORD tz_offset_min, WORD dst_offset_min, BYTE time_flags);
	BOOL SetSignalDiscrete(LONG iGroupNo, LONG iSignalNo, LONG bDiscrete, const wchar_t* strN2T);
	BOOL SetTimeStat(LONG iGroupNo, DOUBLE xMean, DOUBLE xMin, DOUBLE xMax, DOUBLE xMinDelta, DOUBLE xMaxDelta);
	BOOL SetRecordValues(LONG iGroupNo, DOUBLE *pValues, LONG nRecords);
	void CreateSRBlocks();
	void SetFileTimeFraction(DOUBLE FileTimeFraction);
	void SetNanoTimeUTC(LONGLONG NanoTimeUTC);
	BOOL WriteRecords(LONG iGroupNo, void *pBuffer, LONG nRecords);

protected:
	HMODULE m_hMod;
	INT_PTR m_hHandle;

	INT_PTR (*m_pfInitDll)(void);
	void (*m_pfExitDll)(INT_PTR hHandle);
	BOOL(*m_pfCreateMDF4)(INT_PTR hHandle, const wchar_t* strToolName, const wchar_t* strToolVendor, const wchar_t* strToolVersion, const wchar_t* strComment, INT_PTR lVersion);
	void(*m_pfSetPathName)(INT_PTR hHandle, const wchar_t* strName);
	void(*m_pfAddGroup)(INT_PTR hHandle, const wchar_t* strComment, DWORD_PTR ExpNValues, INT_PTR* iNo);
	void(*m_pfAddGroup64)(INT_PTR hHandle, const wchar_t* strComment, __int64 ExpNValues, INT_PTR* iNo);
	BOOL(*m_pfAddSignal)(INT_PTR hHandle, LONG iGroupNo, const wchar_t* strName, const wchar_t* strLabel, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE yFactor, DOUBLE yOffset, LONG bHasNovalues, DOUBLE Novalue, LONG invalPos, LONG* iNo);
	BOOL(*m_pfAddTimeInfo)(INT_PTR hHandle, LONG iGroupNo, const wchar_t* strName, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset);
	BOOL(*m_pfAddTimeSignal)(INT_PTR hHandle, LONG iGroupNo, const wchar_t* strName, LONG lDataType, LONG lnFirstBit, LONG lnBits, const wchar_t* strUnit, DOUBLE Factor, DOUBLE Offset, LONG* iNo);
	BOOL(*m_pfMakeGroups)(INT_PTR hHandle);
	BOOL(*m_pfSetSignalValue)(INT_PTR hHandle, LONG iGroupNo, LONG iSignalNo, DOUBLE Value, LONG bIsNovalue);
	BOOL(*m_pfWriteRecord)(INT_PTR hHandle, LONG iGroupNo);
	BOOL(*m_pfFlushGroup)(INT_PTR hHandle, LONG iGroupNo);
	void(*m_pfClose)(INT_PTR hHandle);
	BOOL(*m_pfCreateSRBlock)(INT_PTR hHandle, LONG lGroupNo, DOUBLE dt, DOUBLE xrange, DOUBLE OldXFactor, DOUBLE OldXOffset);
	void(*m_pfSetFileTime)(INT_PTR hHandle, LONGLONG FileTime);
	BOOL(*m_pfGroupDescription)(INT_PTR hHandle, LONG lGroupNo, const wchar_t* strAcqName, const wchar_t* strCGComment, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment);
	BOOL(*m_pfSignalDescription)(INT_PTR hHandle, LONG iGroupNo, LONG iSignalNo, const wchar_t* strSIName, const wchar_t* strSIPath, const wchar_t* strSIComment);
	DWORD(*m_pfGetRecordSize)(INT_PTR hHandle, LONG lGroupID);
	void(*m_pfSetRecord)(INT_PTR hHandle, LONG lGroupNo, BYTE* pBuffer);
	BOOL(*m_pfSetMinMax)(INT_PTR hHandle, LONG iGroupNo, LONG iSignalNo, double Min, double Max);
	LONG(*m_pfGetInvalidBytes)(INT_PTR hHandle, LONG lGroupID);
	BOOL(*m_pfFileDescription)(INT_PTR hHandle, const wchar_t* strHDComment, LONG lTimerQualityClass, LONGLONG start_time_ns, WORD tz_offset_min, WORD dst_offset_min, BYTE time_flags);
	BOOL(*m_pfSetSignalDiscrete)(INT_PTR hHandle, LONG iGroupNo, LONG iSignalNo, LONG bDiscrete, const wchar_t* strN2T);
	BOOL(*m_pfSetTimeStat)(INT_PTR hHandle, LONG iGroupNo, DOUBLE xMean, DOUBLE xMin, DOUBLE xMax, DOUBLE xMinDelta, DOUBLE xMaxDelta);
	BOOL(*m_pfSetRecordValues)(INT_PTR hHandle, LONG iGroupNo, DOUBLE *pValues, LONG nRecords);
	void(*m_pfCreateSRBlocks)(INT_PTR hHandle);
	void(*m_pfSetFileTimeFraction)(INT_PTR hHandle, DOUBLE FileTimeFraction);
	void(*m_pfSetNanoTimeUTC)(INT_PTR hHandle, LONGLONG NanoTimeUTC);
	BOOL(*m_pfWriteRecords)(INT_PTR hHandle, LONG iGroupNo, void *pBuffer, LONG nRecords);
};

