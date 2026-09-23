#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  // Need at least one argument: the command to time
  if(argc < 2){
    fprintf(2, "usage: time1 <command> [args...]\n");
    exit(1);
  }

  // Record the start time before creating the child
  int start = uptime();

  int pid = fork();
  if(pid < 0){
    fprintf(2, "time1: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // Child: replace this process with the requested command.
    // argv[1] is the command name; &argv[1] is its argument list.
    exec(argv[1], &argv[1]);
    // exec only returns if it failed
    fprintf(2, "time1: exec %s failed\n", argv[1]);
    exit(1);
  }

  // Parent: wait for the child to finish
  wait(0);

  // Record the end time and report the difference
  int end = uptime();
  printf("elapsed time: %d ticks\n", end - start);

  exit(0);
}
