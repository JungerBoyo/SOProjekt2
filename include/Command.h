#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>

struct Command
{
  uint32_t readersCount;
  uint32_t writersCount;
  uint32_t problemNo;
};

struct Command ParseCommand(int argc, const char* const* argv);

#endif