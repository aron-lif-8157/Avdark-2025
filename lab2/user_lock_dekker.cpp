#include "user_locks.hpp"
#include <atomic>

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0] = m_flag[1] = false;
    m_turn = 0;
}

void user_lock_dekker::lock(int thread_id) {
    m_flag[thread_id] = true; // Atomic store, ensures visibility (Då variabeln är atomic blir denna händelse också atomic)
    //! Memory fence needed here to ensure this store is visible before proceeding
    //! vi måste garantera att m_turn inte ändras innan vi har satt m_flag till true
    
    while (m_flag[1 - thread_id]) {     // Check if other thread wants to enter
        if (m_turn != thread_id) {      // If it's not my turn
            m_flag[thread_id] = false; // Release intention to enter
            //! Memory fence needed here to ensure store is visible before waiting
            //! vi måste garantera att m_flag har satts till false innan vi går in i while-loopen
            
            while (m_turn != thread_id) {   // Wait for my turn
                                            // Busy wait
            }
            m_flag[thread_id] = true;       // Re-acquire intention to enter
            //! Memory fence needed here to prevent reordering of operations
            //! vi måste garantera att m_flag har satts till true innan vi går ut ur while-loopen
        }
    }
}

void user_lock_dekker::unlock(int thread_id) {
    //! Memory fence needed here to ensure all prior stores are visible before releasing lock
    //! vi måste garantera att alla tidigare operationer har utförts innan vi släpper låset
    m_turn = 1 - thread_id;     // Set turn to the other thread
    m_flag[thread_id] = false;  // Release the lock
}