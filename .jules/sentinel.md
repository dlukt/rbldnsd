## 2026-01-19 - Packet Parsing Integer Underflow
**Vulnerability:** Integer underflow in `parsequery` led to incorrect bounds checking for short DNS packets (< 5 bytes), potentially causing out-of-bounds reads.
**Learning:** Implicit assumptions about packet length (e.g., `qlen - 5`) without explicit checks are dangerous in C, especially when dealing with untrusted network input.
**Prevention:** Always validate input length against minimum expected size (header + minimum payload) before performing arithmetic operations or parsing.
