- [ ] A
	- [x] 1
	- [x] 2
	- [x] 3
	- [x] 4
	- [ ] 5
	- [ ] comment code
- [ ] B
	- [ ] 1
	- [ ] 2
	- [ ] 3
	- [ ] 4
	- [ ] 5

# Task A
## Task 1
**Compile and run the atomic counter test using the nosync counter implementation: `./test_atomic_counter nosync`. Do not modify the nosync implementation. Does it work? Why or why not?**

#### Singlethreaded tests
Single thread tests succeed due to it only being a single thread operating on it, meaning there can be no race conditions since there is only one thread executing the instructions per the program counter

#### Multithreaded tests
These tests fail since we have no synchronization and data races occur between the threads 

## Task 5
We are tasked to compare the different speed of each synchronization implementation.

The more operations per second the better performance is.
We can se that :
- the best performing implementation is atomic increase / decrease.
	- This is resonable since fetch_add and fetch_sub are both implemented as a single atomic instruction.
- Second best performing implementation is atomic compare
	- This is also resonable since it loops a atomic check and only adds or decrease the value if the check is true. This adds some overhead to the operations.
- Third and worst performing implementation is the lock implementation.
	- This is also resonable since locks adds a lot of overhead.
	- when a thread tries so acquire a lock that is already helt it will be blocked and wait until it's awoken by a change of the locks status. This adds a lot of overhead.
	- Many lock implementations involve system calls or kernel-level synchronization primitives making them slower then atomic fetch_add and atomic fetch_sub since these avoid os/kernel system level calls

# Task B

## T1
It works as expected. We are using a std::mutex which is a mutual exclusion lock meaning only one thrad have access to the ritical setion at a time. Mutex locks ensure proper synchronization between threads.

## T2
It works. Our two variables, flag and turn, are both atomic so every update to these value will inhearently be atomic. the busywaiting is needed to allow for execution when the thread is available which it will be sooner or later it will se the value change and execute it's action.

## T3
**Language memory model**
the memory model for our c++ is Sequential Consistency by default when using std::atomic

**Memory model of hardware**
This of course depends on the hardware the code is being run on but in our case we ran on a x86 architecture.
This means that the hardware Memory model used is Total store ordering or TSO. This doesn't really affect anything since the program is relying on the c++ memory model.

**Which memory model are we programming for here?**
We are programming for the c++ Memory model used for atomic operations, so we are programming for sequential consictency

## T4


## T5
The faster implementation is using the mutex locks.
Mutex locks are implemented using OS-level synchronization which is slower than atomic operatiosn but faster then busy waiting. Since our implementation of dekkers algorithm uses busy waiting it will always be slower then mutex locks.
The difference between busy waiting and mutex locks is that busy waiting will use the cpu to do it's check constantly while mutex locks get put to sleep when waiting and then awoken.
One could almost list it like this.
Fastest: atomic operations.
Middle: mutex.
Slowest: busy waiting