# Makefile for OpenBSD (and NetBSD)

PROG = omupschat

SRCS = omupschat.c omron_command.c

# For OpenBSD
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib -lusb

# For NetBSD
#SHLIBDIR = /usr/pkg/lib
#CFLAGS += -I/usr/pkg/include
#LDFLAGS += -lusb

NOMAN = 1

.include <bsd.prog.mk>
