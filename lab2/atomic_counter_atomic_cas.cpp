#include "atomic_counters.hpp"

atomic_counter_atomic_cas::atomic_counter_atomic_cas()
    : atomic_counter()
    , m_value(0) {
}

int atomic_counter_atomic_cas::increment() {
    // using atomic compare and exchange (CAS) operations
    int prev_value = m_value.load();
    while (!m_value.compare_exchange_weak(prev_value, prev_value+1)) {
    }
    return prev_value;
}

int atomic_counter_atomic_cas::decrement() {
    // using atomic compare and exchange (CAS) operations
    int prev_value = m_value.load();
    while (!m_value.compare_exchange_weak(prev_value, prev_value-1)) {
    }
    return prev_value;
}

void atomic_counter_atomic_cas::set(int value) {
    // using atomic store operations
    m_value.store(value);
}

int atomic_counter_atomic_cas::get() {
    // using atomic load operations
    return m_value.load();
}
