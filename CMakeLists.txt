# CMake instructions to make the static lib
project(WinBuf)

cmake_minimum_required(VERSION 3.0)

ADD_LIBRARY( WinStream STATIC
             WinBuf.cpp )


# CMake instructions to test using the static lib

SET( APP_EXE StaticTest )

ADD_EXECUTABLE( ${APP_EXE}
                test_winbuf.cpp )

TARGET_LINK_LIBRARIES( ${APP_EXE}
                       WinStream )
