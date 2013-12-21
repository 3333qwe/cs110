/**
 * File: mapreduce-worker.cc
 * -------------------------
 * Presents the implementation of the MapReduceWorker class.
 */

#include "mapreduce-worker.h"
#include <cassert>
#include <sstream>
#include "mr-messages.h"
#include "string-utils.h"
#include "client-socket.h"
#include "socket++/sockstream.h"
using namespace std;

MapReduceWorker::MapReduceWorker(const string& serverHost, unsigned short serverPort,
                                 const string& executablePath, const string& executable,
                                 const string& outputPath, const string& logfilePath) :
  serverHost(serverHost), serverPort(serverPort), executable(executable), 
  executablePath(executablePath), outputPath(outputPath), logfilePath(logfilePath) {
  createLogFile();
}

void MapReduceWorker::work() {
  string inputFile;
  while (true) {
    string inputFile;
    if (!requestInputFile(inputFile)) break;
    logfile << "Processing chunk named \"" + inputFile + "\"." << endl; 
    notifyServerInputFileWasProcessed(inputFile);
  }
  
  logfile << "Server is out of input files, so this worker is shutting down..." << endl;
}

void MapReduceWorker::createLogFile() {
  logfile.open(getLogFileName().c_str());
  assert(!!logfile);
  logfile << "Begin worker logging...." << endl;
  logfile << "server-host: " << serverHost << endl;
  logfile << "server-port: " << serverPort << endl;
  logfile << "executable: " << executable << endl;
  logfile << "executablePath: " << executablePath << endl;
  logfile << "outputPath: " << outputPath << endl;
}

string MapReduceWorker::getLogFileName() const {
  ostringstream oss;
  oss << logfilePath << "/" << getenv("HOST") << "-" << executable << ".log";
  return oss.str();
}

bool MapReduceWorker::requestInputFile(string& inputFile) {
  int clientSocket = getClientSocket();
  sockbuf sb(clientSocket);
  iosockstream ss(&sb);
  logfile << "Sending WORKER_READY message to server..." << endl;
  sendWorkerReady(ss);
  logfile << "Waiting for response from server..." << endl;
  string message, payload;
  receiveMessage(ss, message, payload);
  logfile << message << " response received from server..." << endl;
  if (message == "SERVER_DONE") return false;
  inputFile = trim(payload);
  return true;
}

void MapReduceWorker::notifyServerInputFileWasProcessed(const string& inputFile) {
  int clientSocket = getClientSocket();
  sockbuf sb(clientSocket);
  iosockstream ss(&sb);
  logfile << "Sending JOB_DONE to server..." << endl;
  sendJobCompleted(ss, inputFile);
}

static const int kServerInaccessible = 2;
int MapReduceWorker::getClientSocket() {
  int clientSocket = createClientSocket(serverHost, serverPort);
  if (clientSocket == kClientSocketError) {
    logfile << "Server at " << serverHost << ":" << serverPort << " "
            << "could not be reached! Aborting..." << endl;
    exit(kServerInaccessible);
  }
  
  return clientSocket;
}
