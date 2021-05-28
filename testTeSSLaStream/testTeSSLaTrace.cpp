#pragma once

#include "pch.h"
#include "CppUnitTest.h"

#include <fstream>
#include <list>

#include "../Mdf4Converter/CTeSSLaTrace.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TeSSLaTraceTest
{
	TEST_CLASS(TeSSLaTraceTest)
	{
	public:

		//creates two streams and inserts them in order
		TEST_METHOD(TestExportMultipleStreams1)
		{
			//create streams and insert events
			CTeSSLaStreamFloat* streamA = new CTeSSLaStreamFloat("stimulus");
			CTeSSLaStreamFloat* streamB = new CTeSSLaStreamFloat("response");

			std::list<std::string> expectedLines;

			static unsigned int ENTRY_COUNT = 100;
			static unsigned int TIME_DIST_A = 25;
			static unsigned int TIME_DIST_B = 5;

			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				streamA->addEntry(new CTeSSLaStreamEventFloat(streamA, i * TIME_DIST_A, 3.77845));
				streamB->addEntry(new CTeSSLaStreamEventFloat(streamB, i * TIME_DIST_A + TIME_DIST_B, 5.84561));
				char buffer[11];
				_itoa_s(i * TIME_DIST_A, buffer, 10);
				buffer[10] = 0;
				expectedLines.push_back(std::string(buffer) + ": " + streamA->getName() + " = " + std::to_string(3.77845));
				_itoa_s(i * TIME_DIST_A + TIME_DIST_B, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamB->getName() + " = " + std::to_string(5.845610));
			}

			// create trace, insert streams and export
			static std::string FILE_PATH = "test.trace";

			CTeSSLaTrace* trace = new CTeSSLaTrace();
			trace->addStream(streamA);
			trace->addStream(streamB);

			trace->exportTrace(FILE_PATH);

			// load file and compare
			std::ifstream inputFile(FILE_PATH, std::ios::binary | std::ios::in);
			Assert::IsTrue(inputFile.is_open());

			int lastTimeStamp = 0;

			for (std::string line; std::getline(inputFile, line); ) 
			{
				//check timestamp
				int timestamp = atoi(line.substr(0,line.find(':')).c_str());
				Assert::IsTrue(timestamp >= lastTimeStamp);
				lastTimeStamp = timestamp;
				//search line in expectedLines and remove
				BOOL foundLine = false;
				for (auto it = expectedLines.begin(); !foundLine && it != expectedLines.end(); )
				{
					if ((*it).compare(line) == 0)
					{
						expectedLines.erase(it);
						foundLine = true;
					}
					else
					{
						it++;
					}
				}
				//Line must be found
				Assert::IsTrue(foundLine);
			}
			//all expected lines must be found (and therefore removed)
			unsigned int remainingLines = expectedLines.size();
			Assert::IsTrue(remainingLines == 0);
		}

		//creates two streams and inserts them out of order
		TEST_METHOD(TestExportMultipleStreams2)
		{
			//create streams and insert events
			CTeSSLaStreamFloat* streamA = new CTeSSLaStreamFloat("streamA");
			CTeSSLaStreamFloat* streamB = new CTeSSLaStreamFloat("streamB");
			CTeSSLaStreamFloat* streamC = new CTeSSLaStreamFloat("streamC");

			std::list<std::string> expectedLines;

			static unsigned int ENTRY_COUNT = 100;
			static unsigned int TIME_DIST_A = 1;
			static unsigned int TIME_DIST_B = 3;
			static unsigned int TIME_DIST_C = 10;

			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				streamA->addEntry(new CTeSSLaStreamEventFloat(streamA, i * TIME_DIST_A, 3.77845));
				streamB->addEntry(new CTeSSLaStreamEventFloat(streamB, i * TIME_DIST_B, 5.84561));
				streamC->addEntry(new CTeSSLaStreamEventFloat(streamC, i * TIME_DIST_C, 2.71828));
			}
			unsigned int insertedLinesA = 0, insertedLinesB = 0, insertedLinesC = 0;

			char buffer[11];
			buffer[10] = 0;
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i* TIME_DIST_A, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamA->getName() + " = " + std::to_string(3.77845));
				insertedLinesA++;
			}
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i* TIME_DIST_B, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamB->getName() + " = " + std::to_string(5.84561));
				insertedLinesB++;
			}
			for (unsigned int i = 0; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i* TIME_DIST_C, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamC->getName() + " = " + std::to_string(2.71828));
				insertedLinesC++;
			}

			// create trace, insert streams and export
			static std::string FILE_PATH = "test.trace";

			CTeSSLaTrace* trace = new CTeSSLaTrace();
			trace->addStream(streamA);
			trace->addStream(streamB);
			trace->addStream(streamC);

			trace->exportTrace(FILE_PATH);

			// load file and compare
			std::ifstream inputFile(FILE_PATH, std::ios::binary | std::ios::in);
			Assert::IsTrue(inputFile.is_open());

			int lastTimeStamp = 0;

			for (std::string line; std::getline(inputFile, line); )
			{
				//check timestamp
				int timestamp = atoi(line.substr(0, line.find(':')).c_str());
				Assert::IsTrue(timestamp >= lastTimeStamp);
				lastTimeStamp = timestamp;
				//search line in expectedLines and remove
				BOOL foundLine = false;
				for (auto it = expectedLines.begin(); !foundLine && it != expectedLines.end(); )
				{
					if ((*it).compare(line) == 0)
					{
						expectedLines.erase(it);
						foundLine = true;
					}
					else
					{
						it++;
					}
				}
				//Line must be found
				Assert::IsTrue(foundLine);
			}
			//all expected lines must be found (and therefore removed)
			unsigned int remainingLines = expectedLines.size();
			Assert::IsTrue(remainingLines == 0);
		}

		TEST_METHOD(TestExportMultipleStreams3)
		{
			//create streams and insert events
			CTeSSLaStreamFloat* streamA = new CTeSSLaStreamFloat("streamA");
			CTeSSLaStreamFloat* streamB = new CTeSSLaStreamFloat("streamB");
			CTeSSLaStreamFloat* streamC = new CTeSSLaStreamFloat("streamC");

			std::list<std::string> expectedLines;

			static unsigned int ENTRY_COUNT = 100;
			static unsigned int TIME_DIST_A = 1;
			static unsigned int TIME_DIST_B = 3;
			static unsigned int TIME_DIST_C = 10;

			for (unsigned int i = 1; i < ENTRY_COUNT; i++)
			{
				streamA->addEntry(new CTeSSLaStreamEventFloat(streamA, i * TIME_DIST_A, 3.77845));
				streamB->addEntry(new CTeSSLaStreamEventFloat(streamB, i * TIME_DIST_B, 5.84561));
				streamC->addEntry(new CTeSSLaStreamEventFloat(streamC, i * TIME_DIST_C, 2.71828));
			}
			unsigned int insertedLinesA = 0, insertedLinesB = 0, insertedLinesC = 0;

			char buffer[11];
			buffer[10] = 0;
			for (unsigned int i = 1; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i * TIME_DIST_B, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamB->getName() + " = " + std::to_string(5.84561));
				insertedLinesB++;
			}
			for (unsigned int i = 1; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i * TIME_DIST_C, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamC->getName() + " = " + std::to_string(2.71828));
				insertedLinesC++;
			}
			for (unsigned int i = 1; i < ENTRY_COUNT; i++)
			{
				_itoa_s(i * TIME_DIST_A, buffer, 10);
				expectedLines.push_back(std::string(buffer) + ": " + streamA->getName() + " = " + std::to_string(3.77845));
				insertedLinesA++;
			}

			// create trace, insert streams and export
			static std::string FILE_PATH = "test.trace";

			CTeSSLaTrace* trace = new CTeSSLaTrace();
			trace->addStream(streamB);
			trace->addStream(streamC);
			trace->addStream(streamA);

			trace->exportTrace(FILE_PATH);

			// load file and compare
			std::ifstream inputFile(FILE_PATH, std::ios::binary | std::ios::in);
			Assert::IsTrue(inputFile.is_open());

			int lastTimeStamp = 0;

			for (std::string line; std::getline(inputFile, line); )
			{
				//check timestamp
				int timestamp = atoi(line.substr(0, line.find(':')).c_str());
				Assert::IsTrue(timestamp >= lastTimeStamp);
				lastTimeStamp = timestamp;
				//search line in expectedLines and remove
				BOOL foundLine = false;
				for (auto it = expectedLines.begin(); !foundLine && it != expectedLines.end(); )
				{
					if ((*it).compare(line) == 0)
					{
						expectedLines.erase(it);
						foundLine = true;
					}
					else
					{
						it++;
					}
				}
				//Line must be found
				Assert::IsTrue(foundLine);
			}
			//all expected lines must be found (and therefore removed)
			unsigned int remainingLines = expectedLines.size();
			Assert::IsTrue(remainingLines == 0);
		}

	};
}
