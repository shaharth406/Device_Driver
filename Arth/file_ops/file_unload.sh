#!/bin/sh
module="file_ops"
#device="my_device"

sudo rmmod ./$module.ko $* || exit 1

echo "\n Here we check the message using dmesg:\n"

# here we print dmesg for checking module removed or not
dmesg | tail -5

echo "\n Here we clean the make file using make clean:\n"

#Here we clean the makefile using make clean
make clean 

echo "\nModule Run successfully..!!\n"
