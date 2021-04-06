
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
    locks *next;
} locks;

// keeps track of condition variable created by user
typedef struct conds{
    int condId;
    conds *next;
} conds;

locks *lockHead;
conds *condHead;

int P2_Startup(void *arg)
{
    int rc, pid;

    P2ClockInit();
    P2DiskInit();

    // malloc and set the head node values for lists
    lockHead = (locks *)malloc(sizeof(locks));
    lockHead.lockId = -1;
    lockHead.next = NULL;
    condHead = (conds *)malloc(sizeof(conds));
    condHead.condId = -1;
    condHead.next = NULL;

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

void checkLock(int lid){

}

void checkCond(int vid){

}

int P2_lockCreate(char *name, int *lid){

}

int P2_lockFree(int lid){

}

int P2_lockName(char *name, int lid){

}

int P2_lockAcquire(int lid){

}

int P2_lockRelease(int lid){

}

int P2_condCreate(char *name, int lid, int *vid){

}

int P2_condFree(int vid){

}

int P2_condName(int vid, char *name){

}

int P2_condWait(int vid){

}

int P2_condSignal(int vid){

}

int P2_condBraodcast(int vid){

}

static void lockCreateStub(USLOSS_Sysargs *sysargs){
    int lid;
    char *name = (char *)sysargs->arg1;
    int rc = P2_lockCreate(name, &lid);
    sysargs->arg1 = lid;
    sysargs->arg4 = rc;
}

static void lockFreeStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockFree(lid);
    sysargs->arg4 = rc;
}

static void lockNameStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    char *name = (char *)sysargs->arg2;
    int rc = P2_lockName(name, lid);
    sysargs->arg4 = rc;
}

static void lockAcquireStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockAcquire(lid);
    sysargs->arg4 = rc;
}

static void lockReleaseStub(USLOSS_Sysargs *sysargs){
    int lid = (int)sysargs->arg1;
    int rc = P2_lockRelease(lid);
    sysargs->arg4 = rc;
}

static void condCreateStub(USLOSS_Sysargs *sysargs){
    char *name = (char *)sysargs->arg1;
    int lid = (int)sysargs->arg2;
    int vid;
    int rc = P2_condCreate(name, lid, &vid);
    sysargs->arg1 = vid;
    sysargs->arg4 = rc;
}

static void condFreeStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condFree(vid);
    sysargs->arg4 = rc;
}

static void condNameStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    char *name = (char *)sysargs->arg2;
    int rc = P2_condName(vid, name);
    sysargs->arg4 = rc;
}

static void condWaitStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condWait(vid);
    sysargs->arg4 = rc;
}

static void condSignalStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condSignal(vid);
    sysargs->arg4 = rc;
}

static void condBroadcastStub(USLOSS_Sysargs *sysargs){
    int vid = (int)sysargs->arg1;
    int rc = P2_condBroadcast(vid);
    sysargs->arg4 = rc;
}