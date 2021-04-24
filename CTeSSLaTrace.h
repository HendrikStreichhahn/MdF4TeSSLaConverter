#pragma once

#include "CTeSSLaStream.h"

class CTeSSLaTrace
{
public:
	CTeSSLaTrace();
	void addStream(CTeSSLaStream* stream) { this->mvStreams.push_back(stream); };
	void printTrace();
	BOOL exportTrace(std::string filePath);
protected:
	std::vector<CTeSSLaStream*> mvStreams;
private:
	bool notAllNegative(std::vector<int> currentIndices);

};