#include "atomic_counters.hpp"

atomic_counter_atomic_incdec::atomic_counter_atomic_incdec()
    : atomic_counter()
    , m_value(0) {
}

int atomic_counter_atomic_incdec::increment() {
    // Using fetch_add with relaxed memory ordering
    int prev_value = m_value.fetch_add(1, std::memory_order_relaxed);
    return prev_value;
}

int atomic_counter_atomic_incdec::decrement() {
    // Using fetch_sub with relaxed memory ordering
    int prev_value = m_value.fetch_sub(1, std::memory_order_relaxed);
    return prev_value;
}

void atomic_counter_atomic_incdec::set(int value) {
    // Using store with relaxed memory ordering
    m_value.store(value, std::memory_order_relaxed);
}

int atomic_counter_atomic_incdec::get() {
    // Using load with relaxed memory ordering
    return m_value.load(std::memory_order_relaxed);
}