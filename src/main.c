#include <signal.h>

#include "Command.h"
#include "Global.h"
#include "Problems.h"


int main(int argc, const char* const* argv)  
{
  struct Command args = ParseCommand(argc, argv);

  signal(SIGINT, Quit);

  switch(args.problemNo)
  {
    case 0: WritersStarvation(args.readersCount, args.writersCount); break;
    case 1: ReadersStarvation(args.readersCount, args.writersCount); break;
    case 2: NoStarvation(args.readersCount, args.writersCount); break;
  }
  
  return 0;
}