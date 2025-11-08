set  (PRIVATE_SOURCES
	"Private/ByteProcessor.cpp"
)

set  (PUBLIC_SOURCES
    "Public/ByteProccessor.h"
	"Public/Source.h" 
	"Public/Sink.h"
)

add_library(Proccessor ${PRIVATE_SOURCES})

target_include_directories(Proccessor 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Public
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Public
)