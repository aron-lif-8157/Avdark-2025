#include "atomic_counters.hpp"




#include "atomic_counters.hpp"
#include <mutex>

atomic_counter_lock::atomic_counter_lock()
    : atomic_counter()
    , m_value(0)
    , m_lock() {
}

int atomic_counter_lock::increment() {
    std::lock_guard<std::mutex> guard(m_lock);  // Lock is acquired here
    int prev_value = m_value;
    m_value = m_value + 1;
    return prev_value;  // Lock is released when guard goes out of scope
}

int atomic_counter_lock::decrement() {
    std::lock_guard<std::mutex> guard(m_lock);
    int prev_value = m_value;
    m_value = m_value - 1;
    return prev_value;
}

void atomic_counter_lock::set(int value) {
    std::lock_guard<std::mutex> guard(m_lock);
    m_value = value;
}

int atomic_counter_lock::get() {
    std::lock_guard<std::mutex> guard(m_lock);
    return m_value;
}

/* --- old implementation


atomic_counter_lock::atomic_counter_lock()
    : atomic_counter()
    , m_value(0)
    , m_lock() {
}

int atomic_counter_lock::increment() {
    // locks here
    m_lock.lock();
    int prev_value = m_value;
    m_value = m_value + 1;
    m_lock.unlock();
    return prev_value;
}

int atomic_counter_lock::decrement() {
    // locks here
    m_lock.lock();
    int prev_value = m_value;
    m_value = m_value - 1;
    m_lock.unlock();
    return prev_value;
}

void atomic_counter_lock::set(int value) {
    // locks here
    m_lock.lock();
    m_value = value;
    m_lock.unlock();
}

int atomic_counter_lock::get() {
    // locks here
    m_lock.lock();
    int return_value = m_value;
    m_lock.unlock();
    return return_value;
}
*/