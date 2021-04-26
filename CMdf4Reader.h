// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#pragma once

#import "C:\\Users\\Hendrik Streichhahn\\Documents\\uni\\HiWi\\mdf4reader\\MDF4Reader.dll" no_namespace
// CMdf4Reader wrapper class

class CMdf4Reader : public COleDispatchDriver
{
public:
	CMdf4Reader() {} // Calls COleDispatchDriver default constructor
	CMdf4Reader(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMdf4Reader(const CMdf4Reader& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IMdf4Reader methods
public:
	void OpenMDF4(LPCTSTR strPathName)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strPathName);
	}
	long get_FileTime()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	double get_TimeFraction()
	{
		double result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	long get_NGroups()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void GetGroupName(BSTR * strGroupName, long iGroupIndex)
	{
		static BYTE parms[] = VTS_PBSTR VTS_I4;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strGroupName, iGroupIndex);
	}
	void LoadGroup(long iGroupIndex)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, iGroupIndex);
	}
	long get_NSignals()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	long get_NSamples()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	double get_FirstTimestamp()
	{
		double result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	double get_LastTimestamp()
	{
		double result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	void TimeToIndex(double TimeStamp, long lStartIndex, long * lIndex)
	{
		static BYTE parms[] = VTS_R8 VTS_I4 VTS_PI4;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, TimeStamp, lStartIndex, lIndex);
	}
	void LoadTimeSignal(BSTR * strTimeName)
	{
		static BYTE parms[] = VTS_PBSTR;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strTimeName);
	}
	void LoadSignal(BSTR * strName, long iSignalIndex)
	{
		static BYTE parms[] = VTS_PBSTR VTS_I4;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strName, iSignalIndex);
	}
	void GetTimeSignal(BSTR * strUnit, long * monotony, double * rmin, double * rmax, double * raster, long * bIsVirtual)
	{
		static BYTE parms[] = VTS_PBSTR VTS_PI4 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PI4;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strUnit, monotony, rmin, rmax, raster, bIsVirtual);
	}
	void GetSignal(BSTR * strDisplayName, BSTR * strAliasName, BSTR * strUnit, BSTR * strComment, long * discrete)
	{
		static BYTE parms[] = VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PI4;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, strDisplayName, strAliasName, strUnit, strComment, discrete);
	}
	void GetData(long bTime, long lFirstIndex, long lLastIndex, VARIANT * pBuffer, long * nValuesRead)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT VTS_PI4;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, bTime, lFirstIndex, lLastIndex, pBuffer, nValuesRead);
	}
	void CacheData(long bTime, long lFirstIndex, long lLastIndex, long * nValuesCached)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, bTime, lFirstIndex, lLastIndex, nValuesCached);
	}
	double get_CachedValue(long bTime, long lIndex)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, bTime, lIndex);
		return result;
	}
	CString get_Comment(long nBlock, long nElement)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, nBlock, nElement);
		return result;
	}
	long get_NoOfSRBlocks()
	{
		long result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	double get_SRdt(long nBlock)
	{
		double result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, nBlock);
		return result;
	}
	long get_SRCycleCount(long nBlock)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, nBlock);
		return result;
	}
	void CacheSRData(long nBlock, long bTime, long lFirstIndex, long lLastIndex, long * nValuesCached)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, nBlock, bTime, lFirstIndex, lLastIndex, nValuesCached);
	}
	void CachedSRValues(long bTime, long lIndex, double * Min, double * Max, double * Mean)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 VTS_PR8 VTS_PR8;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, bTime, lIndex, Min, Max, Mean);
	}
	unsigned __int64 get_NSamples64()
	{
		unsigned __int64 result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_UI8, (void*)&result, nullptr);
		return result;
	}
	unsigned __int64 get_MDF4File()
	{
		unsigned __int64 result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_UI8, (void*)&result, nullptr);
		return result;
	}
	void GetSignalDetail(long * lDataType, long * nFirstBit, long * nBits, double * Factor, double * Offset, double * RawMin, double * RawMax, long * bHasNoValues, long * invalPos)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PI4 VTS_PI4;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, lDataType, nFirstBit, nBits, Factor, Offset, RawMin, RawMax, bHasNoValues, invalPos);
	}
	long get_RecordSize()
	{
		long result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void GetRecord(__int64 i64StartIndex, __int64 i64EndIndex, unsigned char * pBuffer)
	{
		static BYTE parms[] = VTS_I8 VTS_I8 VTS_PUI1;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, i64StartIndex, i64EndIndex, pBuffer);
	}
	long get_InvalidBytes()
	{
		long result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	long get_TimerQualityClass()
	{
		long result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	long get_Version()
	{
		long result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	__int64 get_NanoTimeUTC()
	{
		__int64 result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I8, (void*)&result, nullptr);
		return result;
	}
	long get_tz_offset_min()
	{
		long result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	long get_dst_offset_min()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	long get_time_flags()
	{
		long result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void GetDataEx(long bTime, long lFirstIndex, long lLastIndex, VARIANT * pBuffer, long * nValuesRead)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT VTS_PI4;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, bTime, lFirstIndex, lLastIndex, pBuffer, nValuesRead);
	}

	// IMdf4Reader properties
public:

};
