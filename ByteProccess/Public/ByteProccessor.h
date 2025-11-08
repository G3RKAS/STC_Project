#pragma once

#include <thread>
#include <atomic>
#include <vector>

#include "Source.h"
#include "Sink.h"

using std::thread;
using std::atomic;
using std::vector;
using std::unique_ptr;

class ByteProcessor
{
public:
	~ByteProcessor();

	thread::id StartNewWork(Source*, Sink*);
	void JoinWorkIfExists(thread::id);
	void StopExistingWork(thread::id);
	void StopAllExisitingWorks();

private:
	struct ThreadControl
	{
		thread ThreadHandler;
		thread::id ThreadID;
		atomic<bool> IsRunning = false;
	};

	void ThreadTask(Source*, Sink*);
	vector<unique_ptr<ThreadControl>>::iterator GetThreadByID(thread::id);
	void GetCharsFromByte(uint8_t, char*, size_t);

	vector<unique_ptr<ThreadControl>> ThreadList;
};