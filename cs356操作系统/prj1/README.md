## Prj1

### Problem1

Write a new system call in Linux.

* The system call you write should take two arguments and return the process tree information in a depthfirst-search (DFS) order.


* Each system call must be assigned a number. Your system call should be assigned number 28

  ​

```shell
cd problem1
make
android avd
adb push ptree.ko /data/misc/ptree/ptree.ko
adb shell 
cd data/misc/ptree
insmod ptree.ko
rmmod ptree.ko
```

And we will get the output similar to the testscript.txt

  ​

### Problem2 Test your new system call

Write a simple C program which calls ptree

  * Print the entire process tree (in DFS order) using tabs to indent children with respect to their parents.

  * The output format of every process is:

```c
	printf(/* correct number of \t */);
	printf("%s,%d,%ld,%d,%d,%d,%d\n", p.comm, p.pid, p.state,
	p.parent_pid, p.first_child_pid, p.next_sibling_pid, p.uid); 
```



  ```shell
  cd problem2/jni
  ndk-build
  android avd
  adb push ../libs/armeabi/testModuleARM /data/misc/ptree/callptree
  adb shell 
  cd data/misc/ptree
  ./callptree | tee testscript.txt
  ```

  And we will get the output similar to the testscript.txt

### Problem3 Test your new system call

* Generate a new process and output “StudentIDParent” with PID, then generates its children process output “StudentIDChild” with PID.


* Use execl to execute ptree in the child process，show the relationship between above two process. 



```shell
cd problem3/jni
ndk-build
android avd
adb push ../libs/armeabi/testModuleARM /data/misc/ptree/execlptree
adb shell 
cd data/misc/ptree
./execlptree | tee testscript.txt
```
And we will get the output similar to the testscript.txt

### Problem4 Caesar Encryption Sever 

* Caesar cipher, is one of the simplest and most widely known encryption techniques. During encryption, each letter in the plaintext is replaced by a letter some fixed number of positions down the alphabet. In this problem, we set the number=3 .
* Please develop a Caesar Encryption Server, which receives plaintext from clients and sends the corresponding ciphertext to clients.
* Only the letters need to be encrypted, e.g. How are you? → Krz duh brx?
* The Server can serve at most 2 clients concurrently, more clients coming have to wait.
* The server-side program must be concurrent multi-threaded.
* Client input :q to end the service.
* For simplicity, you can execute one server and multiple clients in one host. 

```shell
#first terminal
cd problem4
g++ -pthread server.cpp -o server
g++ -pthread client.cpp -o client
./server						#1
```

```shell
#second terminal
cd problem4
./client						#2
Are you ok?						#3
:q								#9
```

```shell
#third terminal
cd problem4						
./client						#4
Hello!23333						#5
nihao							#8
:q								#12
```

```shell
#fourth terminal
cd problem4						
./client						#6
something secret				#7
something secret				#10
:q								#11
```

And we will get the output similar to the testscript.txt
