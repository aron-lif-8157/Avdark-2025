#include "user_locks.hpp"

user_lock_clh::user_lock_clh()
    : user_lock()
    , m_tail(&m_cells[2]) {  // Initially, tail points to the third cell (unlocked state)
    
    m_local[0].local_cell = &m_cells[0];
    m_local[0].previous = nullptr;
    m_local[1].local_cell = &m_cells[1];
    m_local[1].previous = nullptr;

    // Initialize all cells to "false" (unlocked state)
    m_cells[0] = false;
    m_cells[1] = false;
    m_cells[2] = false;
}

void user_lock_clh::lock(int thread_id) {
    local_l *l = &m_local[thread_id];

    // Step 1: Set our node's cell to "true" (indicating we want the lock)
    l->local_cell->store(true, std::memory_order_relaxed);

    // Step 2: Atomically swap the tail to point to our node and get the previous node
    l->previous = m_tail.exchange(l->local_cell, std::memory_order_acquire);

    // Step 3: Spin on the previous node until it becomes "false"
    while (l->previous->load(std::memory_order_acquire)) {
        // Busy wait (spinning)
    }
}

void user_lock_clh::unlock(int thread_id) {
    local_l *l = &m_local[thread_id];

    // Step 1: Release our lock by setting our node's cell to "false"
    l->local_cell->store(false, std::memory_order_release);

    // Step 2: (Optional) Reuse the previous node's cell for future locks
    // This step is not strictly necessary for correctness but can improve performance
    // by reusing cells instead of allocating new ones.
    l->local_cell = l->previous;
}