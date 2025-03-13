#pragma once

#include "common/types.hpp"
#include "pool.hpp"

struct PoolDiagnostics {
    // Usage
    usize peak_allocation_count;
    usize total_allocations;
    usize total_frees;
    usize reallocation_count;

    // Memory
    usize peak_memory_used;
    usize current_memory_used;

    // Performatce
    double average_allocation_time; // Nano-seconds
    double max_allocation_time;     // Nano-seconds
    usize fragmentation_count;      // Number of gaps between 0 and extent

    // Error tracking
    usize invalid_handle_uses;
    usize out_of_bounds_attempts;
    usize double_free_attempts;
};

struct DiagnosticPool {
    Pool pool;
    PoolDiagnostics diagnostics;
    const char *label;

    // struct AllocationRecord {
    //   usize index;
    //   const char *allocation_site; // Source file:line
    //   f64 time;
    //   bool is_valid;
    // };

    // std::vector<AllocationRecord> history;
};
