#include <xinu.h>
local	lid32	newlock(void);
/**
 * Create a new semaphore and return the ID to the caller
 * @return ID of the mutex to caller, or SYSERR
 */
lid32	lock_create()
{
	intmask	mask;		/* saved interrupt mask	*/
	lid32	lockid;		/* lock ID to return	*/
	mask = disable();
	if ((lockid = newlock()) == SYSERR)
	{
		restore(mask);
		return SYSERR;
	}
	restore(mask);
	return lockid;
}
/**
 * Allocate an unused lock and return its index
 * @return	ID of free lock
 */
local	lid32	newlock(void)
{
	static	lid32	nextlock = 0;	/* next lockid to try	*/
	lid32	lockid;			/* ID to return	*/
	int32	i;			/* iterate through # entries	*/
	//TODO START
	// loop through each element in the lock table.
	for (i = 0; i < NLOCK; i++)
	{
		// and find a lock that is free to use
		nextlock = i;
		if (locktab[nextlock].state == LOCK_FREE)
		{
			lockid = nextlock;
			// set its state to used, and reset the mutex to FALSE
			locktab[lockid].state = LOCK_USED;
			locktab[lockid].lock = FALSE;
			// return its lockid
			//__TEST__kprintf("New lock %d\n",lockid);
			return lockid;
		}
	}
	// if there is no such lock, return SYSERR
	return SYSERR;
}
/**
 * Delete a lock by releasing its table entry
 * @param ID of lock to release
 */
syscall	lock_delete(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/
	struct  queue *lqueue;		/* ptr to lock table entry wait queue */
	pid32	lqhpid;			/* PID of lock table entry wait queue head	*/
	struct  procent *hprptr; /* ptr to process at head of wait queue */
	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}
	//TODO START
	// reset lock's state to free and the mutex to FALSE
	lptr->state = LOCK_FREE;
	lptr->lock = FALSE;
	// remove all processes waiting on its queue, and send them to the ready queue
	lqueue = lptr->wait_queue;
	while(nonempty(lqueue))
	{
		lqhpid = dequeue(lqueue);
		hprptr = &proctab[lqhpid];
		enqueue(lqhpid, readyqueue, hprptr->prprio);
	}
	//TODO (RAG) - remove all RAG edges to and from this lock
	rag_dealloc(lqhpid, lptr);
	//TODO END
	resched();
	restore(mask);
	return OK;
}
/**
 * Acquires the given lock, or cause current process to wait until acquired
 * @param lockid	lock on which to wait
 * @return status
 */
syscall	acquire(lid32 lockid)
{
	intmask mask;			// saved interrupt mask
	struct	lockentry *lptr;	// ptr to lock table entry
	struct  procent *currptr; // ptr to current process
	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}
	//TODO START
	// enqueue the current process ID on the lock's wait queue
	currptr = &proctab[currpid];
	enqueue(currpid, lptr->wait_queue, currptr->prprio);
	//TODO (RAG) - add a request edge in the RAG
	rag_request(currpid, lptr);
	//TODO END
	restore(mask);				//reenable interrupts
	//TODO START
	// lock the mutex!
	mutex_lock(&(lptr->lock));
	//TODO END
	mask = disable();			//disable interrupts
	//TODO START
	//__TEST__kprintf("Locked %d!\n", lockid);
	//TODO (RAG) - we reache this point. Must've gotten the lock! Transform request edge to allocation edge
	rag_dealloc(currpid, lptr);
	rag_alloc(currpid, lptr);
	//TODO END
	restore(mask);				//reenable interrupts
	return OK;
}
/**
 * Unlock, releasing a process if one is waiting
 * @param lockid ID of lock to unlock
 * @return status
 */
syscall	release(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/
	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr= &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}
	//TODO START
	// remove current process' ID from the lock's queue
	remove(currpid, lptr->wait_queue);
	// unlock the mutex
	//__TEST__kprintf("Unlocked %d!\n", lockid);
	mutex_unlock(&(lptr->lock));
	//TODO (RAG) - remove allocation edge from RAG
	rag_dealloc(currpid, lptr);
	//TODO END
	restore(mask);
	return OK;
}
