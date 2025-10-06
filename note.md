# C++ Threading & Concurrency — Complete Guide

> A practical, example-driven reference covering `std::thread`, synchronization (`std::mutex`, `std::condition_variable`), atomics, memory orders and fences, data races, deadlocks, and best practices.

---

## Contents

1. Introduction & Mental model
2. `std::thread`: creating and managing threads
3. Thread lifetime: `join`, `detach`, exceptions
4. Race conditions & undefined behavior
5. Mutexes: `std::mutex`, `std::recursive_mutex`, `std::timed_mutex`
6. Locking helpers: `std::lock_guard`, `std::unique_lock`, `std::scoped_lock`
7. Condition variables: `std::condition_variable`, `std::notify_*` patterns
8. Atomics: `std::atomic<T>`, common operations
9. Memory orders: `seq_cst`, `acquire/release`, `relaxed`, `consume`
10. Memory fences: `atomic_thread_fence`
11. Lock-free programming & common patterns
12. Deadlocks, livelocks, priority inversion — avoidance
13. Performance considerations
14. Modern patterns: task-based concurrency, `std::async`, executors
15. Checklists & best practices
16. Exercises and further reading

---

## 1. Introduction & mental model

Threads run concurrently and may interleave in any order. When multiple threads access shared memory without proper synchronization, the program has **data races**, leading to **undefined behavior**.

**Rule:** Any shared object accessed by multiple threads where at least one access is a write must be synchronized (mutexes, atomics, or other synchronization). If not, UB.

Two mechanisms to coordinate threads:

* **Mutual exclusion** (mutexes) — serialize access to shared data.
* **Atomic operations** and **memory ordering** — coordinate without full mutexes, possibly lock-free.

Think in terms of *happens-before* relations: synchronization creates ordering guarantees.

---

## 2. `std::thread`: creating and managing threads

```cpp
#include <thread>
#include <iostream>

void worker(int id) {
    std::cout << "worker " << id << " running\n";
}

int main() {
    std::thread t(worker, 1); // start thread
    // do work in main thread
    t.join(); // wait until t finishes
}
```

**Notes:**

* Threads are RAII-like but `std::thread` is *not* joinable by destructor. If a `std::thread` is still joinable when it is destroyed, `std::terminate()` is called.
* A moved-from `std::thread` is empty (non-joinable).

---

## 3. Thread lifetime: `join`, `detach`, exceptions

* `join()` blocks the calling thread until the thread finishes. Preferred when you need the result or orderly shutdown.
* `detach()` makes the thread independent; resources cleaned up when it finishes. Use sparingly — it's easy to leak resources or access destroyed objects.

**Example safety pattern using RAII:**

```cpp
struct JoinGuard {
    std::thread &t;
    ~JoinGuard() { if (t.joinable()) t.join(); }
};

void f() {
    std::thread t([]{ /*...*/ });
    JoinGuard g{t};
    // any early return will join
}
```

If an exception escapes a function and you haven't joined/detached the thread, the destructor of `std::thread` will call `std::terminate()`.

---

## 4. Race conditions & undefined behavior

**Bad example (data race):**

```cpp
int counter = 0;

void inc() { ++counter; }

int main() {
    std::thread a(inc);
    std::thread b(inc);
    a.join(); b.join();
}
```

This is a data race because `counter` is concurrently modified without synchronization. Fix with `std::atomic<int>` or a mutex.

---

## 5. Mutexes

Basic API:

* `std::mutex` — non-recursive
* `std::recursive_mutex` — allows same thread to lock multiple times
* `std::timed_mutex` / `std::recursive_timed_mutex` — timed try-lock variants

**Example:**

```cpp
#include <mutex>
#include <vector>

std::mutex m;
std::vector<int> shared;

void push_value(int v) {
    std::lock_guard<std::mutex> lk(m);
    shared.push_back(v);
}
```

Don't hold locks across blocking calls or long-running operations.

---

## 6. Locking helpers

* `std::lock_guard<Mutex>` — simple RAII lock, cannot unlock manually.
* `std::unique_lock<Mutex>` — more flexible: can defer locking, unlock, relock, transfer ownership.
* `std::scoped_lock` (C++17) — lock multiple mutexes safely without deadlock (uses `std::lock`).

**Deadlock avoidance with `std::scoped_lock`:**

```cpp
std::mutex m1, m2;

void f() {
    std::scoped_lock lk(m1, m2); // locks both without deadlock
    // safe access
}
```

