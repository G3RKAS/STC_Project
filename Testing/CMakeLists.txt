# CMakeList.txt : CMake project for Testing

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set  (TEST_SOURCES
	"MainTesting.cpp"
    "Testing.cpp"
    "Mock.cpp"
)

add_executable(RunTests ${TEST_SOURCES})

target_link_libraries(RunTests gtest gmock)

target_include_directories(RunTests PRIVATE
    get_target_property(gtest_include_dirs gtest INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(gmock_include_dirs gmock INTERFACE_INCLUDE_DIRECTORIES)
)