cmake_minimum_required (VERSION 3.2)
project (TC)
set (CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



## begin gtest and gmock
set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
## end gtest and gmock

include_directories(include)
add_executable(
    test_runner
    src/tc/test/avl_tree_test.cxx
    src/tc/test/tree_test.cxx)
target_link_libraries(test_runner gtest gmock_main)
add_test(NAME avl_tree_test COMMAND test_runner)
add_test(NAME tree_test COMMAND test_runner)

