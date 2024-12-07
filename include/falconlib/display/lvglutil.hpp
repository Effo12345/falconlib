#pragma once
#include <functional>
#include "liblvgl/lvgl.h"

namespace falconlib {

   template <typename T>
   struct Callback;

   template <typename Ret, typename... Params>
   struct Callback<Ret(Params...)> {
      template <typename... Args> 
      static Ret callback (Args... args) {                    
         return func(args...);  
      }
      static std::function<Ret(Params...)> func; 
   };

   template <typename Ret, typename... Params>
   std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

   typedef void (*lv_callback_t)(lv_event_t*);

   inline void lv_obj_set_hidden (lv_obj_t* obj, bool isHidden) {
      if (isHidden)
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
      else 
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
   }

   inline lv_callback_t lv_build_event_cb (std::function<void(_lv_event_t *)> func) {
      Callback<void(lv_event_t*)>::func = func;
      return static_cast<lv_callback_t>(Callback<void(lv_event_t*)>::callback);
   }

} // namespace falconlib