LibLog
======

A simple logging library written in C.  Simplest, minimalistic library for logging for an app.
I am writing these only for practice. :)

#############
#BUILD
#############

#to build library. "make" takes care of adding liblog.so to /usr/lib and updating ldconfig
make clean
make

#to build testapp
make test

#############
#RUN
#############

./test
