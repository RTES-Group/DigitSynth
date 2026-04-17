#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_

#include <functional>

#include <sys/types.h>


using DoneCallback = std::function<void()>;
using GpioCallback = std::function<void(void)>;                  


#endif
