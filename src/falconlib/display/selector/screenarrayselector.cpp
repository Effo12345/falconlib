#include "falconlib/display/selector/screenarrayselector.hpp"
#include "liblvgl/llemu.hpp"

namespace falconlib {

    static void test (lv_event_t* event) {
        lv_label_create(lv_scr_act());
    }

    ScreenArraySelector::ScreenArraySelector(const inputData input) : userFunctions{input} {}

    ScreenArraySelector::~ScreenArraySelector() {
        internalDisable();
    }

    void ScreenArraySelector::internalInit() {
        pros::lcd::shutdown();

        //Check if any of the rows or columns are out of bounds
        if(userFunctions.size() > 3)
            throw std::runtime_error("Auton selector overflow");
        for(int i = 0; i < userFunctions.size(); i++) {
            if(userFunctions[i].size() > 5)
                throw std::runtime_error("Auton selector overflow");
        }

        // Init back button and label
        backButton = std::make_unique<Button>(lv_scr_act(), 0, 0, 75, 40, "Back", true);
        autonName = lv_label_create(lv_scr_act());

        // Generate callbacks for button presses
        const int selectionCbId = -975682751;
        Callback<void(lv_event_t*), selectionCbId>::func = std::bind(&ScreenArraySelector::setSelection, this, std::placeholders::_1);
        lv_callback_t selectionCb = static_cast<lv_callback_t>(Callback<void(lv_event_t*), selectionCbId>::callback);
        const int backCbId = 185610950;
        Callback<void(lv_event_t*), backCbId>::func = std::bind(&ScreenArraySelector::back, this, std::placeholders::_1);
        lv_callback_t backCb = static_cast<lv_callback_t>(Callback<void(lv_event_t*), backCbId>::callback);
        // lv_callback_t selectionCb = lv_build_event_cb(std::bind(&ScreenArraySelector::setSelection, this, std::placeholders::_1), -10);
        // lv_callback_t backCb = lv_build_event_cb(std::bind(&ScreenArraySelector::back, this, std::placeholders::_1));

        //Initialize selector buttons
        for(int i = 0; i < userFunctions.size(); i++) {
            for(int j = 0; j < userFunctions[i].size(); j++) {
                // mainPage.push_back(Button(lv_scr_act(), i, j, userFunctions[i][j].first.c_str(), false));
                mainPage.push_back(std::make_shared<Button>(lv_scr_act(), i, j, userFunctions[i][j].first.c_str(), false));
                int buttonIndex = mainPage.size() - 1;
                buttonMap.insert({mainPage[buttonIndex]->getInternalButton(), userFunctions[i][j].second});
            
                // lv_btn_set_action(mainPage[buttonIndex].getInternalButton(), LV_BTN_ACTION_CLICK, selectionTrampoline);
                lv_obj_add_event_cb(mainPage[buttonIndex]->getInternalButton(), selectionCb, LV_EVENT_CLICKED, NULL);
            }
        }

        // lv_btn_set_action(backButton.getInternalButton(), LV_BTN_ACTION_CLICK, backTrampoline);
        lv_obj_add_event_cb(backButton->getInternalButton(), backCb, LV_EVENT_CLICKED, NULL);

        //Sets theme to night
        lv_theme_t * th = lv_theme_default_init(lv_disp_get_default(), lv_color_hex(0x4b4c43), lv_color_hex(0x00ff00), true, LV_FONT_DEFAULT);
        lv_disp_set_theme(lv_disp_get_default(), th);

        //Places autonName in the correct spot
        lv_obj_align(autonName, LV_ALIGN_TOP_MID, 0, 5);
        lv_obj_set_hidden(autonName, true);
    }

    void ScreenArraySelector::internalDisable() {
        buttonMap.clear();
        backButton.reset();
        lv_obj_del(autonName);

        pros::lcd::initialize();
    }

    /**
     * Sets whether the auton selector is visible/interactable
     *
     * @param istate The desired state to set
     */
    void ScreenArraySelector::setActive(bool state) {
        if (!isInit)
            return;

        for(auto b : mainPage) {
            b->setHidden(!state);
        }

        backButton->setHidden(!state);
        lv_obj_set_hidden(autonName, !state);
    }

    /**
     * A callback function run by the selector buttons when they are pressed.
     * Sets autonToRun based on the buttonMap. Enables backButton and autonName
     * while disabling the selector buttons
     *
     * @param event Object bound to button press event
     */
    void ScreenArraySelector::setSelection(lv_event_t* event) {
        lv_obj_t* btn = lv_event_get_target(event);

        autonToRun = buttonMap[btn];
        lv_label_set_text(autonName, lv_label_get_text(lv_obj_get_child(btn, 0)));

        for(auto b : mainPage) {
            b->setHidden(true);
        }

        backButton->setHidden(false);
        lv_obj_set_hidden(autonName, false);
    }

    /**
     * A callback function run by the selector buttons when they are pressed.
     * Sets autonToRun based on the buttonMap. Enables backButton and autonName
     * while disabling the selector buttons
     *
     * @param btn Pointer to the button pressed
     */
    void ScreenArraySelector::back(lv_event_t* event) {
        autonToRun = nullptr;

        lv_label_set_text(autonName, "Returned");

        for(auto b : mainPage) {
            b->setHidden(false);
        }

        backButton->setHidden(true);
        lv_obj_set_hidden(autonName, true);
    }

} // namespace falconlib