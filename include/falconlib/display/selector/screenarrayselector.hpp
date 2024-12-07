#pragma once
#include "falconlib/display/lvglutil.hpp"
#include "falconlib/display/selector/abstractselector.hpp"



#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "falconlib/display/button.hpp"

namespace falconlib {

    class ScreenArraySelector : AbstractSelector {
        using inputData = std::vector<std::vector<std::pair<std::string, std::function<void()>>>>;

        // Disallow default constructor
        ScreenArraySelector() = delete;

        void internalInit();
        void internalDisable();

        void setSelection(lv_event_t* event);
        void back(lv_event_t* event);

        std::vector<Button> mainPage;
        std::map<lv_obj_t*, std::function<void()>> buttonMap;

        std::unique_ptr<Button> backButton;
        lv_obj_t* autonName = nullptr;

        const inputData& input;

    public:
        ScreenArraySelector(const inputData& userFunctions = {});

        void setActive(bool state);
    };

}