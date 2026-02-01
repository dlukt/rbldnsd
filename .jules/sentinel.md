## 2026-01-24 - Integer Overflow in satoi
**Vulnerability:** The `satoi` function in `rbldnsd.c` lacked integer overflow checks, allowing large numeric inputs to wrap around. This could allow an attacker (or accidental configuration) to pass very large values for UID/GID or ports that wrap to privileged values (e.g., UID 0 or port 53).
**Learning:** Standard library functions like `atoi` are unsafe for security-critical contexts. Custom parsers must always include overflow validation.
**Prevention:** Use `strtol` with checks or implement manual overflow validation (checking against `INT_MAX/10`) in custom parsers.

## 2026-02-17 - Incorrect RDLEN Parsing in Packet Construction
**Vulnerability:** The internal helper macros for packet construction (`nextRR`, `sameDATA`) in `rbldnsd_packet.c` incorrectly treated the RDLENGTH field of a DNS Resource Record as an 8-bit value (reading only the LSB). This would cause the parser to desynchronize (jumping to the wrong offset) if any RR with length > 255 bytes was ever added to the packet, potentially leading to memory corruption or logic errors.
**Learning:** Even when working with "known" internal data structures, strictly adhering to protocol specifications (RDLENGTH is 16-bit) is crucial to prevent latent bugs that become security issues when assumptions change (e.g., allowing larger records).
**Prevention:** Use standard macros or helper functions that correctly handle multi-byte integer parsing (network byte order) rather than ad-hoc pointer arithmetic on byte arrays.

## 2026-03-22 - Out-of-Bounds Read in Packet Parsing Loop
**Vulnerability:** The internal macro `hasRR(c,e)` in `rbldnsd_packet.c` defined as `((c) < (e))` allowed the loop to continue even if `c` was less than 12 bytes from the end of the buffer `e`. This caused subsequent macros `rrDLEN(c)` (reading `c[10]`) and `nextRR(c)` to read uninitialized memory beyond the valid data range, potentially leading to logic errors or reading garbage data.
**Learning:** Checking for mere existence of a pointer within bounds (`c < e`) is insufficient when parsing structures with fixed headers. One must verify the *entire* header size is available (`c + header_size <= e`) before accessing any fields.
**Prevention:** Always check `ptr + sizeof(header) <= end` before dereferencing struct fields in binary parsers.

## 2026-05-20 - Signed Integer Limits on Privileges
**Vulnerability:** The custom integer parser `satoi` in `rbldnsd.c` used signed arithmetic (`int`), preventing the use of UIDs/GIDs larger than `INT_MAX` (2^31-1). This artificial limitation hindered security hardening in environments using large, unprivileged UIDs (e.g., containers, LDAP).
**Learning:** Legacy integer parsing logic often defaults to `int`, silently truncating or rejecting valid large unsigned values required for modern system identifiers.
**Prevention:** Use `strtoul` or explicit `unsigned` arithmetic when parsing system identifiers like UIDs, GIDs, or ports, and validate against `UINT_MAX`.
