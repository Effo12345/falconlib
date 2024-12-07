#pragma once
#include "falconlib/util/taskwrapper.hpp"

namespace falconlib {

class AbstractSelector : TaskWrapper {
private:
    void loop();
    
protected:
    // Allow move semantics but not copy
    AbstractSelector() = default;
    AbstractSelector(const AbstractSelector& sel) = delete;
    AbstractSelector(AbstractSelector&& sel) = default;

    virtual ~AbstractSelector();

    virtual void internalInit() = 0;
    virtual void internalDisable() = 0;
    virtual void internalLoop() {}

    std::function<void()> autonToRun;

public:
    void init();
    void disable();
    void run();
};

} // namespace falconlib