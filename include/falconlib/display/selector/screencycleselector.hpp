#pragma once
#include "falconlib/display/selector/abstractselector.hpp"

namespace falconlib {

    class ScreenCycleSelector : public AbstractSelector {
        using inputData = std::vector<std::pair<std::string, std::function<void()>>>;

        // Disallow default constructor
        ScreenCycleSelector() = delete;

        void internalInit();
        void internalDisable();

        void update();
        int clockMod(int a, int b);

        void leftBtnCb();
        void rightBtnCb();

        inputData userFunctions;
        int selPos = 0;
        const int line;

    public:
        ScreenCycleSelector(const inputData input, int lineNum = 0);

    };

} // namespace falconlib