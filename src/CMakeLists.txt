cmake_minimum_required(VERSION 3.14)
project(mudpuppy)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory (engines)
#add_subdirectory (evaluators)
#add_subdirectory (searchUtils)

add_executable(mudpuppy main.cpp main.h Board.cpp Board.h Art.cpp Art.h
        BitBoardUtils.cpp BitBoardUtils.h engines/EngineRandom.cpp engines/EngineRandom.h
        engines/EngineMinimax.cpp engines/EngineMinimax.cpp evaluators/EvalSimpleCount.cpp
        evaluators/EvalSimpleCount.h engines/EngineBase.h evaluators/EvalBase.h
        evaluators/EvalBasicHeuristics.cpp evaluators/EvalBasicHeuristics.h Perft.cpp
        Perft.h Move.cpp Move.h engines/EngineMinimaxBetter.cpp engines/EngineMinimaxBetter.h
        searchUtils/Extensions.cpp searchUtils/Extensions.h MoveUtils.cpp MoveUtils.h Tests.cpp Tests.h)