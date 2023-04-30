#include "runtime.hpp"

StackPool stack_pool;

Context::Context(Fiber fiber)
        : fiber(std::make_unique<Fiber>(std::move(fiber))),
          stack(stack_pool.alloc()),
          esp(reinterpret_cast<intptr_t>(stack.ptr) + StackPool::STACK_SIZE) {
}

EpollScheduler *current_scheduler = nullptr;

void schedule(Fiber fiber) {
    if (!current_scheduler) {
        throw std::runtime_error("Global scheduler is empty");
    }
    current_scheduler->schedule(std::move(fiber));
}

void yield() {
    if (!current_scheduler) {
        throw std::runtime_error("Global scheduler is empty");
    }
    current_scheduler->yield({});
}

template <class Watch, class... Args>
void create_current_fiber_watch(Args... args) {
    if (!current_scheduler) {
        throw std::runtime_error("Global scheduler is empty");
    }
    current_scheduler->create_current_fiber_watch<Watch>(args...);
}

void trampoline(Fiber *fiber) {
    /// process exceptions with std::current_exception()
    (*fiber)();

    current_scheduler->sched_context.switch_context(Action{Action::STOP});
    __builtin_unreachable();
}

Action Context::switch_context(Action action) {
    auto *eipp = &eip;
    auto *espp = &esp;
    asm volatile(""  /// save ebp
                 ""  /// switch esp
                 ""  /// switch eip with ret_label
                 /// BEFORE (B) GOING BACK
                 "ret_label:"
                 /// AFTER (A)
                 ""  /// load ebp
                 : "+S"(espp), "+D"(eipp)  /// throw action
                 ::"eax", "ebx", "ecx", "edx", "memory", "cc");
    return action;
}

Context FiberScheduler::create_context_from_fiber(Fiber fiber) {
    Context context(std::move(fiber));

    /// stack
    /// function

    return context;
}

YieldData FiberScheduler::yield(YieldData data) {
    /// current_scheduler->sched_context
    /// If THROW -> throw current_scheduler->sched_context.exception
}

void FiberScheduler::run_one() {
    sched_context = std::move(queue.front());
    queue.pop();

    /// run with START or THROW if exception
    /// except if exception with std::rethrow_exception
    /// watch if watch
    /// schedule again if SCHE
}

void scheduler_run(EpollScheduler &sched) {
    if (current_scheduler) {
        throw std::runtime_error("Global scheduler is not empty");
    }
    current_scheduler = &sched;
    current_scheduler->run();
    current_scheduler = nullptr;
}


void EpollScheduler::await_read(Context context, YieldData data) {
    /// Subscribe epoll for read
}

void EpollScheduler::do_read(Node node) {
    /// Do read, schedule fiber
}

void EpollScheduler::await_write(Context context, YieldData data) {
    /// Subscribe epoll for write
}

void EpollScheduler::do_write(Node node) {
    /// Do write, schedule fiber
}

void EpollScheduler::await_accept(Context context, YieldData data) {
    /// Subscribe epoll for accept
}

void EpollScheduler::do_accept(Node node) {
    /// Do accept, schedule fiber
}

void EpollScheduler::do_error(Node node) {
    /// Throw exception any in fiber
}

void EpollScheduler::run() {
    while (true) {
        /// Process all fibers
        /// If no fd to wait break
        /// Wait any fd
        /// If error do_error
        /// Else if in or out process it
    }
}

namespace Async {
    int accept(int fd, sockaddr * addr, socklen_t * addrlen) {
    }

    ssize_t read(int fd, char * buf, size_t size) {
    }

    ssize_t write(int fd, const char * buf, size_t size) {
    }
}
