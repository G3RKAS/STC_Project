#pragma once
#include <cstdint>

class Source
{
public:
	virtual int64_t Read(char*, int64_t) = 0;
};