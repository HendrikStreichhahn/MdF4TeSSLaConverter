#pragma once

#include "CTeSSLaStream.h"

class CTeSSLaTrace
{
public:
	CTeSSLaTrace();
	void addTrace(CTeSSLaStream* stream) { this->mvStreams.push_back(stream); };
	void printTrace();
protected:
	std::vector<CTeSSLaStream*> mvStreams;
private:
	bool notAllNegative(std::vector<int> currentIndices);

};