#pragma once

#include <cstdint>
#include <map>

#include "CTeSSLaStream.h"
#include "CTeSSLaTrace.h"

char nibbleToHexChar(unsigned char byte);
std::string ByteToHexString(unsigned char byte);
std::string uint32_tToHexString(uint32_t val);

class CTeSSLaStreamCANFrame;

class CCANFrame {
public:
	CCANFrame(unsigned char* pFrame, unsigned int frameSize);
	~CCANFrame();
	uint32_t getIdentifier() { return mIdent; };
	unsigned char getDataLength() { return mDataLength; };
	void getDataBytesCopy(unsigned char* buff, long buffSize);
protected:
	uint32_t mIdent;
	unsigned char mDataLength;
	unsigned char* mpDataBytes;
};

class CTeSSLaStreamEventCANFrame : public CTeSSLaStreamEvent
{
public:
	CTeSSLaStreamEventCANFrame(CTeSSLaStreamCANFrame* owner, unsigned long timeStamp, CCANFrame* canFrame);
	std::string toString();
protected:
	CCANFrame* mpCanFrame;
};

class CTeSSLaStreamCANFrame : public CTeSSLaStream
{
public:
	CTeSSLaStreamCANFrame(uint32_t CANIdent);
	uint32_t getCANIdent() { return mCANIdent; };
	void addEntry(CTeSSLaStreamEventCANFrame* event);
protected:
	uint32_t mCANIdent;
};

class CCAN_FRAMETeSSLaStreamSet
{
public:
	CCAN_FRAMETeSSLaStreamSet();
	~CCAN_FRAMETeSSLaStreamSet();
	void insertCANFrame(unsigned long timeStamp, CCANFrame* aFrame);
	void addToTrace(CTeSSLaTrace* trace);
protected:
	std::map<uint32_t, CTeSSLaStreamCANFrame*> mmStreams;
};