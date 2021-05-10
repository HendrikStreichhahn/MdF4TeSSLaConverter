#pragma once

#include "stdafx.h"
#include "CTeSSLaStream.h"

#include <stdexcept>
#include <sstream>
#include <limits>

typedef std::numeric_limits< double > dbl;

CTeSSLaStream::CTeSSLaStream(std::string name)
{
	this->mName = name;
}

void CTeSSLaStream::insertEntry(CTeSSLaStreamEvent* entry)
{
	if (this->mvEntries.size() != 0)
		if (this->mvEntries.back()->getTimeStamp() >= entry->getTimeStamp())
		{
			if (this->mvEntries.back()->getTimeStamp() == entry->getTimeStamp())
				std::cout << "Two events with the same timestamp(" << entry->getTimeStamp() << ") in " << this->getName() << std::endl;
			else
			{
				std::cout << "Error, Events not in order!" << std::endl;
				throw new std::logic_error("Inserted Events not in order!");
			}
			
		}
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
	std::ostringstream ss;
	//ss.precision(dbl::max_digits10);
	ss << (this->getTimeStamp()) << ": " << mOwner->getName() << " = " << this->getValue();
	return std::string(ss.str());
	//return std::to_string(this->getTimeStamp()) + ": " + mOwner->getName() + " = " + std::to_string(this->getValue());
};

double CTeSSLaStreamEventFloat::getValue() 
{ 
	return mValue; 
};