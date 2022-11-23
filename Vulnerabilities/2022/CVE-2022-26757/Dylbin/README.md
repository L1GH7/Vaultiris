# Flow Divert Race Condition

In `flow_divert_pcb_init_internal`, a flow divert PCB is created and added to the desired socket below:

```c
    fd_cb = flow_divert_pcb_create(so); // 1
    if (fd_cb != NULL) {
        so->so_fd_pcb = fd_cb;
        so->so_flags |= SOF_FLOW_DIVERT;
        // ...

        error = flow_divert_pcb_insert(fd_cb, group_unit); // 2
        if (error) {
            so->so_fd_pcb = NULL;
            so->so_flags &= ~SOF_FLOW_DIVERT;
            FDRELEASE(fd_cb); // 3
        } else {
```

`flow_divert_pcb_create` (1) creates a flow divert PCB and initializes it with a refcount of 1 to represent the socket's ownership. `flow_divert_pcb_init_internal` has a reference to the PCB on the stack with variable `fd_cb` that is otherwise unaccounted for with the assumption that `fd_cb` should be alive for the duration of the entire function thus the incref/decref can be elided. But `flow_divert_pcb_insert` (2) drops the socket lock, so another thread can call `disconnectx` on the socket, deleting the PCB from the socket after dropping its only reference. This leaves the `fd_cb` pointer dangling pointing to freed memory. If `flow_divert_pcb_insert` fails, as in this testcase (no groups available), the `FD_RELEASE` (3) call will (among other possible outcomes) modify a freed buffer.

The syscalls involved are available inside the app sandbox on iOS 15.4.

This bug was patched in iOS 15.5 (All credit goes to @NedWilliamson for this amazing bug.)
