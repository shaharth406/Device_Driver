File : int_driver.c
Description :
		This file is a simple implementation of interrupts in character driver module.
Steps to compile :
1.Run				   				make
2.Insert module			   			sudo insmod intr.ko
3.Trigger interrupt: Read          			sudo cat /dev/intr_device
  device file
4.See the logs in dmesg 	   			dmesg |tail -10
  kernel ring buffer	
5.Remove module			   			sudo rmmod hello
6.Run				   			make clean
