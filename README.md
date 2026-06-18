# sys-perf-lab
Compare memory, power, performance tradeoffs between common OS-level design choices

# sys-perf-lab

A systems performance lab for practicing memory/performance trade-off reasoning in C.

The goal is to build small low-level components, benchmark different design choices, and explain the results using systems vocabulary: latency, throughput, memory footprint, allocation overhead, cache locality, context switches, contention, and power-sensitive behavior.

This project is meant to improve practical systems design reasoning, especially for OS/kernel, scheduler, embedded, and low-level software interviews.

---

## Project Goal

This project compares multiple queue and synchronization designs under controlled workloads.

Instead of only implementing correct data structures, the focus is:

* What is the performance benefit?
* What is the memory cost?
* What workload favors this design?
* What metrics prove the trade-off?
* What would I choose in a real system?

The main comparison starts with:

* linked-list queue
* ring-buffer queue
* mutex + condition variable blocking queue
* spin-wait queue
* malloc-per-node allocation
* preallocated/object-pool allocation

---

## Development Environment

Primary development setup:

```text
Mac VS Code
    ↓ Remote SSH
Ubuntu Linux VM
    ↓
compile, test, benchmark, perf stat
```

The Linux VM is the main environment for compiling, testing, and collecting benchmark results.

The Mac can still keep a local clone of the repository, but benchmark results should come from Linux only.

---

## Why Linux VM

Linux is used because it supports standard systems benchmarking tools:

```bash
perf stat
perf record
strace
htop
valgrind
```

The final project report should use Linux benchmark results, not macOS timing results.

macOS can be used for quick editing or reading, but official measurements should be collected in the Linux VM.

---

## Git / SSH Workflow

There are two possible workflows.

### Option 1: Remote SSH workflow

Use VS Code Remote SSH from Mac into the Linux VM.

The project files live inside the Linux VM.

```text
Mac VS Code UI
    ↓
Linux VM files
    ↓
Linux compiler and perf tools
```

Commands are run directly inside the Linux VM:

```bash
make
./bench_queue
perf stat ./bench_queue
git add .
git commit -m "Add ring buffer benchmark"
git push
```

This is the preferred workflow.

### Option 2: Separate Mac and Linux clones

Mac and Linux each have their own copy of the repo.

Mac:

```bash
git add .
git commit -m "Implement queue benchmark"
git push
```

Linux VM:

```bash
git pull
make clean
make
perf stat ./bench_queue
```

This works, but changes must be synced manually through GitHub.

---

## SSH Key Setup

There can be different SSH keys for different directions.

```text
Mac → Linux VM
    uses Mac private key
    Linux VM stores Mac public key in ~/.ssh/authorized_keys

Linux VM → GitHub
    uses Linux VM private key
    GitHub stores Linux VM public key
```

This is normal.

The machine starting the SSH connection needs the private key.
The destination stores the public key.

Recommended Mac SSH config:

```sshconfig
Host ubuntu-vm
    HostName YOUR_VM_IP
    User YOUR_LINUX_USERNAME
    IdentityFile ~/.ssh/YOUR_MAC_VM_KEY
    IdentitiesOnly yes
```

Recommended Linux VM SSH config:

```sshconfig
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/YOUR_LINUX_GITHUB_KEY
    IdentitiesOnly yes
```

---

## Repository Structure

```text
sys-perf-lab/
├── README.md
├── Makefile
├── libqueue/   → the actual queue implementations
├── libbench/   → reusable benchmark helper code
├── tests/      → correctness checks
├── bench/      → performance experiments
├── results/    → benchmark outputs, tables, plots, report
└── docs/       → design notes and trade-off explanations

```

---

## Implementation Plan

### Phase 1: Single-thread queue comparison

Implement:

* linked-list queue
* ring-buffer queue

Benchmark:

* enqueue throughput
* dequeue throughput
* total operations per second
* average latency
* p95 latency
* malloc/free count
* memory usage

Expected trade-off:

```text
Linked-list queue:
    flexible size
    simple dynamic growth
    but more malloc/free calls
    worse cache locality
    extra pointer memory per node

Ring-buffer queue:
    contiguous memory
    no per-item allocation
    better cache locality
    predictable performance
    but fixed capacity and backpressure needed
```

---

### Phase 2: Blocking producer-consumer queue

Implement:

* mutex + condition variable queue
* bounded ring-buffer blocking queue

Benchmark workloads:

```text
1 producer / 1 consumer
2 producers / 2 consumers
4 producers / 4 consumers
8 producers / 8 consumers
```

Measure:

