#include "ByteProcessor.h"

using std::move;
using std::make_unique;

/*
	Деструктор объекта, который останавливает все запущенные потоки
*/
ByteProcessor::~ByteProcessor()
{
	StopAllExisitingWorks();
}

/*
	Метод, запускающий новый поток по работе с устройствами InSource и InSink.
	Поток будет работать до тех пор, пока не будет вызван останаливающий метод.
	Возвращает идентификатор созданного потока.
*/
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

/*
	Метод, блокирующий вызывающий поток, пока не будет завершён поток с InThreadID.
*/
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

/*
	Метод, останавливающий поток по InThreadID.
*/
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

/*
	Метод, останавливающий все работающие потоки.
*/
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

/*
	Метод, который выполняет каждый запущенный поток с помощью StartNewWork(InSource, InSink).
*/
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

/*
	Метод получения итератора вектора, по которому находится структура управления потоком с InThreadID.
*/
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

/*
	Метод перевода байта в строковый символ по алгоритму:
	Байт состоит из битов type (6-ой и 7-ой) и битов data (0-5), хранящие данные:
		a. Если type = 0b00, то data – беззнаковое целое;
		b. Если type = 0b01, то data – знаковое целое;
		c. Если type = 0b10, то data – одина из букв латинского алфавита
			В случае, если data > 25 записывает в буфер OutChars "UND"
		d. Если type = 0b11, то в буфер OutChars записывается "NULL"
	Входные параметры:
		InByte - Обрабатываемый байт
		OutChars - Выходной буфер, в который записывается результат алгоритма
		OutCharSize - Размер выходного буфера
*/
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