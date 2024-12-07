#include "falconlib/display/selector/screenarrayselector.hpp"
#include "liblvgl/core/lv_event.h"

namespace falconlib {

//This pointer should never be used when it is unitialized because it is used by
//objects declared inside the Selector class but initialized in that class'
//constructor. It is also not visible in other files because it isn't declared
//In the header file
ScreenArraySelector* ptrSelector;

/**
    * A trampoline function to call member functions of ptrSelector when a
    * selector button is pressed
    *
    * @param btn The selector button that was pressed
    * @return A success flag
    */
static void selectionTrampoline(lv_event_t* btn) {
    //Just in case, throw an easy to understand error if the pointer is 
    //left unitialized
    // if(ptrSelector == nullptr)
    //     throw std::runtime_error("Selector pointer unitialized");

    // ptrSelector->setSelection(btn);

    // return LV_RES_OK;
}

/**
    * A trampoline function fo call a member function of ptrSelector when the
    * back button is pressed
    *
    * @param btn Pointer to the back button
    * @return A success flag
    */
static void backTrampoline(lv_event_t* btn) {
    // if(ptrSelector == nullptr)
    //     throw std::runtime_error("Selector pointer unitialized");

    // ptrSelector->back();

    // return LV_RES_OK;
}


//Dummy function to call when no auton is selected
void ScreenArraySelector::None() {};

/**
    * Auton selector using LVGL's buttons. Computes buttons' locations. Fills
    * mainPage and buttonMap. Initalizes autonToRun. Throws a runtime error
    * if the selector dimensions are out of bounds
    *
    * @param input The names/functions to be processed
    */
ScreenArraySelector::ScreenArraySelector(inputData input) {
    //Check if any of the rows or columns are out of bounds
    if(input.size() > 3)
        throw std::runtime_error("Auton selector overflow");
    for(int i = 0; i < input.size(); i++) {
        if(input[i].size() > 5)
            throw std::runtime_error("Auton selector overflow");
    }

    //Initialize selector buttons
    for(int i = 0; i < input.size(); i++) {
        for(int j = 0; j < input[i].size(); j++) {
            mainPage.push_back(Button(lv_scr_act(), i, j, input[i][j].first.c_str(), false));
            int buttonIndex = mainPage.size() - 1;
            buttonMap.insert({mainPage[buttonIndex].getInternalButton(), input[i][j].second});
            
            // lv_btn_set_action(mainPage[buttonIndex].getInternalButton(), LV_BTN_ACTION_CLICK, selectionTrampoline);
            lv_obj_add_event_cb(mainPage[buttonIndex].getInternalButton(), selectionTrampoline, LV_EVENT_CLICKED, NULL);
        }
    }
    
    // lv_btn_set_action(backButton.getInternalButton(), LV_BTN_ACTION_CLICK, backTrampoline);
    lv_obj_add_event_cb(backButton.getInternalButton(), backTrampoline, LV_EVENT_CLICKED, NULL);

    //Set the desired auton to the dummy None function
    //Non-static member functions must be bound to a this-pointer
    autonToRun = std::bind(&ScreenArraySelector::None, this);

    //Initalize prtSelector
    ptrSelector = this;

    lv_event_t test;

    //Sets theme to night
    lv_theme_t * th = lv_theme_default_init(lv_disp_get_default(), lv_color_hex(0xff0000), lv_color_hex(0x00ff00), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(lv_disp_get_default(), th);

    //Places autonName in the correct spot
    lv_obj_align(autonName, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_hidden(autonName, true);

    // * * * * * * * * * * * * *
    // *                       *
    // *    VERY IMPORTANT     *
    // *                       *
    // * * * * * * * * * * * * *
    Callback<void(lv_event_t*)>::func = std::bind(&ScreenArraySelector::test, this, std::placeholders::_1);
    lv_callback_t func = static_cast<lv_callback_t>(Callback<void(lv_event_t*)>::callback);
}

/**
    * A callback function run by the selector buttons when they are pressed.
    * Sets autonToRun based on the buttonMap. Enables backButton and autonName
    * while disabling the selector buttons
    *
    * @param btn Pointer to the button pressed
    */
void ScreenArraySelector::setSelection(lv_obj_t* btn) {
    autonToRun = buttonMap[btn];

    lv_label_set_text(autonName, lv_label_get_text(lv_obj_get_child(btn, 0)));

    
    for(Button b : mainPage) {
        b.setHidden(true);
    }

    backButton.setHidden(false);
    lv_obj_set_hidden(autonName, false);
}

/**
    * A callback function run by the back button when it is pressed.
    * Enables the selector buttons while disabling backButton and autonName
    */
void ScreenArraySelector::back() {
    autonToRun = std::bind(&ScreenArraySelector::None, this);

    lv_label_set_text(autonName, "Returned");

    for(Button b : mainPage) {
        b.setHidden(false);
    }

    backButton.setHidden(true);
    lv_obj_set_hidden(autonName, true);
}

/**
    * Get the final selection
    */
std::function<void()> ScreenArraySelector::getSelection() {
    return autonToRun;
}

/**
    * Run the selected auton, or None if none were selected
    */
void ScreenArraySelector::runSelection() {
    autonToRun();
}

/**
    * Sets whether the auton selector is visible/interactable
    *
    * @param istate The desired state to set
    */
void ScreenArraySelector::setActive(bool istate) {
    for(auto& button : mainPage) {
        button.setHidden(!istate);
    }

    backButton.setHidden(!istate);
    lv_obj_set_hidden(autonName, !istate);
}

void ScreenArraySelector::test(lv_event_t* event) {}

} // namespace falconlib