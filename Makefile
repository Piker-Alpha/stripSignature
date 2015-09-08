##
#
# stripSignature.c
#
# Intel 64-bit (x86_64) version
#
##

PREFIX=/usr/local

PREFIX=/usr/local

AS=as
AR=ar
CC=cc
RANLIB=ranlib
INSTALL=install
ASFLAGS=-arch x86_64
ARFLAGS=cru
CFLAGS=-arch x86_64 -O2

all: clean stripSignature

.s.o:
	$(AS) $(ASFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

stripSignature: stripSignature.o
	$(CC) $(CFLAGS) -o $@ stripSignature.o

clean:
	clear
	rm -f *.o *.a kext
