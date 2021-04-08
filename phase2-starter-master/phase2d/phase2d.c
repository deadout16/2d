
#include <string.h>
#include <stdlib.h>
#include <usloss.h>
#include <phase1.h>
#include <assert.h>
#include <libuser.h>
#include <libdisk.h>

#include "phase2Int.h"

// keeps track of locks created by user
typedef struct locks{
    int lockId;
    struct locks *next;
} locks;

// keeps track of condition variable created by user
typedef struct conds{
    int condId;
    int lockId;
    struct conds *next;
} conds;

locks *lockHead;
conds *condHead;



// NOTE: Not sure if locks are needed to do this since these are global lists

int checkLock(int lid){
    locks *temp;
    temp = lockHead;
    while(temp->next != NULL){
        if(temp->lockId == lid){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int checkCond(int vid){
    conds *temp;
    temp = condHead;
    while(temp->next != NULL){
        if(temp->condId == vid){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int P2_lockCreate(char *name, int *lid){
    int rc;
    locks *newLock = (locks *)malloc(sizeof(locks));
    newLock->next = lockHead->next;
    lockHead->next = newLock;

    rc = P1_LockCreate(name, lid);

    newLock->lockId = *lid;
    return rc;
}

int P2_lockFree(int lid){
    int rc;
    if(!checkLock(lid)){
        return P1_INVALID_LOCK;
    }
    rc = P1_LockFree(lid);
    return rc;
}

int P2_lockName(char *name, int lid){
    int rc;
    if(!checkLock(lid)){
        return P1_INVALID_LOCK;
    }
    rc = P1_LockName(lid, name, P1_MAXNAME);
    return rc;
}

int P2_lockAcquire(int lid){
    int rc;
    if(!checkLock(lid)){
        return P1_INVALID_LOCK;
    }
    rc = P1_Lock(lid);
    return rc;
}

int P2_lockRelease(int lid){
    int rc;
    if(!checkLock(lid)){
        return P1_INVALID_LOCK;
    }
    rc = P1_Unlock(lid);
    return rc;
}

int P2_condCreate(char *name, int lid, int *vid){
    int rc;
    if(!checkLock(lid)){
        return P1_INVALID_LOCK;
    }
    conds *newCond = (conds *)malloc(sizeof(conds));
    newCond->next = condHead->next;
    condHead->next = newCond;

    rc = P1_CondCreate(name, lid, vid);

    newCond->lockId = lid;  // NOTE int was passed in (error on *lid)
    newCond->condId = *vid;
    return rc;
}

int P2_condFree(int vid){
    int rc;
    if(!checkCond(vid)){
        return P1_INVALID_COND;
    }
    rc = P1_CondFree(vid);
    return rc;
}

int P2_condName(int vid, char *name, int length){
    int rc;
    if(!checkCond(vid)){
        return P1_INVALID_COND;
    }
    rc = P1_CondName(vid, name, length);
    return rc;
}

int P2_condWait(int vid){
    int rc;
    if(!checkCond(vid)){
        return P1_INVALID_COND;
    }
    rc = P1_Wait(vid);
    return rc;
}

int P2_condSignal(int vid){
    int rc;
    if(!checkCond(vid)){
        return P1_INVALID_COND;
    }
    rc = P1_Signal(vid);
    return rc;
}

int P2_condBraodcast(int vid){
    int rc;
    if(!checkCond(vid)){
        return P1_INVALID_COND;
    }
    rc = P1_Broadcast(vid);
    return rc;
}

static void lockCreateStub(USLOSS_Sysargs *sysargs){
    int lid;
    char *name = (char *)sysargs->arg1;
    int rc = P2_lockCreate(name, &lid);
    sysargs->arg1 = (void*) lid;
    sysargs->arg4 = (void*)  rc;
}

static void lockFreeStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockFree(lid);
    sysargs->arg4 = (void*) rc;
}

static void lockNameStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    char *name = (char *)sysargs->arg2;
    int rc = P2_lockName(name, lid);
    sysargs->arg4 = (void*) rc;
}

static void lockAcquireStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockAcquire(lid);
    sysargs->arg4 = (void*) rc;
}

static void lockReleaseStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockRelease(lid);
    sysargs->arg4 = (void*) rc;
}

static void condCreateStub(USLOSS_Sysargs *sysargs){
    char *name = (char *)sysargs->arg1;
    int lid = (int)sysargs->arg2;
    int vid;
    int rc = P2_condCreate(name, lid, &vid);
    sysargs->arg1 = (void*) vid;
    sysargs->arg4 = (void*) rc;
}

static void condFreeStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condFree(vid);
    sysargs->arg4 = (void*) rc;
}

static void condNameStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    char *name = (char *)sysargs->arg2;
    int length = (int) sysargs->arg3;
    int rc = P2_condName(vid, name,length);
    sysargs->arg4 = (void*) rc;
}

static void condWaitStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condWait(vid);
    sysargs->arg4 = (void*) rc;
}

static void condSignalStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condSignal(vid);
    sysargs->arg4 = (void*) rc;
}

static void condBroadcastStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condBraodcast(vid); // spelled wrong?
    sysargs->arg4 = (void*) rc;
}

int P2_Startup(void *arg)
{
    int rc, pid;

    P2ClockInit();
    P2DiskInit();

    // malloc and set the head node values for lists
    lockHead = (locks *)malloc(sizeof(locks));
    lockHead->lockId = -1;
    lockHead->next = NULL;
    condHead = (conds *)malloc(sizeof(conds));
    condHead->condId = -1;
    condHead->next = NULL;

    // install system call handlers
    rc = P2_SetSyscallHandler(SYS_LOCKCREATE, lockCreateStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_LOCKFREE, lockFreeStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_LOCKNAME, lockNameStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_LOCKNAME, lockNameStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_LOCKACQUIRE, lockAcquireStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_LOCKRELEASE, lockReleaseStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDCREATE, condCreateStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDFREE, condFreeStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDNAME, condNameStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDWAIT, condWaitStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDSIGNAL, condSignalStub);
    assert(rc == P1_SUCCESS);

    rc = P2_SetSyscallHandler(SYS_CONDBROADCAST, condBroadcastStub);
    assert(rc == P1_SUCCESS);

    rc = P2_Spawn("P3_Startup", P3_Startup, NULL, 4*USLOSS_MIN_STACK, 2, &pid);
    assert(rc == P1_SUCCESS);
    
    // wait for P3_Startup to terminate

    P2DiskShutdown();
    P2ClockShutdown();

    return 0;
}