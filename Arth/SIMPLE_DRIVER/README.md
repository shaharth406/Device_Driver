File : hello_world.c
Description :
		This file is a simple Hello World module. 
		Contains basic module_init and module_exit functions.
		
Steps to compile :
1.Run				make
2.Insert module			sudo insmod hello_world.ko
3.See the added module		cat /proc/modules | grep hello_world
4.See the logs in dmesg 	dmesg | tail -10
  kernel ring buffer	
5.Remove module			sudo rmmod hello_world
6.Run				make clean


Keypoints to remember:
1.static int hello_init();
2.static void hello_exit();
3.module_init();
4.module_exit();

