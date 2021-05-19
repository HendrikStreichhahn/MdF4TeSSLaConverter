
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
	if (frameSize < 7)
		throw "CCANFrame::CCANFrame! Not enough Bytes!";
	//identifier
	memcpy_s(&this->mIdent, sizeof(this->mIdent), (pFrame + 1), 4);
	//data length
	memcpy_s(&this->mDataLength, sizeof(this->mDataLength), (pFrame + 7), 1);
	if (frameSize < (7 + mDataLength))
		throw "CCANFrame::CCANFrame! Not enough Bytes!";
	//data
	mpDataBytes = (unsigned char*)malloc(this->mDataLength);
	if (mpDataBytes == NULL)
		throw new std::bad_alloc();
	memcpy_s(this->mpDataBytes, mDataLength, pFrame + 8, mDataLength);

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

	std::string dataString = "0x";
	for (int i = 0; i < dataLength; i++)
		dataString += "" + ByteToHexString(buff[i]);


	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + dataString;
}

void CTeSSLaStreamCANFrame::addEntry(CTeSSLaStreamEventCANFrame* event)
{
	insertEntry(event);
}

CTeSSLaStreamCANFrame::CTeSSLaStreamCANFrame(std::string signalName, uint32_t CANIdent)
{
	mName = signalName + "_CAN_" + uint32_tToHexString(CANIdent);
}

CCAN_FRAMETeSSLaStreamSet::CCAN_FRAMETeSSLaStreamSet()
{
//	mmStreams = new std::map<CTeSSLaStreamCANFrame*, uint32_t>();
}


CCAN_FRAMETeSSLaStreamSet::~CCAN_FRAMETeSSLaStreamSet()
{
	//dont free the streams here -> is done in trace
	//for (auto it = mmStreams.begin(); it != mmStreams.end(); it++)
	//	free(it->second);
	//mmStreams.clear();
}

void CCAN_FRAMETeSSLaStreamSet::insertCANFrame(std::string signalName,  unsigned long timeStamp, CCANFrame* aFrame)
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
		CTeSSLaStreamCANFrame* newStream = new CTeSSLaStreamCANFrame(signalName, aFrame->getIdentifier());
		newStream->addEntry(new CTeSSLaStreamEventCANFrame(newStream, timeStamp, aFrame));
		mmStreams.insert({ aFrame->getIdentifier(), newStream });
	}
}

void CCAN_FRAMETeSSLaStreamSet::addToTrace(CTeSSLaTrace* trace)
{
	for (auto it = mmStreams.begin(); it != mmStreams.end(); it++)
		trace->addStream(it->second);
}