**Manual locking with `unique_lock`:**

```cpp
std::unique_lock<std::mutex> lk(m, std::defer_lock);
// do work
lk.lock();
// critical section
lk.unlock();
```

---

## 7. Condition variables

Use `std::condition_variable` to wait for a condition; always in conjunction with a mutex and a predicate.

```cpp
#include <condition_variable>
#include <queue>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;

void producer() {
    {
        std::lock_guard<std::mutex> lk(m);
        q.push(42);
    }
    cv.notify_one();
}

void consumer() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{ return !q.empty(); }); // predicate form
    int v = q.front(); q.pop();
}
```

Important: `wait` may wake spuriously; always use predicate form or loop.

`notify_one()` wakes one waiter; `notify_all()` wakes all.

---

## 8. Atomics

`std::atomic<T>` for integral/floating/pointer/`bool` and `std::atomic<T*>`. Some types are not lock-free.

Basic ops:

* `store`, `load`
* `exchange`, `compare_exchange_weak` / `compare_exchange_strong`
* `fetch_add`, `fetch_sub`, etc.

Example: atomic counter

```cpp
#include <atomic>

std::atomic<int> ac{0};

void inc() { ac.fetch_add(1, std::memory_order_relaxed); }
```

Use atomics to avoid mutexes when operations are simple and inherently atomic.

---

## 9. Memory orders

Memory orders control visibility and ordering of atomic operations across threads.

* `std::memory_order_seq_cst` (sequentially consistent): default, strongest ordering; easy to reason about.
* `std::memory_order_acquire` / `std::memory_order_release`: partial ordering; pair them for synchronization.
* `std::memory_order_relaxed`: no ordering, only atomicity — useful for counters or stats where ordering doesn't matter.
* `std::memory_order_consume`: intended for dependency ordering, but poorly supported — prefer `acquire`.

**Acquire-release example (producer-consumer):**

```cpp
std::atomic<int*> data_ptr{nullptr};
int data;

void producer() {
    data = 100; // non-atomic write
    data_ptr.store(&data, std::memory_order_release);
}

void consumer() {
    int *p;
    while ((p = data_ptr.load(std::memory_order_acquire)) == nullptr) {}
    // after acquire, we see initialized data
    int v = *p;
}
```

`release` on store + `acquire` on load forms a *synchronizes-with* relation: the write to `data` happens-before the load that observes `data_ptr`.

**Seq_cst** also guarantees a single global total order for seq_cst operations — simplifies reasoning but may be slower.

**Relaxed** example for a statistical counter:

```cpp
std::atomic<size_t> hits{0};
void record_hit() { hits.fetch_add(1, std::memory_order_relaxed); }
```

No ordering needed; just atomicity.

---

## 10. Memory fences

`std::atomic_thread_fence` introduces ordering constraints without a specific atomic variable.

* `std::atomic_thread_fence(std::memory_order_acquire)`
* `std::atomic_thread_fence(std::memory_order_release)`
* `std::atomic_thread_fence(std::memory_order_seq_cst)`

**Use case (rare):**

A fence is useful when you have non-atomic writes and a separate atomic flag that you use to publish those writes. An acquire-release fence can be used instead of acquire-release on the atomic variable in some low-level patterns, but it's easy to get wrong. Prefer using atomic store/load with release/acquire semantics on a publishing atomic variable.

**Example (pattern using atomic fence):**

```cpp
int data;
std::atomic<bool> ready{false};

void producer() {
    data = 42;                 // plain write
    std::atomic_thread_fence(std::memory_order_release);
    ready.store(true, std::memory_order_relaxed);
}

void consumer() {
    while (!ready.load(std::memory_order_relaxed)) {}
    std::atomic_thread_fence(std::memory_order_acquire);
    // now it's safe to read data
    use(data);
}
```

This is subtly equivalent to storing `ready` with `release` and loading with `acquire`, but fences separate the ordering from the atomic variable. Use atomic ops with acquire/release unless you have a specialized reason for fences.

---

## 11. Lock-free programming & common patterns

Lock-free doesn't mean simple. Typical patterns:

* **Atomic flags**: spin-wait with `atomic_flag`.
* **Double-checked locking**: only safe with proper memory ordering or once-only initialization.

**Thread-safe lazy initialization (C++11 magic statics):**

```cpp
Foo& instance() {
    static Foo f; // initialized once in a threadsafe manner
    return f;
}
```

