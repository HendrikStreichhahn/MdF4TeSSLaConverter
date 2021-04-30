//
// C Interface for MDF4Reader.dll
//
	INT_PTR M4RDInitDll(void);
	void M4RDExitDll(void);
	BOOL M4RDOpenMDF4(const wchar_t* strPathName);
	long M4RDGetFileTime();
	double M4RDGetTimeFraction();
	__int64 M4RDGetNanoTimeUTC();
	long M4RDGetNGroups();
	const wchar_t* M4RDGetGroupName(long iGroupIndex);
	BOOL M4RDLoadGroup(long iGroupIndex);
	long M4RDGetNSignals();
	unsigned __int64 M4RDGetNSamples();
	double M4RDGetFirstTimestamp();
	double M4RDGetLastTimestamp();
	__int64 M4RDTimeToIndex(double TimeStamp, long lStartIndex);
	const wchar_t* M4RDLoadTimeSignal();
	const wchar_t* M4RDLoadSignal(long iSignalIndex);
	BOOL M4RDGetTimeSignal(wchar_t* pszUnit, long* monotony, double* rmin, double* rmax, double* raster, long* bIsVirtual);
	BOOL M4RDGetSignal(wchar_t* pszDisplayName, wchar_t* pszAliasName, wchar_t* pszUnit, wchar_t* pszComment, long* discrete);
	__int64 M4RDGetData(BOOL bTime, long lFirstIndex, long lLastIndex, double* pBuffer);
	long M4RDCacheData(BOOL bTime, long lFirstIndex, long lLastIndex);
	double M4RDGetCachedValue(BOOL bTime, long lIndex);
	const wchar_t* M4RDGetComment(long nBlock, long nElement);
	long M4RDGetNoOfSRBlocks();
	double M4RDGetSRdt(long nBlock);
	long M4RDGetSRCycleCount(long nBlock);
	long M4RDCacheSRData(long nBlock, BOOL bTime, long lFirstIndex, long lLastIndex);
	BOOL M4RDGetCachedSRValues(BOOL bTime, long lIndex, double* Min, double* Max, double* Mean);
	UINT_PTR M4RDGetMDF4File();
	BOOL M4RDGetSignalDetail(long* lDataType, long* nFirstBit, long* nBits, double* Factor, double* Offset, double* RawMin, double* RawMax, long* bHasNoValues, long* invalPos);
	long M4RDGetRecordSize();
	BOOL M4RDGetRecord(__int64 i64StartIndex, __int64 i64EndIndex, BYTE* pBuffer);
	long M4RDGetInvalidBytes();
	long M4RDGetTimerQualityClass();
	long M4RDGetVersion();
	long M4RDget_tz_offset_min();
	long M4RDget_dst_offset_min();
	long M4RDget_time_flags();
	__int64 M4RDGetDataEx(BOOL bTime, long lFirstIndex, long lLastIndex, double* pBuffer);

