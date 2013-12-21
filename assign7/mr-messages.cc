/**
 * File: mr-messages.cc
 * --------------------
 * Implements the trivial collection of messages that need to be sent from 
 * the server to the mappers and reducers and the equally trivial collection of
 * messages sent from the mappers and reducers back to the server.
 */

#include <sstream>
#include <exception>
#include "string-utils.h"
#include "mr-messages.h"
using namespace std;

void sendWorkerReady(ostream& outstream) { 
  outstream << "WORKER_READY" << " " << endl; 
}

void sendJobStart(ostream& outstream, const string& chunkFile) { 
  outstream << "JOB_START" << " " << chunkFile << endl; 
}

void sendJobCompleted(ostream& outstream, const string& chunkFile) { 
  outstream << "JOB_DONE" << " " << chunkFile << endl; 
}

void sendJobFailed(ostream& outstream, const string& chunkFile) { 
  outstream << "JOB_FAIL" << " " << chunkFile << endl; 
}

void sendServerDone(ostream& outstream) {
  outstream << "SERVER_DONE" << " " << endl; 
}

bool receiveMessage(istream& instream, string& message, string& payload) {
  string line;
  getline(instream, line);
  line = trim(line);
  istringstream iss(line);
  iss >> message;
  const string messagesWithPayloads[] = {"JOB_START", "JOB_DONE", "JOB_FAIL"};
  const size_t numMessagesWithPayloads = 
    sizeof(messagesWithPayloads)/sizeof(messagesWithPayloads[0]);
  bool hasPayload = 
    (find(messagesWithPayloads, messagesWithPayloads + numMessagesWithPayloads, 
          message) != messagesWithPayloads + numMessagesWithPayloads);
  if (hasPayload) iss >> payload;
  return hasPayload;
}
