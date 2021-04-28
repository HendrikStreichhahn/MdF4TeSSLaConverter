#pragma once

#include "stdafx.h"
#include "CTeSSLaStream.h"

#include <stdexcept>

CTeSSLaStream::CTeSSLaStream(std::string name)
{
	this->mName = name;
}

void CTeSSLaStream::insertEntry(CTeSSLaStreamEvent* entry)
{
	if (this->mvEntries.size() != 0)
		if (this->mvEntries.back()->getTimeStamp() >= entry->getTimeStamp())
			throw new std::logic_error("Inserted Events in wrong order!");
	this->mvEntries.push_back(entry);
}

void CTeSSLaStreamVoid::addEntry(CTeSSLaStreamEventVoid* entry)
{ 
	insertEntry(entry);
};

void CTeSSLaStreamInt::addEntry(CTeSSLaStreamEventInt* entry)
{
	insertEntry(entry);
};

void CTeSSLaStreamFloat::addEntry(CTeSSLaStreamEventFloat* entry)
{
	insertEntry(entry);
};


unsigned long CTeSSLaStreamEvent::getTimeStamp()
{
	return mTimeStamp;
};

CTeSSLaStreamEvent::CTeSSLaStreamEvent(CTeSSLaStream* owner, unsigned long timeStamp)
{
	this->mTimeStamp = timeStamp;
	this->mOwner = owner;
}

std::string CTeSSLaStreamEvent::toString()
{
	return std::string();
}

std::string CTeSSLaStreamEventVoid::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = ()";
};

std::string CTeSSLaStreamEventInt::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + std::to_string(this->getValue());
};

CTeSSLaStreamEventInt::CTeSSLaStreamEventInt(CTeSSLaStream* owner, unsigned long timestamp, int value) : CTeSSLaStreamEvent(owner, timestamp)
{ 
	this->mValue = value; 
};

int CTeSSLaStreamEventInt::getValue() 
{ 
	return mValue; 
};

CTeSSLaStreamEventFloat::CTeSSLaStreamEventFloat(CTeSSLaStream* owner, unsigned long timestamp, double value)
	: CTeSSLaStreamEvent(owner, timestamp)
{
	this->mValue = value;
};

std::string CTeSSLaStreamEventFloat::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + std::to_string(this->getValue());
};

double CTeSSLaStreamEventFloat::getValue() 
{ 
	return mValue; 
};