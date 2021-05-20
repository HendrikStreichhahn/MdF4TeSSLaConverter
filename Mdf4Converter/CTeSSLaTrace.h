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