**Double-checked locking (correct):**

```cpp
std::atomic<Widget*> inst{nullptr};
std::mutex m;

Widget* get() {
    Widget* p = inst.load(std::memory_order_acquire);
    if (!p) {
        std::lock_guard<std::mutex> lk(m);
        p = inst.load(std::memory_order_relaxed);
        if (!p) {
            p = new Widget();
            inst.store(p, std::memory_order_release);
        }
    }
    return p;
}
```

Key: use acquire on the load that reads the published pointer and release on the store that publishes it — ensures constructed object is visible.

---

## 12. Deadlocks, livelocks, priority inversion — avoidance

**Deadlock**: cyclic waiting for locks. Avoid by:

* Lock ordering: always acquire locks in a consistent global order.
* Use `std::scoped_lock` to lock multiple mutexes atomically.
* Prefer fine-grained locking only where necessary.

**Livelock**: threads keep yielding work to each other but make no progress. Often needs backoff or redesign.

**Priority inversion**: a low-priority thread holds a lock needed by a high-priority thread. OS-level priority inheritance or redesign should be used in real-time systems.

---

## 13. Performance considerations

* Mutexes are typically cheap when uncontended; contention causes cost.
* Atomics with `relaxed` or `acquire/release` are cheaper than seq_cst, but harder to reason about.
* Spinning (busy wait) is OK for short waits; use `std::this_thread::yield()` or `std::this_thread::sleep_for()` for longer waits.
* Profile before optimizing.

**Example backoff spin loop:**

```cpp
std::atomic_flag lock = ATOMIC_FLAG_INIT;

void lock_spin() {
    while (lock.test_and_set(std::memory_order_acquire)) {
        for (int i = 0; i < 100; ++i) __builtin_ia32_pause(); // or std::this_thread::yield()
    }
}
```

`__builtin_ia32_pause()` is x86 pause; platform-specific but useful in tight spin loops.

---

## 14. Modern patterns: task-based concurrency

Prefer task-based over raw threads:

* `std::async` (with `std::launch::async`) returns `std::future`
* Thread pools / executors (C++ standardization ongoing). Use libraries like `std::thread_pool` (if available in your standard) or third-party libs.

Example `std::async`:

```cpp
#include <future>

auto fut = std::async(std::launch::async, [](int x){ return x*x; }, 10);
int result = fut.get();
```

---

## 15. Checklist & best practices

* Always ask: is data shared? If yes, what synchronizes it?
* Prefer higher-level constructs: `std::thread`, `std::future`, thread pool.
* Prefer mutexes for complex shared structures, atomics for simple counters/flags.
* Use RAII for locks (`lock_guard`, `unique_lock`).
* Avoid `detach()` except for fire-and-forget where lifetime is guaranteed.
* Use `std::condition_variable` with a predicate.
* Prefer `std::memory_order_seq_cst` until you need performance and fully understand weaker orders.
* Avoid `memory_order_consume` — prefer acquire-release.
* Use tools: ThreadSanitizer (TSAN) to detect data races.

---

## 16. Exercises

1. Implement a thread-safe queue using `std::mutex` + `std::condition_variable`.
2. Implement a lock-free ring buffer using `std::atomic<size_t>` indices (multiple producers/consumers is harder).
3. Use `std::async` to parallelize a CPU-bound loop and compare performance with `std::thread`.

---

## Appendix: Common code snippets

**Thread-safe queue** (simple version):

```cpp
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>

template<typename T>
class TSQueue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(T v) {
        {
            std::lock_guard<std::mutex> lk(m);
            q.push(std::move(v));
        }
        cv.notify_one();
    }

    T pop() { // blocks until available
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{ return !q.empty(); });
        T v = std::move(q.front()); q.pop();
        return v;
    }

    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lk(m);
        if (q.empty()) return std::nullopt;
        T v = std::move(q.front()); q.pop();
        return v;
    }
};
```

**Atomic publish/consume example (correct ordering):**

```cpp
#include <atomic>
#include <thread>
#include <iostream>

int data;
std::atomic<int*> ptr{nullptr};

void producer() {
    data = 123;
    ptr.store(&data, std::memory_order_release);
}

void consumer() {
    int* p;
    while ((p = ptr.load(std::memory_order_acquire)) == nullptr) {}
    std::cout << *p << "\n"; // safe
}
```

---

If you'd like, I can also:

* Provide a downloadable example project (CMake + files).
* Walk through ThreadSanitizer e
