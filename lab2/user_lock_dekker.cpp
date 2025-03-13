#include "user_locks.hpp"

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0] = m_flag[1] = false;
    m_turn = 0;
}

void user_lock_dekker::lock(int thread_id) {
    m_flag[thread_id] = true;                   // Då variabeln är atomic blir denna händelse också atomic
    while (m_flag[1 - thread_id]) {             // Kollar om den andra flaggan är true
        if (m_turn != thread_id) {              // Om det inte är min tur
            m_flag[thread_id] = false;          // Sätter min flagga till false
            while (m_turn != thread_id) {       // Väntar på att det blir min tur
                // Busy wait                    
            }
            m_flag[thread_id] = true;
        }
    }

}


void user_lock_dekker::unlock(int thread_id) {
    m_turn = 1 - thread_id;
    m_flag[thread_id] = false;
}