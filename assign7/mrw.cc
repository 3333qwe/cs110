/**
 * File: mrw.cc
 * ------------
 * This provides the entry point for all worker servers.
 * The implementation is straightforward, as it passes the
 * buck to the MapReduceWorker class.  See mapreduce-worker.h/cc
 * for the full details on how the workers do their job.
 */

#include "mapreduce-worker.h"

static const int kExpectedArgumentCount = 7;
int main(int argc, char *argv[]) {
  if (argc != kExpectedArgumentCount) return 1;
  MapReduceWorker mrw(/* serverHost = */ argv[1], 
                      /* serverPort = */ atoi(argv[2]), 
                      /* executable = */ argv[3], 
                      /* executablePath = */ argv[4], 
                      /* outputPath = */ argv[5], 
                      /* logfilePath = */ argv[6]);
  mrw.work();
  return 0;
}
