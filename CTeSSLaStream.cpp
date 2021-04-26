#pragma once

#include "stdafx.h"
#include "CTeSSLaStream.h"

#include <stdexcept>

CTeSSLaStream::CTeSSLaStream(std::string name)
{
	this->mName = name;
}

void CTeSSLaStream::insertEntry(CTeSSLaStreamEntry* entry)
{
	if (this->mvEntries.size() != 0)
		if (this->mvEntries.back()->getTimeStamp() >= entry->getTimeStamp())
			throw new std::logic_error("Inserted Events in wrong order!");
	this->mvEntries.push_back(entry);
}

void CTeSSLaStreamVoid::addEntry(CTeSSLaStreamEntryVoid* entry)
{ 
	insertEntry(entry);
};

void CTeSSLaStreamInt::addEntry(CTeSSLaStreamEntryInt* entry)
{
	insertEntry(entry);
};

void CTeSSLaStreamFloat::addEntry(CTeSSLaStreamEntryFloat* entry)
{
	insertEntry(entry);
};


unsigned long CTeSSLaStreamEntry::getTimeStamp()
{
	return mTimeStamp;
};

CTeSSLaStreamEntry::CTeSSLaStreamEntry(CTeSSLaStream* owner, unsigned long timeStamp)
{
	this->mTimeStamp = timeStamp;
	this->mOwner = owner;
}

std::string CTeSSLaStreamEntry::toString()
{
	return std::string();
}

std::string CTeSSLaStreamEntryVoid::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = ()";
};

std::string CTeSSLaStreamEntryInt::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + std::to_string(this->getValue());
};

CTeSSLaStreamEntryInt::CTeSSLaStreamEntryInt(CTeSSLaStream* owner, unsigned long timestamp, int value) : CTeSSLaStreamEntry(owner, timestamp)
{ 
	this->mValue = value; 
};

int CTeSSLaStreamEntryInt::getValue() 
{ 
	return mValue; 
};

CTeSSLaStreamEntryFloat::CTeSSLaStreamEntryFloat(CTeSSLaStream* owner, unsigned long timestamp, double value)
	: CTeSSLaStreamEntry(owner, timestamp)
{
	this->mValue = value;
};

std::string CTeSSLaStreamEntryFloat::toString()
{
	return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + std::to_string(this->getValue());
};

double CTeSSLaStreamEntryFloat::getValue() 
{ 
	return mValue; 
};