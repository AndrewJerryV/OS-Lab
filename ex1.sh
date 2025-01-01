#!/bin/bash
echo "Operating System and Version:"
cat /etc/os-release
echo "Release Number:"
uname -r
echo "Kernel Version:"
uname -v
echo "Available Shells:"
cat /etc/shells
echo "CPU Information:"
lscpu
echo "Memory Information:"
free -h
echo "Hard Disk Information:"
lsblk
echo "Disk Model and Cache:"
sudo lshw
echo "Mounted File Systems:"
df
