## 2026-01-19 - Packet Parsing Integer Underflow
**Vulnerability:** Integer underflow in `parsequery` led to incorrect bounds checking for short DNS packets (< 5 bytes), potentially causing out-of-bounds reads.
**Learning:** Implicit assumptions about packet length (e.g., `qlen - 5`) without explicit checks are dangerous in C, especially when dealing with untrusted network input.
**Prevention:** Always validate input length against minimum expected size (header + minimum payload) before performing arithmetic operations or parsing.

## 2026-01-19 - vssprintf Buffer Corruption
**Vulnerability:** `vssprintf` (custom snprintf wrapper) allowed buffer overflow if passed a negative buffer size (casted to large `size_t` by `vsnprintf`) or buffer underrun (accessing `buf[-1]`) if passed size 0.
**Learning:** Wrapper functions around standard library calls must strictly validate inputs, especially when types differ (`int` vs `size_t`). `vsnprintf` behavior on error or zero size must be handled carefully.
**Prevention:** Add explicit `if (size <= 0) return 0;` checks in string formatting wrappers to ensure robustness against calculation errors in callers.

## 2026-01-19 - DNS Query Strict Validation
**Vulnerability:** Missing validation of `ancount`, `nscount`, and `arcount` in `parsequery` allowed malformed queries (e.g., non-empty Answer section) to be processed.
**Learning:** Even if extra fields are ignored, failing to validate that they are empty (as required by RFC for Queries) violates the principle of "Fail Securely" and strict protocol compliance.
**Prevention:** Explicitly validate that unused sections (Answer, Authority) are empty in query parsers to reject malformed or potentially malicious packets early.
