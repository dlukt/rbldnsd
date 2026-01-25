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

## 2026-01-23 - Signal Handling Race Condition
**Vulnerability:** A race condition in signal handling allowed concurrent signals to overwrite the `signalled` state variable, causing signals (like `SIGTERM` or `SIGHUP`) to be lost.
**Learning:** Signal handlers using `read-modify-write` operations on shared variables must block other signals to ensure atomicity. `volatile` is not enough for RMW operations.
**Prevention:** Always use `sa_mask` in `sigaction` to block all handled signals during the execution of any signal handler, and ensuring all relevant signals are included in the mask.

## 2026-01-24 - Integer Overflow in satoi
**Vulnerability:** The `satoi` function in `rbldnsd.c` lacked integer overflow checks, allowing large numeric inputs to wrap around. This could allow an attacker (or accidental configuration) to pass very large values for UID/GID or ports that wrap to privileged values (e.g., UID 0 or port 53).
**Learning:** Manual string-to-integer conversion loops must always verify that `n * 10 + digit` does not exceed `INT_MAX` before performing the operation, as signed integer overflow is undefined behavior in C.
**Prevention:** Use standard library functions like `strtol` (with range checks) or implement explicit overflow detection logic: `if (n > INT_MAX / 10 || (n == INT_MAX / 10 && d > INT_MAX % 10))`.

## 2026-01-25 - Zone Dump Injection via Unescaped Strings
**Vulnerability:** `ds_generic_dump` and `dump_a_txt` failed to properly escape quotes and backslashes when dumping TXT records to BIND format. This allowed Zone File Injection where a crafted record could inject new lines/records or corrupt the zone data when the dump was reloaded.
**Learning:** When generating structured output (like BIND zone files), simply wrapping strings in quotes is insufficient. Special characters (quotes, backslashes) must be escaped to prevent the consumer from misinterpreting the boundaries.
**Prevention:** Implement and use a dedicated `write_escaped_string` helper function that strictly escapes `"` and `\` characters according to the target format specification (RFC 1035).
