## 2024-05-22 - [Optimization Selection]
**Learning:** `memcmp` optimization in `btrie.c` was already present, despite initial scan. Always verify current code state against memory or hypothesis. Replacing simple arithmetic macros like `HEXVAL` with 256-byte tables might not yield performance benefits on modern CPUs due to cache pressure vs branch prediction efficiency. However, removing integer division in `ip4atos` yielded significant (~1.4x) speedup.
**Action:** Verify "obvious" optimizations are not already applied. Benchmark small lookups vs arithmetic before committing to table-based optimizations.

## 2024-05-23 - [Loop Unrolling in String Processing]
**Learning:** Manual loop unrolling (4-way) in byte-processing loops like `dns_dntol` can still yield measurable speedups (~15%) even on modern compilers with `-O2`, especially for tight loops involving table lookups.
**Action:** Look for other byte-level string processing loops (e.g., in `dns_dntop`) that could benefit from unrolling.
