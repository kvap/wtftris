#!/bin/sh
clang -g -std=gnu99 -o wtftris main.c field.c figure.c shapes.c -lncurses -lm
