#include "falconlib/display/selector/abstractselector.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

namespace falconlib {

    void AbstractSelector::init() {
        autonToRun = nullptr;
        
        startTask("auton_selector");
        internalInit();
    }

    void AbstractSelector::disable() {
        stopTask();
        internalDisable();
    }

    void AbstractSelector::run() {
        if (autonToRun) {
            autonToRun();
        }
        disable();
    }

    void AbstractSelector::loop() {
        while (!pros::c::competition_is_autonomous()) {
            internalLoop();
            pros::delay(25);
        }

        disable();
    }

} // namespace falconlib