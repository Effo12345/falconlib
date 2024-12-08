#pragma once

#include "liblvgl/extra/widgets/chart/lv_chart.h"
#include "liblvgl/lvgl.h"
#include "liblvgl/misc/lv_color.h"
#include <map>

namespace falconlib {

    class Grapher {
        lv_obj_t* chart;
        std::map<lv_palette_t, lv_chart_series_t*> serMap;

        const int graphWidth, graphHeight, xPos, yPos;
        bool isInit = false;
    public:
        Grapher(int width = 480, int height = 240, int x = 0, int y = 0);
        ~Grapher();

        void init(int min, int max, int pointCount = 250);
        void disable();

        void newData(double data, lv_palette_t series);
        void clear();
    };

} // namespace falconlib