#!/bin/sh

#This shell script is for loading module and check Major no , dmesg 


#Execute make file using make
make
# $Id: file_ops_load,v 1.2 2018/12/18 11:50:49 Arth Shah & Abhijit kashid $
module="file_ops" #module_name
device="my_dev" #Device_Driver Name
mode="666"	#mode permission

# invoke insmod with all arguments we got
# and use a pathname, as insmod doesn't look in . by default
sudo insmod ./$module.ko $* || exit 1 #if module name is not found than it exit

sleep 2 # some delay

echo "\ncheking loaded module by invoking lsmod\n"

lsmod | head # its show inserted module in Module list

# retrieve major number
#major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

echo "\nYou can see here your Device's major No:\n"
echo "Major=" $(cat /proc/devices | grep "$device" | awk  '{print $1}')

echo "\nChecking massage using dmesg:\n "
dmesg | tail -10  

echo "\nModule run successfully...!!\n"
