#pragma once
#include <cstdint>

class Sink
{
public:
	virtual int64_t Write(char*, int64_t) = 0;
};