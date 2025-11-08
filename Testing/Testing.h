#pragma once
#include <gtest/gtest.h>

#include <ByteProccessor.h>
#include <Mock.h>

using ::testing::InSequence;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

TEST(ProccessorWorkTesting, WrongDataInput)
{
	ByteProcessor Processor;

	MockSource SourceDevice;
	MockSink SinkDevice;

	EXPECT_CALL(SourceDevice, Read(_, 1))
		.WillOnce([](char* Buffer, int64_t size)
		{
			Buffer[0] = 0b11000000;
			return 1;
		})
		.WillOnce([](char* Buffer, int64_t size)
		{
			Buffer[0] = 0b11001110;
			return 1;
		})
		.WillOnce([](char* Buffer, int64_t size)
		{
			Buffer[0] = 0b10011010;
			return 1;
		})
		.WillOnce([](char* Buffer, int64_t size)
		{
			Buffer[0] = 0b10111111;
			return 1;
		})
		.WillRepeatedly(Return(-1));

	char FS_TestBuffer[5] = "NULL"; 
	char TF_TestBuffer[5] = "UND";

	{
		InSequence WriteSeq;

		EXPECT_CALL(SinkDevice, Write(StrEq(FS_TestBuffer), 5))
			.Times(2)
			.WillRepeatedly(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(TF_TestBuffer), 5))
			.Times(2)
			.WillRepeatedly(Return(5));
	}

	Processor.StartNewWork(&SourceDevice, &SinkDevice);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Processor.StopAllExisitingWorks();
}

TEST(ProccessorWorkTesting, CorrectDataInputCase1)
{
	ByteProcessor Processor;

	MockSource SourceDevice;
	MockSink SinkDevice;

	EXPECT_CALL(SourceDevice, Read(_, 1))
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b00000000;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b00001111;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b00111111;
				return 1;
			})
		.WillRepeatedly(Return(-1));

	char F_TestBuffer[5] = "0";
	char S_TestBuffer[5] = "15";
	char T_TestBuffer[5] = "63";

	{
		InSequence WriteSeq;

		EXPECT_CALL(SinkDevice, Write(StrEq(F_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(S_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(T_TestBuffer), 5))
			.WillOnce(Return(5));
	}

	Processor.StartNewWork(&SourceDevice, &SinkDevice);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Processor.StopAllExisitingWorks();
}

TEST(ProccessorWorkTesting, CorrectDataInputCase2)
{
	ByteProcessor Processor;

	MockSource SourceDevice;
	MockSink SinkDevice;

	EXPECT_CALL(SourceDevice, Read(_, 1))
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b01100000;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b01001111;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b01111111;
				return 1;
			})
		.WillRepeatedly(Return(-1));

	char F_TestBuffer[5] = "-32";
	char S_TestBuffer[5] = "15";
	char T_TestBuffer[5] = "-1";

	{
		InSequence WriteSeq;

		EXPECT_CALL(SinkDevice, Write(StrEq(F_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(S_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(T_TestBuffer), 5))
			.WillOnce(Return(5));
	}

	Processor.StartNewWork(&SourceDevice, &SinkDevice);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Processor.StopAllExisitingWorks();
}

TEST(ProccessorWorkTesting, CorrectDataInputCase3)
{
	ByteProcessor Processor;

	MockSource SourceDevice;
	MockSink SinkDevice;

	EXPECT_CALL(SourceDevice, Read(_, 1))
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b10000000;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b10001101;
				return 1;
			})
		.WillOnce([](char* Buffer, int64_t size)
			{
				Buffer[0] = 0b10011001;
				return 1;
			})
		.WillRepeatedly(Return(-1));

	char F_TestBuffer[5] = "a";
	char S_TestBuffer[5] = "n";
	char T_TestBuffer[5] = "z";

	{
		InSequence WriteSeq;

		EXPECT_CALL(SinkDevice, Write(StrEq(F_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(S_TestBuffer), 5))
			.WillOnce(Return(5));

		EXPECT_CALL(SinkDevice, Write(StrEq(T_TestBuffer), 5))
			.WillOnce(Return(5));
	}

	Processor.StartNewWork(&SourceDevice, &SinkDevice);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Processor.StopAllExisitingWorks();
}
