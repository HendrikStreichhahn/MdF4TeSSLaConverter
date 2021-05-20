#pragma once

#include <vector>
#include <string>

class CTeSSLaStream;

class CTeSSLaStreamEvent
{
public:
	CTeSSLaStreamEvent(CTeSSLaStream* owner, unsigned long timeStamp);
	virtual ~CTeSSLaStreamEvent() {};
	unsigned long getTimeStamp();
	virtual std::string toString();
protected:
	unsigned long mTimeStamp;
	CTeSSLaStream* mOwner;
};

class CTeSSLaStreamEventVoid : public CTeSSLaStreamEvent
{
public:
	CTeSSLaStreamEventVoid(CTeSSLaStream* owner, unsigned long timeStamp) : CTeSSLaStreamEvent(owner, timeStamp) {};
	std::string toString();
protected:
};

class CTeSSLaStreamEventInt : public CTeSSLaStreamEvent
{
public:
	CTeSSLaStreamEventInt(CTeSSLaStream* owner, unsigned long timestamp, long value);
	std::string toString();
	long getValue();
protected:
	long mValue;
};

class CTeSSLaStreamEventFloat : public CTeSSLaStreamEvent
{
public:
	CTeSSLaStreamEventFloat(CTeSSLaStream* owner, unsigned long timestamp, double value);
	std::string toString();
	double getValue();
protected:
	double mValue;
};

class CTeSSLaStream
{
public:
	CTeSSLaStream(std::string name);
	virtual ~CTeSSLaStream();
	std::string getName() { return mName; };
	std::vector<CTeSSLaStreamEvent*> getEntries() { return mvEntries; };
protected:
	CTeSSLaStream() {};
	std::string mName;
	std::vector<CTeSSLaStreamEvent*> mvEntries;
	void insertEntry(CTeSSLaStreamEvent* entry);
};

class CTeSSLaStreamVoid : public CTeSSLaStream
{
public:
	CTeSSLaStreamVoid(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEventVoid* entry);
};

class CTeSSLaStreamInt : public CTeSSLaStream
{
public:
	CTeSSLaStreamInt(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEventInt* entry);
};

class CTeSSLaStreamFloat : public CTeSSLaStream
{
public:
	CTeSSLaStreamFloat(std::string name) : CTeSSLaStream(name) {};
	void addEntry(CTeSSLaStreamEventFloat* entry);
};