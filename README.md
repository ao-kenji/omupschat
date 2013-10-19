omupschat
=========

omupschat - chat test program with OMRON UPS, connected by USB.

This is my experimental program to chat with OMRON UPS using libusb on OpenBSD.
It does not work well yet, now:-(

* OpenBSD
To compile, you need to add following packages:
- libusb1
- libusb-compat

* Linux (Ubuntu) - as a reference
To compile, you need to add following packages:
- libusb-dev
- libbsd-dev
Then build by 'make -f Makefile.linux'.
