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

