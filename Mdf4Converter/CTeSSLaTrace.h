#pragma once

#include "CTeSSLaStream.h"
#include <string>

class CTeSSLaTrace
{
public:
	CTeSSLaTrace();
	~CTeSSLaTrace();
	void addStream(CTeSSLaStream* stream);
	void printTrace();
	BOOL exportTrace(std::string filePath);
protected:
	std::vector<CTeSSLaStream*> mvStreams;
private:
	bool notAllNegative(std::vector<int> currentIndices);
};

class CTeSSLaTraceEventListElement;

class CTeSSLaTraceEventList
{
public:
	CTeSSLaTraceEventListElement* getHead();
	// inserts an event in the list, keeping the chronological order
	void insertEvent(CTeSSLaStreamEvent* newEvent);
	// removes the head of the list. Inserts the next event from the stream into the list.
	void updateTraceEventList();
protected:
	CTeSSLaTraceEventListElement* headEvent = NULL;
	void insertEvent(CTeSSLaTraceEventListElement* currentEvent,  CTeSSLaStreamEvent* newEvent);
};

// Linked list of TeSSLa Events in chronological order
class CTeSSLaTraceEventListElement
{
public:
	CTeSSLaTraceEventListElement(CTeSSLaStreamEvent* event);

	// getter for the event
	CTeSSLaStreamEvent* getEvent();

	// setter and getter for the next element in the list
	void setNext(CTeSSLaTraceEventListElement* next);
	CTeSSLaTraceEventListElement* getNext();
	
protected:
	CTeSSLaTraceEventListElement* nextEvent = NULL;
	CTeSSLaStreamEvent* event = NULL;
};