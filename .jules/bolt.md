## 2024-05-22 - [Optimization Selection]
**Learning:** `memcmp` optimization in `btrie.c` was already present, despite initial scan. Always verify current code state against memory or hypothesis. Replacing simple arithmetic macros like `HEXVAL` with 256-byte tables might not yield performance benefits on modern CPUs due to cache pressure vs branch prediction efficiency. However, removing integer division in `ip4atos` yielded significant (~1.4x) speedup.
**Action:** Verify "obvious" optimizations are not already applied. Benchmark small lookups vs arithmetic before committing to table-based optimizations.

## 2024-05-23 - [Loop Unrolling in String Processing]
**Learning:** Manual loop unrolling (4-way) in byte-processing loops like `dns_dntol` can still yield measurable speedups (~15%) even on modern compilers with `-O2`, especially for tight loops involving table lookups.
**Action:** Look for other byte-level string processing loops (e.g., in `dns_dntop`) that could benefit from unrolling.

## 2024-05-24 - [Lookup Table for Character Classification]
**Learning:** Replacing `switch`/`if` logic with a 256-byte lookup table in `dns_dntop` yielded a ~35% speedup. Even for simple classification (safe/special/octal-escape), eliminating branches in tight string processing loops is highly effective.
**Action:** Identify other character classification loops that rely on multiple comparisons and consider replacing them with precomputed tables.

## 2024-05-25 - [Table Lookup vs Arithmetic in String Escaping]
**Learning:** Replacing integer division/modulo with a precomputed 3-byte lookup table (`dec_octets`) for decimal escaping in `dns_dntop` yielded a ~2.5x speedup for non-printable characters. Also, replacing `switch` with explicit `if/else` checks for the common case proved faster than the compiler-generated jump table/comparisons for this specific distribution.
**Action:** When optimizing string escaping or formatting loops, prioritize table lookups over arithmetic, and verify if `switch` statements are generating optimal code for the expected data distribution.

## 2024-05-26 - [Branchless String Copying]
**Learning:** For small, fixed-width string conversions (like IP octets), unconditional copying (e.g., via `memcpy`) from a null-padded lookup table is significantly faster (~1.8x) than conditional logic, provided the destination buffer is large enough to handle the maximum write.
**Action:** When optimizing string serialization of small integers or fixed-width types, look for opportunities to replace branches with unconditional writes using padded lookup tables.

## 2024-05-27 - [Branchless Bit Extraction via Padding]
**Learning:** `extract_bits` in `btrie.c` contained a conditional branch `if (shift + nbits > 8)` to safely read the next byte. On 64-bit systems (where `TBM_STRIDE` is 5), this branch is often mispredicted. By ensuring all input buffers (keys) are padded with at least 1 byte (rounded up to 8 for alignment) at all call sites, we can unconditionally read the next byte `(prefix[idx] << 8) | prefix[idx + 1]`, eliminating the branch.
**Action:** When optimizing low-level bit/byte processing loops, check if conditional boundary checks can be eliminated by padding the input data structure, but be meticulous about updating ALL call sites to provide the padded buffer.

## 2024-05-28 - [Hex Escape Sequence Greediness]
**Learning:** In C string literals, `\x` is greedy and consumes all following valid hex digits (e.g., `\x07example` is parsed as `\x07e` followed by `x`, `a`...). This caused severe benchmark data corruption where domain name lengths were interpreted as 126 instead of 7.
**Action:** When constructing test strings with hex escapes, always break the string literal (e.g., `"\x07" "example"`) or use octal escapes (e.g., `\007example`) to prevent accidental merging with subsequent characters.
