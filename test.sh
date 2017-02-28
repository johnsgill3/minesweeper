#!/bin/bash
#1*1
#222
#1*1

NUMROW=9
NUMCOL=9
NUMBOMB=10
rm -rf fifo0 fifo1
mkfifo fifo0 fifo1

build/minesweeper  -r ${NUMROW} -c ${NUMCOL} -b ${NUMBOMB} -i -d > fifo0 < fifo1 2>game.log &
build/solver -r ${NUMROW} -c ${NUMCOL} -b ${NUMBOMB} -g 1 -d < fifo0 > fifo1 2>solver.log
