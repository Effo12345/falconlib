#include "falconlib/display/button.hpp"
#include "liblvgl/core/lv_obj_tree.h"

namespace falconlib {

    /**
     * Initializes the internal button with the correct name, placement, and
     * state.
     *
     * @param parent The button's parent object
     * @param column Column placement
     * @param row Row placement
     * @param title Name displayed on the button
     * @param isHidden Whether the button is visible on initialization
     */
    Button::Button(lv_obj_t * parent, int column, int row, const char * title, bool isHidden) {
        button = lv_btn_create(parent);
        lv_obj_set_pos(button, (column * 155) + 10, (row * 45) + 5);
        lv_obj_set_size(button, 150, 40);
        lv_obj_set_hidden(button, isHidden);

        label = lv_label_create(button);
        lv_label_set_text(label, title);
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);
    }

    /**
     * Initializes the internal button with the correct name, placement, and
     * state.
     *
     * @param parent The button's parent object
     * @param horiz Horizontal pixel placement (from top left corner)
     * @param vert Vertical pixel placement (from top left corner)
     * @param title Name displayed on the button
     * @param isHidden Whether the button is visible on initialization
     */
    Button::Button(lv_obj_t * parent, int horiz, int vert, int width, int height, const char * title, bool isHidden) {
        button = lv_btn_create(parent);
        lv_obj_set_pos(button, horiz, vert);
        lv_obj_set_size(button, width, height);
        lv_obj_set_hidden(button, isHidden);

        label = lv_label_create(button);
        lv_label_set_text(label, title);
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);
    }

    Button::~Button() {
        lv_obj_del(button);
        lv_obj_del(label);
    }

} // namespace falconlib