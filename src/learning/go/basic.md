# Golang

## GMP

- M for machine, represents an OS thread.
- G represents a goroutine. (includes the stack, the instruction pointer and other information important for scheduling goroutines, like any channel it might be blocked on)
- P for processor, represents a context for scheduling.
