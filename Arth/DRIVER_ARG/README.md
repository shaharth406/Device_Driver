File : test_arg.c
Description :
		This file is a simple parameter passing module. 
		Contains basic module_init and module_exit functions and while inserting driver we have to pass parameter.
                module_param(),module_param_array(),module_param_cb() macro are used and predefined in moduleparam.h 
                module_param(name,type,permission),module_param_array(name,type,number,permission);module_param_cb(name,operation,pointer,permission);
		
Steps to compile :
1.Run				make
2.Insert module			sudo insmod test_arg.ko value = 2 arr_value=100,200,300,400 name="arth_shah" 
3.See the added module		cat /proc/modules | grep param
4.See the logs in dmesg 	dmesg | tail -2
  kernel ring buffer	
5.Remove module			sudo rmmod param
6.See the logs in dmesg 	dmesg | tail -2
7.Run				make clean


Keypoints to remember:
module_param();
module_param(name,type,permission);
module_param_array();
module_param_array(name,type,number,permission);
module_param_cb();
module_param_cb(name,operation,pointer,permission);


