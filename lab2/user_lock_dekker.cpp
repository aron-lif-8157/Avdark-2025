#include "user_locks.hpp"
#include <atomic>

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0].store(false, std::memory_order_seq_cst);
    m_flag[1].store(false, std::memory_order_seq_cst);
    m_turn.store(0, std::memory_order_seq_cst);
}

void user_lock_dekker::lock(int thread_id) {
    m_flag[thread_id].store(true, std::memory_order_seq_cst); // Ensure visibility

    while (m_flag[1 - thread_id].load(std::memory_order_seq_cst)) { // Check other thread
        if (m_turn.load(std::memory_order_seq_cst) != thread_id) { // Not my turn
            m_flag[thread_id].store(false, std::memory_order_seq_cst); // Back off
            
            while (m_turn.load(std::memory_order_seq_cst) != thread_id) {
                // Busy wait
            }
            
            m_flag[thread_id].store(true, std::memory_order_seq_cst); // Reassert intent
        }
    }
}

void user_lock_dekker::unlock(int thread_id) {
    m_turn.store(1 - thread_id, std::memory_order_seq_cst); // Set turn for other thread
    m_flag[thread_id].store(false, std::memory_order_seq_cst); // Release lock
}