/**********************************
 * FILE NAME: MP1Node.cpp
 *
 * DESCRIPTION: Membership protocol run by this Node.
 * 				Header file of MP1Node class.
 **********************************/

#ifndef _MP1NODE_H_
#define _MP1NODE_H_

#include "stdincludes.h"
#include "Log.h"
#include "Params.h"
#include "Member.h"
#include "EmulNet.h"
#include "Queue.h"

#include <unordered_map>

/**
 * Macros
 */
#define TREMOVE 20
#define TFAIL 5
#define MAXQUEUELEN 1000
/*
 * Note: You can change/add any functions in MP1Node.{h,cpp}
 */

/**
 * Message Types
 */
enum MsgTypes{
    JOINREQ,
    JOINREP,
	PING,
	PINGREQ,
	ACK,
	FAIL,
	DIRECTPING,
	INDIRECTPING,
    DUMMYLASTMSGTYPE
};

/**
 * STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message
 */
typedef struct MessageHdr {
	enum MsgTypes msgType;
}MessageHdr;

struct MessageBody{
	long discriptionID;
	int host;
	short port;
	long heartBeat;
	long timeStamp;
	vector<MemberListEntry> memberList;
};

struct notFullFilledPing{
	long discriptionID;
	long timestamp;
	int host;
	short port;
};
/**
 * CLASS NAME: MP1Node
 *
 * DESCRIPTION: Class implementing Membership protocol functionalities for failure detection
 */
class MP1Node {
private:
	EmulNet *emulNet;
	Log *log;
	Params *par;
	Member *memberNode;
	char NULLADDR[6];
	size_t curPingMemIndex;
	notFullFilledPing* waitingPingList;
	size_t waitingCount;
	// std::unordered_map<long, long> *pingPromise;
public:
	MP1Node(Member *, Params *, EmulNet *, Log *, Address *);
	Member * getMemberNode() {
		return memberNode;
	}
	int recvLoop();
	static int enqueueWrapper(void *env, char *buff, int size);
	void nodeStart(char *servaddrstr, short serverport);
	int initThisNode(Address *joinaddr);
	int introduceSelfToGroup(Address *joinAddress);
	int finishUpThisNode();
	void nodeLoop();
	void checkMessages();
	bool recvCallBack(void *env, char *data, int size);
	void nodeLoopOps();
	int isNullAddress(Address *addr);
	Address getJoinAddress();
	void initMemberListTable(Member *memberNode);
	void printAddress(Address *addr);
	MessageBody parseMessageBody(MsgTypes, char* body);
	void encountJoinReq(MessageBody);
	void encountJoinRep(MessageBody);
	void encountPing(MessageBody);
	void encountAck(MessageBody);
	virtual ~MP1Node();
};

#endif /* _MP1NODE_H_ */
