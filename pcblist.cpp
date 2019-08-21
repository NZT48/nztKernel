#include "pcblist.h"

PCBList::PCBList(){
    front = back = 0;
}

PCBList::~PCBList(){
	HARD_LOCK
	Node* tmp;
	while(front){
		tmp = front;
		front = front->next;
		delete tmp;
	}
    HARD_UNLOCK
}

void PCBList::release(){
		for(PCB* tmp; tmp = get(); tmp->reschedule());
}


void PCBList::put(PCB* pcb){
    HARD_LOCK
        Node* node = new Node(pcb);
    HARD_UNLOCK

    if(back == 0){
        front = back = node;
    } else {
        back->next = node;
        back = node;
    }

}

PCB* PCBList::get(){
	LOCK;
	if (front) {
		PCB* ret = front->pcb;
		Node* old = front;
		front = front->next;
		if( front == 0) {
			back = 0;
		}
		UNLOCK;
		HARD_LOCK;
		delete old;
		HARD_UNLOCK;
		return ret;
	}
	UNLOCK;
	return 0;
}



void PCBList::PriorPut(PCB* p, Time maxTimeToWait) {
    LOCK;

    if (maxTimeToWait == 0) {
        maxTimeToWait = infiniteTimeSlice;
    }
    else if (maxTimeToWait > maximumTimeSlice) {
        maxTimeToWait = maximumTimeSlice;
    }

    if (front == 0) {
		front = back = new Node(p, 0, maxTimeToWait);
	} else {
        Node *prev = 0, *cur = front;

        while(cur != 0 && maxTimeToWait > cur->timeLeft){
            prev = cur;
            cur = cur->next;
        }

        if (prev == 0) {	// umetanje na pocetak prioritetnog reda
            front = new Node(p, front, maxTimeToWait);
        }
        else if (cur == 0) {	// umetanje na kraj prioritetnog reda
            Node* newElem = new Node(p, 0, maxTimeToWait);
            back->next = newElem;
            back = newElem;
        }
        else {		// umetanje u sredinu prioritetnog reda
            prev->next = new Node(p, cur, maxTimeToWait);
        }

    }

    UNLOCK;
}
