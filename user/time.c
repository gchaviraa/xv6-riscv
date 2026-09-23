#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pstat.h"   //full definition of struct rusage

int
main(int argc, char **argv)
{
  //Need at least one argument: the command to time
  if(argc < 2){
    fprintf(2, "usage: time <command> [args...]\n");
    exit(1);
  }

  //Record the start time before creating the child
  int start = uptime();

  int pid = fork();
  if(pid < 0){
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    //Child: replace this process with the requested command.
    //argv[1] is the command name; &argv[1] is its argument list.
    exec(argv[1], &argv[1]);
    //exec only returns if it failed
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  //Parent: wait for the child and collect its CPU usage
  int status;
  struct rusage ru;
  if(wait2(&status, &ru) < 0){
    fprintf(2, "time: wait2 failed\n");
    exit(1);
  }

  //Record the end time
  int end = uptime();

  int elapsed = end - start;
  int cpu = ru.cputime;
  //Percentage of elapsed time the child spent on the CPU.
  //Guard against dividing by zero for very short commands.
  int pct = (elapsed > 0) ? (cpu * 100) / elapsed : 0;

  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
         elapsed, cpu, pct);

  exit(0);
}
