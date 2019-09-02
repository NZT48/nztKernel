#include "defin.h"
#include "sigList.h"
#include "timer.h"

SignalList::~SignalList(){
    LOCK;
    SignalList::SignalElem* tmp = 0;
	while(head){
		tmp = head;
		head = head->next;
		HARD_LOCK;
		delete tmp;
		HARD_UNLOCK;
	}
    UNLOCK;
}

void SignalList::put(SignalId id){
	if(id > 15) return;
    SignalElem* tmp = new SignalElem(id);
    if(head)
        tail->next = tmp;
    else
        head = tmp;
    tail = tmp;
}

SignalId SignalList::get(){
	SignalElem* cur = head;
	while(cur){
		if(Timer::runningPCB->blockedSignals[cur->id] || PCB::globallyBlockedSignals[cur->id]){
			cur = cur->next;
		}
		else{
			int ret = cur->id;
			remove(cur);
			return ret;
		}
	}
	return -1; /* There are no more signals */
}

void SignalList::remove(SignalElem* e){
    SignalElem *cur = head, *prev = 0;
    while(cur && cur != e){
        prev = cur;
        cur = cur->next;
    }
    if(cur){
        if(cur != head)
            prev->next = cur->next;
        else
            head = head->next;
        if(cur == tail)
            tail = prev;
        HARD_LOCK;
            delete cur;
        HARD_UNLOCK;
    }
}
