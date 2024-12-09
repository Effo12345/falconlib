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

        const int leftBtnCbId = 223856782;
        Callback<void(void), leftBtnCbId>::func = std::bind(&ScreenCycleSelector::leftBtnCb, this);
        llemu_callback_t leftBtn = static_cast<llemu_callback_t>(Callback<void(void), leftBtnCbId>::callback);
        pros::lcd::register_btn0_cb(leftBtn);

        const int rightBtnCbId = -9386581;
        Callback<void(void), rightBtnCbId>::func = std::bind(&ScreenCycleSelector::rightBtnCb, this);
        llemu_callback_t rightBtn = static_cast<llemu_callback_t>(Callback<void(void), rightBtnCbId>::callback);
        pros::lcd::register_btn2_cb(rightBtn);

        update();
    }

    void ScreenCycleSelector::internalDisable() {
        // Reinit lcd to clear callbacks
        pros::lcd::shutdown();
        pros::lcd::initialize();

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