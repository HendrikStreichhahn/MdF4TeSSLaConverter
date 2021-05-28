#pragma once

#include "pch.h"
#include "CppUnitTest.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TeSSLaStreamTest
{
	TEST_CLASS(TeSSLaStreamTest)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			int i1 = 1;
			int i2 = 1;
			Assert::AreEqual(i1, i2);
		}
		TEST_METHOD(TestStreamEventVoidToString)
		{
			//create stream and entries
			CTeSSLaStreamVoid* aStream = new CTeSSLaStreamVoid("testStreamVoid");
			static unsigned int ENTRY_COUNT = 1000;
			std::vector<std::string> expectedStrings;
			for (unsigned int i = 0; i < ENTRY_COUNT; i++) {
				char buffer[33];
				_itoa_s(i, buffer, 10);
				buffer[32] = 0;
				expectedStrings.push_back(std::string(buffer) + ": " + aStream->getName() + " = " + "()");
				aStream->addEntry(new CTeSSLaStreamEventVoid(aStream, i));
			}
			//check toString method
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				Assert::AreEqual(0, aStream->getEntries()[i]->toString().compare(expectedStrings[i]));
			}
			//clean up
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
				free(aStream->getEntries()[i]);
		}

		TEST_METHOD(TestStreamEventIntToString)
		{
			//create stream and entries
			CTeSSLaStreamInt* aStream = new CTeSSLaStreamInt("testStreamInt");
			static unsigned int ENTRY_COUNT = 1000;
			std::vector<std::string> expectedStrings;
			for (unsigned int i = 0; i < ENTRY_COUNT; i++) {
				char buffer[33];
				_itoa_s(i, buffer, 10);
				buffer[32] = 0;
				expectedStrings.push_back(std::string(buffer) + ": " + aStream->getName() + " = " + std::string(buffer));
				aStream->addEntry(new CTeSSLaStreamEventInt(aStream, i, i));
			}
			//check toString method
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				Assert::AreEqual(0, aStream->getEntries()[i]->toString().compare(expectedStrings[i]));
			}
			//clean up
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
				free(aStream->getEntries()[i]);
		}

		TEST_METHOD(TestStreamEventFloatToString)
		{
			//create stream and entries
			CTeSSLaStreamFloat* aStream = new CTeSSLaStreamFloat("testStreamFloat");
			static unsigned int ENTRY_COUNT = 1000;
			std::vector<std::string> expectedStrings;
			for (unsigned int i = 0; i < ENTRY_COUNT; i++) {
				char buffer[33];
				_itoa_s(i, buffer, 10);
				buffer[32] = 0;
				//make expected string
				//std::ostringstream ss;
				//ss << i << ": " << aStream->getName() << " = " << 3.1415927*i;
				//expectedStrings.push_back(std::string(ss.str()));
				expectedStrings.push_back(std::to_string(i) + ": " + aStream->getName() + " = " + std::to_string(3.1415927 * i));
				aStream->addEntry(new CTeSSLaStreamEventFloat(aStream, i, 3.1415927*i));
			}
			//check toString method
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				std::string eventString = aStream->getEntries()[i]->toString();
				Assert::AreEqual(0, eventString.compare(expectedStrings[i]));
			}
			//clean up
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
				free(aStream->getEntries()[i]);
		}


	};
}
