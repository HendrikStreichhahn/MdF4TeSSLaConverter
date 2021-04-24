#pragma once

#include <vector>
#include <string>

class CTeSSLaStream;

class CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntry(CTeSSLaStream* owner, int timeStamp);
	~CTeSSLaStreamEntry() {};
	int getTimeStamp() { return mTimeStamp; };
	std::string toString() { return ""; };
protected:
	int mTimeStamp;
	CTeSSLaStream* mOwner;
};

class CTeSSLaStreamEntryVoid : public CTeSSLaStreamEntry
{
public:
	std::string toString();
protected:
};

class CTeSSLaStreamEntryInt : public CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntryInt(CTeSSLaStream* owner, int timestamp, int value) : CTeSSLaStreamEntry(owner, timestamp) { this->mValue = value; };
	std::string toString();
	int getValue() { return mValue; };
protected:
	int mValue;
};

class CTeSSLaStreamEntryFloat : public CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntryFloat(CTeSSLaStream* owner, int timestamp, double value) : CTeSSLaStreamEntry(owner, timestamp) { this->mValue = value; };
	std::string toString();
	double getValue() { return mValue; };
protected:
	double mValue;
};

class CTeSSLaStream
{
public:
	CTeSSLaStream(std::string name);
	std::string getName() { return mName; };
	std::vector<CTeSSLaStreamEntry*> getEntries() { return mvEntries; };
protected:
	std::string mName;
	std::vector<CTeSSLaStreamEntry*> mvEntries;
};

class CTeSSLaStreamVoid : public CTeSSLaStream
{
public:
	void addEntry(CTeSSLaStreamEntryVoid* entry) { this->mvEntries.push_back(entry); };
};

class CTeSSLaStreamInt : public CTeSSLaStream
{
public:
	void addEntry(CTeSSLaStreamEntryInt* entry) { this->mvEntries.push_back(entry); };
};

class CTeSSLaStreamFloat : public CTeSSLaStream
{
public:
	void addEntry(CTeSSLaStreamEntryFloat* entry) { this->mvEntries.push_back(entry); };
};