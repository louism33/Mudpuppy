cmake_minimum_required(VERSION 3.14)
project(mudpuppyroot)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(mudpuppyroot src/main.cpp src/main.h src/Board.cpp src/Board.h src/Art.cpp src/Art.h src/BitBoardUtils.cpp src/BitBoardUtils.h src/engines/EngineRandom.cpp src/engines/EngineRandom.h src/engines/EngineMinimax.cpp src/engines/EngineMinimax.cpp src/evaluators/EvalSimpleCount.cpp src/evaluators/EvalSimpleCount.h src/engines/EngineBase.h src/evaluators/EvalBase.h src/evaluators/EvalBasicHeuristics.cpp src/evaluators/EvalBasicHeuristics.h src/Perft.cpp src/Perft.h src/MoveUtils.cpp src/MoveUtils.h src/Tests.cpp src/Tests.h src/Move.cpp src/Move.h src/engines/EngineMinimaxBetter.cpp src/engines/EngineMinimaxBetter.h src/searchUtils/Extensions.cpp src/searchUtils/Extensions.h)