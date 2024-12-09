#pragma once
#include "liblvgl/lvgl.h"
#include "falconlib/display/displayutil.hpp"

namespace falconlib {

    class Button {
        lv_obj_t* button = nullptr;
        lv_obj_t* label = nullptr;

        // Don't allow copying since calling the destructor in pre-init throws prefetch error
        Button(const Button& b) = delete;
        Button(Button&& b) = default;
    public:
        Button(lv_obj_t * parent, int column, int row, const char * title, bool isHidden);
        Button(lv_obj_t * parent, int horiz, int vert, int width, int height, const char * title, bool isHidden);
        ~Button();

        lv_obj_t* getInternalButton() { 
            return button;
        }

        void setHidden(bool istate) {
            lv_obj_set_hidden(button, istate);
        }
    };

} // namespace falconlib