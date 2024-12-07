#pragma once
#include "falconlib/display/lvglutil.hpp"



#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "falconlib/display/button.hpp"

namespace falconlib {

    class ScreenArraySelector {
        using inputData = std::vector<std::vector<std::pair<std::string, std::function<void()>>>>;

        //Declare the default constructor as private to ensure it is never called.
        //ptrSelector MUST be initialized
        ScreenArraySelector() = delete;

        //Dummy function to call when no auton is selected
        void None();

        std::vector<Button> mainPage;

        std::map<lv_obj_t*, std::function<void()>> buttonMap;

        Button backButton = Button(lv_scr_act(), 0, 0, 75, 40, "Back", true);;
        lv_obj_t* autonName = lv_label_create(lv_scr_act());

        std::function<void()> autonToRun;

    public:
        ScreenArraySelector(inputData input = {});

        std::function<void()> getSelection();
        void runSelection();

        void setActive(bool istate);

        void setSelection(lv_obj_t* btn);
        void back();

        void test(lv_event_t*);
    };

}