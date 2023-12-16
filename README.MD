Steps to run:
**************************************

**Compile the client.c file**

$ gcc client.c -o client

**Compile the server.c file**

$ gcc server.c -o server

-----------------------------------------------------------

Start the server

$ ./server

**The server should start without any errors and server should start listening on port 12345

If there is any error kill the server by using below command and then start the server**

$ fuser -k -n udp 12345
$ ./server

-------------------------------------------------------------

Start the client

Usage: ./client <input_file>

Example:

$ ./client hw6input.txt

The client should start sending the message packets to the server

----------------------------------------------------------------
----------------------------------------------------------------

To run the testcases:

create a folder named "testcases" in the same directory where client.c program is saved

$ mkdir testcases

copy all the testcase input files to the folder "testcases"

Then run the ./testscript.sh file

$ ./testscript.sh


If the testscript.sh file is not executable then change the permission

$ chmod +x testscript.sh
$ ./testscript.sh


This file will start the server and execute all the test files from the testcase folder and send the message packets to the server and save the log files 
in the "testlogs" directory ( which will automatically be created if not exists)

-------------------------------------------------------------------------------------------------------------------------------------------------------------------