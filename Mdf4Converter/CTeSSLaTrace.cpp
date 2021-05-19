#pragma once

#include "stdafx.h"
#include "CTeSSLaTrace.h"

#include <fstream>
//#include <iostream>
//#include <cstdlib>

CTeSSLaTrace::CTeSSLaTrace()
{

}

void CTeSSLaTrace::addStream(CTeSSLaStream* stream) 
{
	this->mvStreams.push_back(stream); 
};

bool CTeSSLaTrace::notAllNegative(std::vector<int> currentIndices)
{
	for (int var : currentIndices)
		if (var >= 0)
			return true;
	return false;
}

BOOL CTeSSLaTrace::exportTrace(std::string filePath)
{
	std::ofstream outputStrm(filePath, std::ios::binary|std::ios::out);
	if (!outputStrm)
	{
		return false;
	}

	//vector of current indices in the streams
	std::vector<int> currentIndices;
	for (int i = 0; i < mvStreams.size(); i++)
		currentIndices.push_back(0);

	//output events as long as they are not all included
	while (notAllNegative(currentIndices))
	{
		//get earliest stream entry index
		int currentlyEarliestIndex = -1;
		int currentlyEarliestTimeStamp = INT_MAX;
		for (unsigned int i = 0; i < currentIndices.size(); i++)
		{
			CTeSSLaStream* currStrm = mvStreams.at(i);
			if (currentIndices[i] < 0 || currentIndices[i] >= currStrm->getEntries().size())
				continue;
			if (currStrm->getEntries().at(currentIndices[i])->getTimeStamp() < currentlyEarliestTimeStamp)
			{
				currentlyEarliestTimeStamp = mvStreams.at(i)->getEntries().at(currentIndices[i])->getTimeStamp();
				currentlyEarliestIndex = i;
			}
		}
		//print event  to file
		outputStrm << mvStreams.at(currentlyEarliestIndex)->getEntries().at(currentIndices[currentlyEarliestIndex])->toString() << std::endl;
		//update index
		currentIndices[currentlyEarliestIndex]++;
		if (currentIndices[currentlyEarliestIndex] >= mvStreams.at(currentlyEarliestIndex)->getEntries().size())
			currentIndices[currentlyEarliestIndex] = -1;
	}

	outputStrm.close();
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