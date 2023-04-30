#include <queue>
#include <cassert>

#include "fibers.hpp"

class FiberScheduler {
public:
    friend class Watch;
    /// Fiber simple trampoline
    friend void trampoline(Fiber *fiber);  // TODO

    virtual ~FiberScheduler() {
        assert(queue.empty());
    }

    void schedule(Fiber fiber) {
        schedule(create_context_from_fiber(std::move(fiber)));
    }

    void schedule(Context context) {
        queue.push(std::move(context));
    }

    /// Prepare stack, execution, arguments, etc...
    static Context create_context_from_fiber(Fiber fiber);  // TODO

    /// Reschedule self to end of queue
    static YieldData yield(YieldData);  // TODO

    template <class Watch, class... Args>
    void create_current_fiber_watch(Args... args) {
        sched_context.watch = std::make_shared<Watch>(args...);
    }

    bool empty() {
        return queue.empty();
    }

protected:
    /// Proceed one context from queue
    void run_one();  // TODO

    /// Proceed till queue is not empty
    virtual void run() {
        while (!empty()) {
            run_one();
        }
    }

private:
    std::queue<Context> queue;
    Context sched_context;
};
