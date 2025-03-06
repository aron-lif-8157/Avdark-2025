#include "atomic_counters.hpp"

atomic_counter_atomic_incdec::atomic_counter_atomic_incdec()
    : atomic_counter()
    , m_value(0) {
}

int atomic_counter_atomic_incdec::increment() {
    // using atomic fetch_add operations
    int prev_value = m_value.load();
    m_value.fetch_add(1);
    return prev_value;
}

int atomic_counter_atomic_incdec::decrement() {
    // using atomic fetch_sub operations
    int prev_value = m_value.load();
    m_value.fetch_sub(1);
    return prev_value;
}

void atomic_counter_atomic_incdec::set(int value) {
    // using atomic store operations
    m_value.store(value);
}

int atomic_counter_atomic_incdec::get() {
    // using atomic load operations
    return m_value.load();
}
