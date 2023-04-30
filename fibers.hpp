#include <functional>
#include <cinttypes>
#include <stdexcept>
#include <memory>
#include <optional>

#include "stack_pool.hpp"


using Fiber = std::function<void()>;

union YieldData {
    void * ptr = {};
    int32_t i;
    uint32_t u;
    size_t s;
    ssize_t ss;
};
static_assert(sizeof(YieldData) <= sizeof(void*));


struct Action {
    enum {
        /// Scheduler -> Fiber
        START,
        THROW,

        /// Fiber -> Scheduler
        STOP,
        SCHED,
    } action;

    YieldData user_data{};
};


class Watch;

struct Context {
    std::unique_ptr<Fiber> fiber;
    StackPool::Stack stack;

    intptr_t eip = 0;
    intptr_t esp = 0;
    std::shared_ptr<Watch> watch;
    std::exception_ptr exception{};
    YieldData yield_data = {};

    Context() = default;

    explicit Context(Fiber fiber);

    Context(Context &&other) = default;

    Context(const Context &other) = delete;

    Context &operator=(Context &&other) = default;

    void operator=(const Context &other) = delete;

    /// swap current eip and esp
    Action switch_context(Action);  // TODO
};

class Watch {
public:
    virtual ~Watch() = default;

    /// Inspect context after execution
    virtual void operator()(Action &, Context &) = 0;
};
