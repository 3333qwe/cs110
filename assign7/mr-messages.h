/**
 * File: mapreduce-messages.h
 * --------------------------
 * Provides common messsage formats to be passed between 
 * the mapreduce-server and mapreduce-workers.  In general,
 * All functions named send* expect an iosockstream (layered
 * over the network connection to another host) to be supplied
 * as the first argument.  The understanding is that messages
 * published to that iosockstream are sent to the other side and
 * ingested in a classic request/response manner.
 */

#pragma once
#include <iostream>
#include <string>

/**
 * Function: sendWorkerReady
 * -------------------------
 * When a worker is free and can process another input chunk (whether
 * it's for a mapper or a reducer job), it notifies the server by
 * sending it this message.
 */
void sendWorkerReady(std::ostream& outstream);

/**
 * Function: sendJobStart
 * ----------------------
 * When a server hears that a worker is idle and is willing to do more work,
 * it messages that worker by sending it this message (and identifying the
 * absolute path to the file that it should process).
 */
void sendJobStart(std::ostream& outstream, const std::string& chunkFile);

/**
 * Function: sendJobCompleted
 * --------------------------
 * When a worker invokes an executable (be it a mapper or a reducer) on 
 * a particular chunk, and that executable comes back with a zero exit code
 * (which expresses success!), the worker notifies the server that the identified
 * chunk has been fully processed.
 */
void sendJobCompleted(std::ostream& outstream, const std::string& chunkFile);

/**
 * Function: sendJobFailed
 * -----------------------
 * When a worker invokes a mapper or reducer executable on a particular
 * chunk, and that executable returns a nonzero error code, the worker
 * notifies the server of the failure by sending it this message.  This way
 * the server can reschedule the chunk to be processed later on).
 */
void sendJobFailed(std::ostream& outstream, const std::string& chunkFile);

/**
 * Function: sendServerDone
 * ------------------------
 * When the server has gotten confirmation that all chunks have been processed,
 * it sends this message to workers when that say they're ready for another job.
 * By sending this message from the server to a worker, the worker knows that
 * it can shut itself down.
 */
void sendServerDone(std::ostream& outstream);

/**
 * Function: receivedMessage
 * -------------------------
 * This is a generic routine that the server and the workers use
 * to surface a message that comes in over a network connection.  Some
 * messages are accompanied by a payload (which, in practice, is always
 * the name of a chunk file), and that chunk file is surfaced via the
 * third argument.  The return value is true if and only if the
 * message is accompanied by a payload.
 */
bool receiveMessage(std::istream& instream, std::string& message, std::string& payload);
