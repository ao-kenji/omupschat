omupschat
=========

omupschat - chat test program with OMRON UPS, connected by USB.

This is my experimental program to chat with OMRON UPS using libusb on OpenBSD.
It does not work well yet, now:-(

OpenBSD
-------
To compile, you need to install following packages by pkg_add(1):
- libusb1
- libusb-compat
Then build by 'make'.

Linux (Ubuntu)
--------------
As a reference, Linux version is also available.
To compile, you need to install following packages by apt-get(8):
- libusb-dev
- libbsd-dev
Then build by 'make -f Makefile.linux'.
