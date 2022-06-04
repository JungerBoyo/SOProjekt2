#include "Command.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char* HELP = "\
  Usage:\n\n\
    readerswriters -R <readers_count> -W <writers_count> -P <problem_solution>\n\n\
    //////////////////////////////////////////////\n\
    ///> problem_solution:                    ////\n\
    ///       if 0  { writers starvation }    ////\n\
    ///       if 1  { readers starvation }    ////\n\
    ///       if 2  {    no starvation   }    ////\n\
    //////////////////////////////////////////////\n\n\n\
  Options:\n\n\
    -h    show this message\n\n";

struct Command ParseCommand(int argc, const char* const* argv)
{
  struct Command result;

  char i;
  while((i = getopt(argc, argv, "hR:W:P:")) != -1)
  {
    if(i == '?')
    {
      printf("ParseCommand error\n\n %s", HELP);
      exit(EXIT_FAILURE);
    }
    else
    {
      switch(i)
      {
        case 'h':
          printf("%s", HELP);
          exit(EXIT_SUCCESS);
        case 'W':
          result.writersCount = atoi(optarg);
          break;
        case 'R':
          result.readersCount = atoi(optarg);
          break;
        case 'P':
          result.problemNo = atoi(optarg);
          break;

        default:break;
      }
    }
  }

  if(result.problemNo > 2)
  {
    printf("ParseCommand error\n\n %s", HELP);
    exit(EXIT_FAILURE);
  }

  return result;
}