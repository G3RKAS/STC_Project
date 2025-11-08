#pragma once
#include <gmock/gmock.h>
#include "Source.h"
#include "Sink.h"

class MockSource : public Source {
public:
    MOCK_METHOD(int64_t, Read, (char*, int64_t), (override));
};

class MockSink : public Sink {
public:
    MOCK_METHOD(int64_t, Write, (char*, int64_t), (override));
};