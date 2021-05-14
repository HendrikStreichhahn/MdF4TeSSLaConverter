
#include "stdafx.h"

#include "CTeSSLaCANStream.h"
#include <cstring>
#include <cstdlib>
#include <new>
#include <sstream>

char nibbleToHexChar(unsigned char byte)
{
	if (byte >= 0 && byte <= 9)
		return '0' + byte;
	else
		return 'A' + (byte-0xA);
}

std::string ByteToHexString(unsigned char byte)
{
	char bytes[3];
	bytes[0] = nibbleToHexChar((byte & 0xF0) >> 4);
	bytes[1] = nibbleToHexChar(byte & 0x0F);
	bytes[2] = 0;

	std::string res = std::string(bytes) ;
	return res;
}

std::string uint32_tToHexString(uint32_t val)
{
	std::string res = "";
	res += ByteToHexString((val & 0xFF000000) >> 24);
	res += ByteToHexString((val & 0xFF0000) >> 16);
	res += ByteToHexString((val & 0xFF00) >> 8);
	res += ByteToHexString(val & 0xFF);

	return res;
}

CCANFrame::CCANFrame(unsigned char* pFrame, unsigned int frameSize)
{
	switch (frameSize)
	{
	case (27):
		memcpy_s(&this->mIdent, sizeof(this->mIdent), (pFrame + 9), 4);
		memcpy_s(&this->mDataLength, sizeof(this->mDataLength), (pFrame + 15), 1);
		mpDataBytes = (unsigned char*)malloc(this->mDataLength);
		if (mpDataBytes == NULL)
			throw new std::bad_alloc();
		memcpy_s(this->mpDataBytes, mDataLength, pFrame + 16, mDataLength);
		break;
	default:
		throw "Unsupported CANFrame Size! Only CAN 2.0B Message Frames (not extended) supported yet!";
	}
}

CCANFrame::~CCANFrame()
{
	free(mpDataBytes);
}

void CCANFrame::getDataBytesCopy(unsigned char* buff, long buffSize)
{
	memcpy_s(buff, buffSize, mpDataBytes, mDataLength);
}

CTeSSLaStreamEventCANFrame::CTeSSLaStreamEventCANFrame(CTeSSLaStreamCANFrame* owner, unsigned long timeStamp, CCANFrame* canFrame) : CTeSSLaStreamEvent(owner, timeStamp)
{
	this->mpCanFrame = canFrame;
}

std::string CTeSSLaStreamEventCANFrame::toString()
{
	std::stringstream aStream;

	unsigned int dataLength = mpCanFrame->getDataLength();
	unsigned char* buff = (unsigned char*)malloc(dataLength);
	
	if (buff == NULL)
		return "";

	mpCanFrame->getDataBytesCopy(buff, dataLength);

	std::string dataString = "";
	for (int i = 0; i < dataLength; i++)
		dataString += " " + ByteToHexString(buff[i]);


	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " =" + dataString;
}

void CTeSSLaStreamCANFrame::addEntry(CTeSSLaStreamEventCANFrame* event)
{
	insertEntry(event);
}

CTeSSLaStreamCANFrame::CTeSSLaStreamCANFrame(uint32_t CANIdent)
{
	mName = "CAN_" + uint32_tToHexString(CANIdent);
}

CCAN_FRAMETeSSLaStreamSet::CCAN_FRAMETeSSLaStreamSet()
{
//	mmStreams = new std::map<CTeSSLaStreamCANFrame*, uint32_t>();
}


CCAN_FRAMETeSSLaStreamSet::~CCAN_FRAMETeSSLaStreamSet()
{
	for (auto it = mmStreams.begin(); it != mmStreams.end(); it++)
		free(it->second);
	mmStreams.clear();
}

void CCAN_FRAMETeSSLaStreamSet::insertCANFrame(unsigned long timeStamp, CCANFrame* aFrame)
{
	//check, if stream with same identifier exists
	auto stream = mmStreams.find(aFrame->getIdentifier());
	if (stream != mmStreams.end())
	{
		stream->second->addEntry(new CTeSSLaStreamEventCANFrame(stream->second, timeStamp, aFrame));
	}
	else
	{
		//create new stream
		CTeSSLaStreamCANFrame* newStream = new CTeSSLaStreamCANFrame(aFrame->getIdentifier());
		newStream->addEntry(new CTeSSLaStreamEventCANFrame(newStream, timeStamp, aFrame));
		mmStreams.insert({ aFrame->getIdentifier(), newStream });
	}
}

void CCAN_FRAMETeSSLaStreamSet::addToTrace(CTeSSLaTrace* trace)
{
	for (auto it = mmStreams.begin(); it != mmStreams.end(); it++)
		trace->addStream(it->second);
}