* throughput
* average latency
* p95 latency
* p99 latency
* CPU usage
* context switches
* dropped events if bounded

Expected trade-off:

```text
Mutex + condition variable:
    better power behavior
    threads sleep when no work is available
    but wakeup and context-switch overhead exists
```

---

### Phase 3: Spin-wait comparison

Implement:

* spin-wait version
* compare against mutex + condition variable version

Measure:

* throughput
* latency
* CPU usage
* context switches

Expected trade-off:

```text
Spin-wait:
    can reduce latency for very short waits
    but wastes CPU cycles
    worse for power-sensitive systems

Condition variable:
    better when waiting can be longer
    saves CPU by sleeping
    but has wakeup overhead
```

---

### Phase 4: Allocation strategy

Compare:

* malloc per queue node
* object pool
* preallocated ring buffer

Measure:

* throughput
* malloc/free count
* memory footprint
* latency spikes
* cache behavior if available through perf

Expected trade-off:

```text
Malloc-heavy design:
    flexible
    simple
    but unpredictable under high load
    more allocator overhead
    possible fragmentation

Preallocated design:
    predictable
    fewer allocations
    better hot-path performance
    but uses memory upfront
    requires capacity planning
```

---

## Benchmark Commands

Build:

```bash
make clean
make
```

Run correctness tests:

```bash
./tests/test_queue
./tests/test_threaded_queue
```

Run quick benchmark:

```bash
./bench/bench_queue --quick
```

Run full benchmark:

```bash
./bench/bench_queue
```

Run with perf:

```bash
perf stat ./bench/bench_queue
```

Optional perf counters:

```bash
perf stat -e cycles,instructions,cache-references,cache-misses,context-switches,cpu-migrations ./bench/bench_queue
```

---

## Metrics to Report

Each benchmark should report:

```text
Implementation
Workload
Thread count
Total operations
Throughput
Average latency
p95 latency
p99 latency
Malloc/free count
Memory footprint
CPU usage
Context switches
Cache misses if available
```

Example result table:

```text
Implementation      Throughput     Avg Latency     p95 Latency     Malloc Count
Linked List         1.2M ops/s      140 us          220 us          1,000,000
Ring Buffer         3.8M ops/s       45 us           70 us          1
```

---

## Final Report Structure

The final report should be written in `results/report.md`.

Suggested structure:

```markdown
# Queue Performance Trade-off Report

## Question

When is a ring buffer better than a linked-list queue?

## Hypothesis

A ring buffer should improve throughput and latency because it avoids per-node allocation and improves cache locality.

## Implementations

- linked-list queue
- ring-buffer queue
- mutex/condition-variable blocking queue
- spin-wait queue

## Metrics

- throughput
- average latency
- p95 latency
- p99 latency
- memory footprint
- malloc/free count
- context switches
- CPU utilization
- cache behavior

## Results

Include tables and plots.

## Analysis

Explain why one design performed better or worse.

## Trade-off Summary

Explain when each design should be chosen.

## Interview Takeaway

Summarize the design reasoning in interview language.
```

---

## Interview Takeaway Template

Use this structure when explaining results:

```text
The benefit is X, but the cost is Y.
I would choose this design when Z.
I would measure A, B, and C to confirm.
```

Example:

```text
A ring buffer improved throughput because it avoids per-node allocation and uses contiguous memory, which improves cache locality. The cost is fixed capacity and the need for backpressure logic. I would choose it for bounded producer-consumer queues where predictable performance matters, and I would measure throughput, p95 latency, memory usage, and dropped events.
```

---

## Resume Bullet Later

Possible resume bullet:

```text
Built a C systems performance lab comparing linked-list, ring-buffer, and blocking queue designs under producer-consumer workloads; benchmarked throughput, p95 latency, allocation overhead, CPU usage, and cache behavior on Linux.
```

Stronger version after results:

```text
Benchmarked C queue implementations under multi-threaded producer-consumer workloads, showing how ring-buffer and preallocated designs reduce allocation overhead and improve latency while requiring capacity planning and backpressure handling.
```

---

## First Task

Start with only this:

```text
1. Create repo
2. Add Makefile
3. Implement linked_queue.c
4. Implement ring_queue.c
5. Implement test_queue.c
6. Implement bench_queue.c
7. Print throughput and average latency
8. Run perf stat on Linux
9. Write first result table
```

Do not start with eBPF or advanced kernel topics yet.

The first goal is to produce one clean, measurable trade-off:

```text
linked list vs ring buffer
```

Once that is done, add multithreading and synchronization trade-offs.
