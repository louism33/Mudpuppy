//
// Created by louis on 6/16/19.
//

#ifndef MUDPUPPY_TIMEMANAGEMENT_H
#define MUDPUPPY_TIMEMANAGEMENT_H

#include <chrono>


typedef std::chrono::milliseconds::rep TimePoint;

inline TimePoint now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::steady_clock::now().time_since_epoch()).count();
}


#endif //MUDPUPPY_TIMEMANAGEMENT_H
