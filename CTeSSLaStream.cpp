#include "stdafx.h"
#include "CTeSSLaStream.h"

CTeSSLaStream::CTeSSLaStream(std::string name)
{
	this->mName = name;
}

CTeSSLaStreamEntry::CTeSSLaStreamEntry(CTeSSLaStream* owner, int timeStamp)
{
	this->mTimeStamp = timeStamp;
	this->mOwner = owner;
}

std::string CTeSSLaStreamEntryVoid::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + "= ()";
};

std::string CTeSSLaStreamEntryInt::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + "=" + std::to_string(this->getValue());
};

std::string CTeSSLaStreamEntryFloat::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + "=" + std::to_string(this->getValue());
};