# Makefile for OpenBSD

PROG = omupschat

SRCS = omupschat.c omron_command.c
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib -lusb

NOMAN = 1

.include <bsd.prog.mk>
