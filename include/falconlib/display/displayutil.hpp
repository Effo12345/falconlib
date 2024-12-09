#pragma once
#include <functional>
#include "liblvgl/lvgl.h"

namespace falconlib {

   template <typename T, int tag = 0>
   struct Callback;

   template <typename Ret, typename... Params, int tag>
   struct Callback<Ret(Params...), tag> {
      template <typename... Args>
      static Ret callback (Args... args) {
         return func(args...);
      }
      static std::function<Ret(Params...)> func;
   };

   template <typename Ret, typename... Params, int tag>
   std::function<Ret(Params...)> Callback<Ret(Params...), tag>::func;

   // LVGL utils
   typedef void (*lv_callback_t)(lv_event_t*);

   inline void lv_obj_set_hidden (lv_obj_t* obj, bool isHidden) {
      if (isHidden)
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
      else 
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
   }

   // inline lv_callback_t lv_build_event_cb (std::function<void(lv_event_t *)> func, const int id) {
   //    Callback<void(lv_event_t*), id>::func = func;
   //    return static_cast<lv_callback_t>(Callback<void(lv_event_t*), id>::callback);
   // }

   // LLEMU utils
   typedef void (*llemu_callback_t)(void);

   inline llemu_callback_t llemu_build_btn_cb(std::function<void(void)> func) {
      Callback<void(void)>::func = func;
      return static_cast<llemu_callback_t>(Callback<void(void)>::callback);
   }

} // namespace falconlib