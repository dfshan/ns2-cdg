# TCP CDG algorithm in ns-2
The TCP CDG algorithm is ported from GNU/Linux 4.12.10.

## How to use it
1. Install ns-2.35
2. Copy the code in "tcp" directory to ns source tree
3. Add an entry to `Makefile` and `Makefile.in`:

```makefile
OBJ_C = \
...
tcp/linux/src/tcp_cdg.o \
...
```

4. Compile the source
5. In the tcl script, enable TCP CDG by

```tcl
set send_agent [new Agent/TCP/Linux]
$send_agent set timestamps_ true
$send_agent select_ca cdg
$send_agent set bugFix_ts_ 1
```
