#pragma once

#include "pch.h"
#include "CppUnitTest.h"

#include <fstream>
#include <list>
#include <string.h>

#include "../CallerExample/CTeSSLaCANStream.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TeSSLaCANFrameTest
{
	TEST_CLASS(TeSSLaCANStreamTest)
	{
	public:

		TEST_METHOD(TestReadingCANFrame27_1)
		{
			unsigned char bytes[27] = {0xF2, 0x41, 0xCF, 0xCE, 0xC0, 0x76, 0x32, 0x41, 0x01, 0x01, 
				0xDB, 0xFE, 0x18, 0x01, 0x08, 0x08, 0xA3, 0x54, 0xA3, 0x54, 0xA3, 0x54, 0xFF,
				0xFF, 0x00, 0x00, 0x00 };
			CCANFrame aCanFrame(bytes+8, 19);
			//check id
			uint32_t expectedID = 0x18FEDB01;
			Assert::AreEqual(expectedID, aCanFrame.getIdentifier());
			//check data length
			unsigned char expectedDataLength = 0x8;
			Assert::AreEqual(expectedDataLength, aCanFrame.getDataLength());
			//check data bytes					  163  84	  163	84	  163  84		255 255
			unsigned char expectedDataBytes[8] = {0xA3, 0x54, 0xA3, 0x54, 0xA3, 0x54, 0xFF, 0xFF };
			unsigned char buff[8];
			aCanFrame.getDataBytesCopy(buff, expectedDataLength);

			for (int i = 0; i < 8; i++)
			{
				unsigned char expected = expectedDataBytes[i];
				unsigned char actual = buff[i];
				Assert::AreEqual(expected, actual);
			}
		}

		TEST_METHOD(TestReadingCANFrame27_2)
		{
			unsigned char bytes[27] = { 0x6A, 0x4D, 0xF3, 0xCE, 0xC0, 0x76, 0x32, 0x41, 0x01, 0x21, 
				0x04, 0xF0, 0x0C , 0x01, 0x08, 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
				0xFF, 0x00, 0x00, 0x00 };
			CCANFrame aCanFrame(bytes+8, 19);
			//check id
			uint32_t expectedID = 0x0CF00421;
			Assert::AreEqual(expectedID, aCanFrame.getIdentifier());
			//check data length
			unsigned char expectedDataLength = 0x8;
			Assert::AreEqual(expectedDataLength, aCanFrame.getDataLength());
			//check data bytes					  
			unsigned char expectedDataBytes[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
			unsigned char buff[8];
			aCanFrame.getDataBytesCopy(buff, expectedDataLength);

			for (int i = 0; i < 8; i++)
			{
				unsigned char expected = expectedDataBytes[i];
				unsigned char actual = buff[i];
				Assert::AreEqual(expected, actual);
			}
		}

		TEST_METHOD(TestReadingCANFrame27_3)
		{
			unsigned char bytes[27] = { 0xBF, 0x9F, 0x1A, 0xCF, 0xC0, 0x76, 0x32, 0x41, 0x01, 0x81, 
				0x03, 0xFF, 0x0C, 0x01, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				0x07, 0x00, 0x00, 0x00 };
			CCANFrame aCanFrame(bytes+8, 19);
			//check id
			uint32_t expectedID = 0x0CFF0381;
			Assert::AreEqual(expectedID, aCanFrame.getIdentifier());
			//check data length
			unsigned char expectedDataLength = 0x8;
			Assert::AreEqual(expectedDataLength, aCanFrame.getDataLength());
			//check data bytes					  
			unsigned char expectedDataBytes[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07 };
			unsigned char buff[8];
			aCanFrame.getDataBytesCopy(buff, expectedDataLength);

			for (int i = 0; i < 8; i++)
			{
				unsigned char expected = expectedDataBytes[i];
				unsigned char actual = buff[i];
				Assert::AreEqual(expected, actual);
			}
		}

		CTeSSLaStreamEventCANFrame* makeCANFrameEvent(CTeSSLaStreamCANFrame* owner, unsigned char id[4], double timeStamp, unsigned char data[8])
		{
			unsigned char buff[27];
			//copy timestamp
			memcpy(buff, &timeStamp, 8);
			// bus channel
			buff[8] = 1;
			//identifier
			memcpy(&(buff[9]), id, 4);
			//ide
			buff[13] = 1;
			//data length
			buff[14] = 8;
			buff[15] = 8;
			//data
			memcpy(&(buff[16]), data, 8);
			buff[24] = 8;
			buff[25] = 8;
			buff[26] = 8;

			return new CTeSSLaStreamEventCANFrame(owner, (long)(timeStamp * 1000), new CCANFrame(buff+8, 19));
		}

		TEST_METHOD(TestCANFrameStream)
		{
			const int FRAME_NUMBER = 20;
			const int DATA_LENGTH = 8;
			unsigned char expectedIdentifier[4] = { 0x30, 0xa4, 0xF0, 0x18 };

			//define timestamps
			double expectedTimeStamps[FRAME_NUMBER];
			for (int i = 0; i < FRAME_NUMBER; i++)
				expectedTimeStamps[i] = 1210048.8 + i * 0.01;
			//define random databytes
			unsigned char expectedData[FRAME_NUMBER][DATA_LENGTH];
			for (int i = 0; i < FRAME_NUMBER; i++)
				for (int j = 0; j < DATA_LENGTH; j++)
					expectedData[i][j] = rand() % 255;
			//define stream
			uint32_t ident = 0x18F0A430;// 30A4F018;
			CTeSSLaStreamCANFrame* stream = new CTeSSLaStreamCANFrame("Test", ident);
			//insert events
			for (int i = 0; i < FRAME_NUMBER; i++)
				stream->addEntry(makeCANFrameEvent(stream, expectedIdentifier, expectedTimeStamps[i], expectedData[i]));

			//check correctness

			for (int i = 0; i < FRAME_NUMBER; i++)
			{

				std::stringstream stringStream;
				stringStream << (long)(expectedTimeStamps[i] * 1000) << ": Test_CAN_";

				uint32_t ident = expectedIdentifier[3] << 24 | expectedIdentifier[2] << 16 | expectedIdentifier[1] << 8 | expectedIdentifier[0];

				stringStream << uint32_tToHexString(ident);

				stringStream << " = 0x";

				for (int j = 0; j < DATA_LENGTH; j++)
					stringStream << ByteToHexString(expectedData[i][j]);

				std::string expectedString = stringStream.str();

				std::string actualString = stream->getEntries().at(i)->toString();

				Assert::AreEqual(expectedString, actualString);
			}

			for (int i = 0; i < FRAME_NUMBER; i++)
			{
				delete stream->getEntries().at(i);
			}
			delete stream;
		}

		TEST_METHOD(TestCANFrameStream_2)
		{
			const int FRAME_NUMBER = 20;
			const int DATA_LENGTH = 8;
			unsigned char expectedIdentifier[4] = { 0x00, 0x04, 0xF0, 0x08 };

			//define timestamps
			double expectedTimeStamps[FRAME_NUMBER];
			for (int i = 0; i < FRAME_NUMBER; i++)
				expectedTimeStamps[i] = 1210048.8 + i * 0.01;
			//define random databytes
			unsigned char expectedData[FRAME_NUMBER][DATA_LENGTH];
			for (int i = 0; i < FRAME_NUMBER; i++)
				for (int j = 0; j < DATA_LENGTH; j++)
					expectedData[i][j] = rand() % 255;
			//define stream
			uint32_t ident = 0x08F00400;// 30A4F018;
			CTeSSLaStreamCANFrame* stream = new CTeSSLaStreamCANFrame("Test", ident);
			//insert events
			for (int i = 0; i < FRAME_NUMBER; i++)
				stream->addEntry(makeCANFrameEvent(stream, expectedIdentifier, expectedTimeStamps[i], expectedData[i]));

			//check correctness

			for (int i = 0; i < FRAME_NUMBER; i++)
			{

				std::stringstream stringStream;
				stringStream << (long)(expectedTimeStamps[i] * 1000) << ": Test_CAN_";

				uint32_t ident = expectedIdentifier[3] << 24 | expectedIdentifier[2] << 16 | expectedIdentifier[1] << 8 | expectedIdentifier[0];

				stringStream << uint32_tToHexString(ident);
				stringStream << " = 0x";

				for (int j = 0; j < DATA_LENGTH; j++)
					stringStream << ByteToHexString(expectedData[i][j]);

				std::string expectedString = stringStream.str();

				std::string actualString = stream->getEntries().at(i)->toString();

				Assert::AreEqual(expectedString, actualString);
			}

			for (int i = 0; i < FRAME_NUMBER; i++)
			{
				delete stream->getEntries().at(i);
			}
			delete stream;
		}


		TEST_METHOD(TestByteToHexString)
		{
			int counter = 0;
			for (unsigned char ctr = 0; ctr < 0xff; ctr++)
			{
				unsigned char i = ctr & 0xF0;
				unsigned char j = ctr & 0xF;
				counter++;
				if (counter > 255)
					throw "Error";
				std::string expected ="";
				switch (i) {
				case (0x0): expected = "0"; break;
				case (0x10): expected = "1"; break;
				case (0x20): expected = "2"; break;
				case (0x30): expected = "3"; break;
				case (0x40): expected = "4"; break;
				case (0x50): expected = "5"; break;
				case (0x60): expected = "6"; break;
				case (0x70): expected = "7"; break;
				case (0x80): expected = "8"; break;
				case (0x90): expected = "9"; break;
				case (0xA0): expected = "A"; break;
				case (0xB0): expected = "B"; break;
				case (0xC0): expected = "C"; break;
				case (0xD0): expected = "D"; break;
				case (0xE0): expected = "E"; break;
				case (0xF0): expected = "F"; break;
				default: throw "Error!";
				}
				switch (j) {
				case (0): expected += "0"; break;
				case (1): expected += "1"; break;
				case (2): expected += "2"; break;
				case (3): expected += "3"; break;
				case (4): expected += "4"; break;
				case (5): expected += "5"; break;
				case (6): expected += "6"; break;
				case (7): expected += "7"; break;
				case (8): expected += "8"; break;
				case (9): expected += "9"; break;
				case (10): expected += "A"; break;
				case (11): expected += "B"; break;
				case (12): expected += "C"; break;
				case (13): expected += "D"; break;
				case (14): expected += "E"; break;
				case (15): expected += "F"; break;
				default: throw "Error!";
				}
				std::string actual = ByteToHexString(i + j);
				Assert::AreEqual(expected, actual);

			}
		}
		

	};
}



