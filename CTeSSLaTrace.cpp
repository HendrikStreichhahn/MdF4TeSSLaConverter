#include "stdafx.h"
#include "CTeSSLaTrace.h"

#include <fstream>
//#include <iostream>
//#include <cstdlib>

CTeSSLaTrace::CTeSSLaTrace()
{

}

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
	for (int i = 0; i < mvStreams.size(); i++)
		for (int j = 0; j < mvStreams[i]->getEntries().size(); j++)
		{
			outputStrm << mvStreams[i]->getEntries()[j]->toString() << std::endl;
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