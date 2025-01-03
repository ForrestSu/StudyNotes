#!/bin/sh

# pacman -S  libpgm glib2

gcc -Wall -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include  -I/usr/include/pgm-5.2 -lpgm pgmsend.c  -o pgm_send


gcc -DG_OS_UNIX -DCONFIG_HAVE_EPOLL -Wall -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include  -I/usr/include/pgm-5.2 -lglib-2.0 -lgthread-2.0 -lpgm  pgmrecv.c  -o pgm_recv