
# Task A
## Task 1
**Compile and run the atomic counter test using the nosync counter implementation: `./test_atomic_counter nosync`. Do not modify the nosync implementation. Does it work? Why or why not?**

**Kör med ./ test_atomic_counter nosync**

#### Singlethreaded tests
single thread fungerar då vi inte har fler trådar och då behöver vi inte någon sync

#### Multithreaded tests
multi thread fungerar inte då vi kör flera trådar utan sync och det introducerar race conditions och liknande

## Task 2
**Implentera ett lås för att synca**

### Single thread
Fungerar eftersom det endast är en tråd och inte behöver någon typ av sync

### multithread
Enkelt mutex lås. Vi låser tråden innan vi laddar in variabler och modifierar dem.
Viktigt att låsa upp tråden innan vi returnerar värde.

## Task 3
**Implement the atomic inc/dec-based atomic counter**

Vi använder atomic loads för att ha säkra ladningar som förhindrar race conditions
Använder fetch.ass(1) för att säkert ladda och addera 1 till m_value.
returnerar tidigare värdet

## Task 4
**atomic CAS-based**

Det finns två olika versioner av compare_echange, Strong och Weak.

Strong är bättre att använda när vi gör en enstaka checks, den baserar om sin pass or fail den faktiskt jämnförelsen av värdet.

Weak kan misslyckas även om värdena är samma men fungerar bra när vi kör kontinuerliga checkar. den tillåter en mer avslapnad memory modell som kan vara mer effektiv. 


## Task 5
We are tasked to compare the different speed of each synchronization implementation.

The more operations per second the better performance is.
We can se that :
- the best performing implementation is **atomic increase / decrease.**
	- This is resonable since fetch_add and fetch_sub are both implemented as a single atomic instruction.
- Second best performing implementation is **atomic compare**
	- This is also resonable since it loops a atomic check and only adds or decrease the value if the check is true. This adds some overhead to the operations.
- Third and worst performing implementation is the lock implementation.
	- This is also resonable since locks adds a lot of overhead.
	- when a thread tries so acquire a lock that is already helt it will be blocked and wait until it's awoken by a change of the locks status. This adds a lot of overhead.
	- Many lock implementations involve system calls or kernel-level synchronization primitives making them slower then atomic fetch_add and atomic fetch_sub since these avoid os/kernel system level calls

# Task B

## T1
Det fungerar vilket är förväntat.
vi använder oss av den inbyggda mutex låset i C som garanterar mutual exclusion. Detta innebär att endast en tråd får tillgång till den kritiska delen av koden.

## T2
Det fungerar, våra två variabler flag och turn är båda atomic so varje uppdatering till dessa variabler blir utförda atomic.
Busywaiting används för att tråden ska bevaka när den får tillgång till att utföras vilket vi vet att vi kommer får för eller senare.

## T3
**Language memory model**
Memory model för c++ är sequential consistency som standard om inget annat sägs.
Sequential consisteny garanterar att trådarna upplever alla sin operation från top till bot.
Alltså att alla store och loads sker i den ordningen de skrivs ut / används

**Memory model of hardware**
Detta beror såklart på hårdvaran som koden utförs på men på den datorn som vi har kört koden på använder sig av x86 arkitekturen.
 x86 arkitekturen avndäner sig av total sotre ordering.
 Total store ordering använder sig av en writeback buffer så writebacks sker inte alltid som den ser ut att göra i koden.

the memory ordering model på hårdvaran påverkar inte programmet egentligen då c++ har sin egen memory ordering model.

**Which memory model are we programming for here?**
Vi programmerar för c++ meory modelen vilket innebär att vi programmerar för sequential consictency.


## T4
Om vi hade haft en annan memory ordering modell så hade den varit mer relaxed. Detta hade kunnat innebära att vi behövt implementera mer synkronisering i koden.

## T5
Den snabbare implementeringen är de inbyggda mutex låsen.
Mutex lås är implementerade på en OS nivå där synxhronizering är långsammare än atomic operations men snabbare än busy waiting.
Eftersom vår dekker implementation är beroende av busy waiting så är det förväntat att den kommer vara långsamare trots att den använder sig av atomic operations som vanligtvis är snabbare än mutex

Busy waiting är så långsamt eftersom den använder CPU:n för att kontinuerligt kolla om det skett någon förändring för lås variabeln.
Mutex lås sätts i sleep tills en förändring observeras av CPU:n då skickas en signal för att väcka mutex låset.



# Bonus

# Lock guards

## Task 1
