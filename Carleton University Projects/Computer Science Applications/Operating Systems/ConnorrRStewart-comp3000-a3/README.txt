Question 1:
	Enter the Question 1 folder with two terminals
	Type (Terminal One):
		make
	Type (Terminal One):
		mkfifo pipe
	Type (Terminal One): 
		./3000pc-fifo-consumer 20 2 pipe
	Type (Terminal Two):
		./3000pc-fifo-producer 20 2 pipe
Question 2:
	Enter the Question 2 folder
	Type:
		make
	Type (test one):
		./3000pc-rendezvous 10000 100 20
	Type (test two):
		./3000pc-rendezvous 100000 1000 2000
	Type (test three):
		./3000pc-rendezvous 100000 200 30
Question 3:
	Enter the Question 3 folder
	Type:
		make
	Type:
		./3000pc-rendezvous 20 2 2
Question 4:
	N/A
Question 5:
	Move the contents of the Question 5 Folder to the desktop
	Type:
		sudo insmod yourtask.ko
		(Note) To remove/replace the module, you must type:
			sudo rmmod -v yourtask
	To write to the module, type:
		echo 1000 > /dev/yourtask
	To get output from the module into the terminal, type:
		cat /dev/yourtask
Question 6:
	N/A