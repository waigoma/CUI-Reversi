cmake_minimum_required(VERSION 3.22)
project(BNS)

set(CMAKE_CXX_STANDARD 14)

add_executable(BNS src/main.cpp src/PoolAllocator.h src/reversi/ReversiGame.cpp src/reversi/ReversiGame.h src/reversi/ReversiConstEnum.h src/reversi/ReversiRecord.h src/reversi/OthelloRecord.cpp)
