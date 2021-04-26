#pragma once

#include <vector>
#include <string>

class CTeSSLaStream;

class CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntry(CTeSSLaStream* owner, unsigned long timeStamp);
	~CTeSSLaStreamEntry() {};
	unsigned long getTimeStamp();
	virtual std::string toString();
protected:
	unsigned long mTimeStamp;
	CTeSSLaStream* mOwner;
};

class CTeSSLaStreamEntryVoid : public CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntryVoid(CTeSSLaStream* owner, unsigned long timeStamp) : CTeSSLaStreamEntry(owner, timeStamp) {};
	std::string toString();
protected:
};

class CTeSSLaStreamEntryInt : public CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntryInt(CTeSSLaStream* owner, unsigned long timestamp, int value);
	std::string toString();
	int getValue();
protected:
	int mValue;
};

class CTeSSLaStreamEntryFloat : public CTeSSLaStreamEntry
{
public:
	CTeSSLaStreamEntryFloat(CTeSSLaStream* owner, unsigned long timestamp, double value);
	std::string toString();
	double getValue();
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
	void insertEntry(CTeSSLaStreamEntry* entry);
};

class CTeSSLaStreamVoid : public CTeSSLaStream
{
public:
	CTeSSLaStreamVoid(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEntryVoid* entry);
};

class CTeSSLaStreamInt : public CTeSSLaStream
{
public:
	CTeSSLaStreamInt(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEntryInt* entry);
};

class CTeSSLaStreamFloat : public CTeSSLaStream
{
public:
	CTeSSLaStreamFloat(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEntryFloat* entry);
};