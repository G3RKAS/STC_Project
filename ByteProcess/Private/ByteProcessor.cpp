#include "ByteProcessor.h"

using std::move;
using std::make_unique;

ByteProcessor::~ByteProcessor()
{
	StopAllExisitingWorks();
}

thread::id ByteProcessor::StartNewWork(Source* InSource, Sink* InSink)
{
	unique_ptr<ThreadControl> NewThread = make_unique<ThreadControl>();
	NewThread->IsRunning = true;
	NewThread->ThreadHandler = thread(&ByteProcessor::ThreadTask, this, InSource, InSink);
	NewThread->ThreadID = NewThread->ThreadHandler.get_id();
	thread::id OutID = NewThread->ThreadID;

	ThreadList.push_back(move(NewThread));

	return OutID;
}

void ByteProcessor::JoinWorkIfExists(thread::id InThreadID)
{
	auto Thread = GetThreadByID(InThreadID);

	if (Thread != ThreadList.end())
	{
		ThreadControl* TempThread = (*Thread).get();

		if (TempThread->ThreadHandler.joinable())
		{
			TempThread->ThreadHandler.join();
		}
	}
}

void ByteProcessor::StopExistingWork(thread::id InThreadID)
{
	auto Thread = GetThreadByID(InThreadID);

	if (Thread != ThreadList.end())
	{
		ThreadControl* TempThread = (*Thread).get();

		TempThread->IsRunning = false;
		if (TempThread->ThreadHandler.joinable())
		{
			TempThread->ThreadHandler.join();
		}
		ThreadList.erase(Thread);

	}
}

void ByteProcessor::StopAllExisitingWorks()
{
	for (unique_ptr<ThreadControl>& value : ThreadList)
	{
		ThreadControl* TempThread = value.get();

		TempThread->IsRunning = false;
		if (TempThread->ThreadHandler.joinable())
		{
			TempThread->ThreadHandler.join();
		}
	}

	ThreadList.clear();
}

using std::this_thread::get_id;
using std::this_thread::sleep_for;

void ByteProcessor::ThreadTask(Source* InSource, Sink* InSink)
{
	auto Finded = GetThreadByID(std::this_thread::get_id());
	if (Finded == ThreadList.end()) return;

	ThreadControl* Thread = (*Finded).get();
	
	char ReadBuffer;
	char WriteBuffer[5];

	while (Thread->IsRunning)
	{
		int64_t ByteRead = InSource->Read(&ReadBuffer,1);

		if (ByteRead == -1)
		{
			sleep_for(std::chrono::seconds(1));
			continue;
		}
		else
		{
			uint8_t Byte = static_cast<uint8_t>(ReadBuffer);

			GetCharsFromByte(Byte, WriteBuffer, 5);

			InSink->Write(WriteBuffer, 5);
		}
	}
}

vector<unique_ptr<ByteProcessor::ThreadControl>>::iterator
ByteProcessor::GetThreadByID(thread::id InThreadID)
{
	return std::find_if(ThreadList.begin(), ThreadList.end(),
		[&InThreadID](unique_ptr<ThreadControl>& ptr) 
		{
			return ptr.get()->ThreadID == InThreadID;
		});
}

using std::snprintf;

void ByteProcessor::GetCharsFromByte(uint8_t InByte, char* OutChars, size_t OutCharSize)
{
	uint8_t Type = InByte >> 6;
	uint8_t Data = InByte & 0b00111111;

	if (Type == 0b00)
	{
		snprintf(OutChars, OutCharSize, "%u", Data);
	}
	else if (Type == 0b01)
	{
		int8_t NewData;
		if (Data & 0b00100000)
		{
			NewData = Data | 0b11000000;
		}
		else
		{
			NewData = Data;
		}
		snprintf(OutChars, OutCharSize, "%d", NewData);
	}
	else if (Type == 0b10)
	{
		if (Data <= 25)
		{
			char ConvertedChar = 'a' + Data;
			snprintf(OutChars, OutCharSize, "%c", ConvertedChar);
		}
		else
		{
			snprintf(OutChars, OutCharSize, "%s", "UND");
		}
	}
	else
	{
		snprintf(OutChars, OutCharSize, "%s", "NULL");
	}

	return;
}