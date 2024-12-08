#include "falconlib/display/selector/screencycleselector.hpp"
#include "falconlib/display/displayutil.hpp"
#include "liblvgl/llemu.hpp"
#include <functional>

namespace falconlib {

    ScreenCycleSelector::ScreenCycleSelector(const inputData input, int lineNum) : userFunctions{input}, line{lineNum} {
        // Make running nothing the default value
        userFunctions.insert(userFunctions.begin(), {"None", nullptr});
    }

    void ScreenCycleSelector::internalInit() {
        pros::lcd::initialize();
        pros::lcd::register_btn0_cb(llemu_build_btn_cb(std::bind(&ScreenCycleSelector::leftBtnCb, this)));
        pros::lcd::register_btn2_cb(llemu_build_btn_cb(std::bind(&ScreenCycleSelector::rightBtnCb, this)));

        update();
    }

    void ScreenCycleSelector::internalDisable() {
        selPos = 0;
    }

    void ScreenCycleSelector::update() {
        int pos = clockMod(selPos, userFunctions.size());
        autonToRun = userFunctions[pos].second;
        pros::lcd::set_text(line, userFunctions[pos].first.c_str());
    }

    int ScreenCycleSelector::clockMod(int a, int b) {
        int mod = a % b;
        return mod + (mod < 0 ? b : 0);
    }

    void ScreenCycleSelector::leftBtnCb() {
        selPos--;
        update();
    }

    void ScreenCycleSelector::rightBtnCb() {
        selPos++;
        update();
    }

} // namespace falconlib