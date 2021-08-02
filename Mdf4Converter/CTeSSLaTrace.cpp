#pragma once

#include "stdafx.h"
#include "CTeSSLaTrace.h"

#include <fstream>
#include <chrono>
#include <iomanip>

CTeSSLaTrace::CTeSSLaTrace()
{

}

CTeSSLaTrace::~CTeSSLaTrace()
{
	for (CTeSSLaStream* stream : mvStreams)
	{
		delete stream;
	}
		
	mvStreams.resize(0);
}

void CTeSSLaTrace::addStream(CTeSSLaStream* stream) 
{
	this->mvStreams.push_back(stream); 
};

bool CTeSSLaTrace::notAllNegative(std::vector<int> currentIndices)
{
	int vecLength = currentIndices.size();
	for (int i = 0; i < vecLength; i++)
		if (currentIndices[i] >= 0)
			return true;
	return false;

	/*for (int var : currentIndices)
		if (var >= 0)
			return true;
	return false;*/
}

BOOL CTeSSLaTrace::exportTrace(std::string filePath)
{
	std::ofstream outputStrm(filePath, std::ios::binary | std::ios::out);
	if (!outputStrm)
	{
		return false;
	}
	//fill the starting list
	CTeSSLaTraceEventList nextEvents;
	for (int i = 0; i < mvStreams.size(); i++)
		nextEvents.insertEvent(mvStreams[i]->getNextOutputEvent());
	int long ctr = 0;
	while (nextEvents.getHead() != NULL)
	{
		outputStrm << nextEvents.getHead()->getEvent()->toString() << std::endl;
		//std::cout << "ctr: " << ctr << std::endl;
		ctr++;
		nextEvents.updateTraceEventList();
	}
	outputStrm.close();
	return true;
}

void CTeSSLaTrace::printTrace()
{
	int streamCount = mvStreams.size();

	std::vector<int> currentIndices;
	for (int i = 0; i < streamCount; i++)
		currentIndices.push_back(0);

	while (notAllNegative(currentIndices))
	{
		//get earliest stream entry/iterator
		int currentlyEarliestIndex = -1;
		int currentlyEarliestTimeStamp = INT_MAX;
		for (unsigned int i = 0; i < currentIndices.size(); i++)
		{
			CTeSSLaStream* currentStream = mvStreams.at(i);
			if (currentStream->getEntries().at(currentIndices[i])->getTimeStamp() < currentlyEarliestTimeStamp)
			{
				currentlyEarliestTimeStamp = mvStreams.at(i)->getEntries().at(currentIndices[i])->getTimeStamp();
				currentlyEarliestIndex = i;
			}
		}


		//print
		std::cout << mvStreams.at(currentlyEarliestIndex)->getEntries().at(currentIndices[currentlyEarliestIndex])->toString() << std::endl;
		//update
		currentIndices[currentlyEarliestIndex]++;
		if (currentIndices[currentlyEarliestIndex] >= mvStreams.at(currentlyEarliestIndex)->getEntries().size())
			currentIndices[currentlyEarliestIndex] = -1;
	}
}

CTeSSLaTraceEventListElement* CTeSSLaTraceEventList::getHead()
{
	return headEvent;
}

// inserts an event in the list, keeping the chronological order
void CTeSSLaTraceEventList::insertEvent(CTeSSLaStreamEvent* newEvent)
{
	if (newEvent == NULL)
		return;
	// no head event-> new event is new head
	if (headEvent == NULL)
	{
		headEvent = new CTeSSLaTraceEventListElement(newEvent);
		return;
	}
	// head event is later than new Event-> new event is new head
	if (headEvent->getEvent()->getTimeStamp() > newEvent->getTimeStamp())
	{
		CTeSSLaTraceEventListElement* newListElement = new CTeSSLaTraceEventListElement(newEvent);
		newListElement->setNext(headEvent);
		headEvent = newListElement;
		return;
	}
	// no of above cases apply-> standard insert
	insertEvent(headEvent, newEvent);
}

void CTeSSLaTraceEventList::insertEvent(CTeSSLaTraceEventListElement* currentEvent, CTeSSLaStreamEvent* newEvent)
{
	// new event is NULL-> do nothing
	if (newEvent == NULL)
		return;
	// new event must be before 
	// next event is NULL-> append new list item
	if (currentEvent->getNext() == NULL)
	{
		currentEvent->setNext(new CTeSSLaTraceEventListElement(newEvent));
		return;
	}
	// next event must be before the new event-> call subroutine for the next list item
	if (currentEvent->getNext()->getEvent()->getTimeStamp() <= newEvent->getTimeStamp())
		insertEvent(currentEvent->getNext(), newEvent);
	// new event must be between this event and the next event-> insert in between
	else
	{
		CTeSSLaTraceEventListElement* newListElement = new CTeSSLaTraceEventListElement(newEvent);
		newListElement->setNext(currentEvent->getNext());
		currentEvent->setNext(newListElement);
	}
}

// removes the head of the list. Inserts the next event from the stream into the list.
void CTeSSLaTraceEventList::updateTraceEventList()
{
	CTeSSLaStream* headStream = headEvent->getEvent()->getOwner();
	insertEvent(headStream->getNextOutputEvent());
	auto newHead = headEvent->getNext();
	delete headEvent;
	headEvent = newHead;
}

CTeSSLaTraceEventListElement::CTeSSLaTraceEventListElement(CTeSSLaStreamEvent* event)
{
	this->event = event;
}

CTeSSLaStreamEvent* CTeSSLaTraceEventListElement::getEvent()
{
	return event;
}

void CTeSSLaTraceEventListElement::setNext(CTeSSLaTraceEventListElement* next)
{
	nextEvent = next;
}

CTeSSLaTraceEventListElement* CTeSSLaTraceEventListElement::getNext()
{
	return nextEvent;
}