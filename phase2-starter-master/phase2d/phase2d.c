
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

int P2_lockCreate()