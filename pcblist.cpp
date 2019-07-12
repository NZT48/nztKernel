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


void PCBList::put(PCB* pcb){
    HARD_LOCK
        Node* node = new Node(pcb);
    HARD_UNLOCK

    if(front){
        back->next = node;
    } else {
        front = node;
    }
    back = node;

}

PCB* PCBList::get(){
    if(!front) return 0;

	Node* tmp = front;
	front = front->next;
    if(!front) back = 0;

    PCB* ret = tmp->pcb;
    tmp->pcb = 0;

    HARD_LOCK
        delete tmp;
    HARD_UNLOCK

	return ret;
}
