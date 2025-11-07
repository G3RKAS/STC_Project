#pragma once
#include "Source.h"
#include "Sink.h"

class ByteProcessor
{
public:
	ByteProcessor(Source*, Sink*);

private:
	Source* SourceDevice;
	Sink* SinkDevice;
};