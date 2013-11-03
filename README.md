omupschat
=========

omupschat - chat test program with OMRON UPS, connected by USB.

This is my experimental program to chat with OMRON UPS using libusb on
(mainly)OpenBSD.  Now it works with some quirk.

OpenBSD
-------
To compile, you need to install following packages by pkg_add(1):
- libusb1
- libusb-compat

Then build by 'make'.

NetBSD
------
To compile, you need to install following packages by pkg_add(1):
- libusb

Then edit Makefile to use NetBSD options, and build by 'make'.

Linux (Ubuntu)
--------------
As a reference, Linux version is also available.
To compile, you need to install following packages by apt-get(8):
- libusb-dev
- libbsd-dev

Then build by 'make -f Makefile.linux'.

How to use
----------
```
% sudo ./omupschat
bus 0, device 0, VendorID:0x10de, ProductID:0x0000, at (000/001)
bus 1, device 1, VendorID:0x10de, ProductID:0x0000, at (001/001)
bus 1, device 2, VendorID:0x067b, ProductID:0x2303, at (001/002)
bus 1, device 3, VendorID:0x0590, ProductID:0x0080, at (001/003)
** Target device found
bus 1, device 4, VendorID:0x0461, ProductID:0x4d15, at (001/004)
bus 1, device 5, VendorID:0x413c, ProductID:0x2003, at (001/005)
Q1
send:[Q1]
read:[(103.6 102.2 103.7 024 59.9 13.6 39.9 00101000]
ret = 64
^C
```
