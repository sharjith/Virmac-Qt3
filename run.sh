#!/bin/sh
# script for running virmac application

# set the virmac graphic kernel library path 
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/lib/
echo "[32mset graphic kernel path to[0m [33m${HOME}/lib/[0m"

# set the virmac geometry kernel library path 
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/lib/
echo "[32mset geometry kernel path to[0m [33m${HOME}/lib/[0m"

# set the virmac collection kernel library path 
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/lib/
echo "[32mset geometry kernel path to[0m [33m${HOME}/lib/[0m"

# set the path where cutter.dat file is located
# failing this causes application crash when accessing the tool library
export VRMTLIB=/home/sharjith/work/progs/qt/virmac
echo "[32mset cutter data path to[0m [33m$VRMTLIB[0m"

# exec the virmac executable
echo "[32mrunning virmac binary.....[0m"
exec /home/sharjith/work/progs/qt/virmac/virmac >& /dev/null &
