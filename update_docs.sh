#!/bin/sh

doxygen doxygen.conf && google-chrome ./html/index.html &
