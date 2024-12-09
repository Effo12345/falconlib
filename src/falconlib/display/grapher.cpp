#include "falconlib/display/grapher.hpp"
#include "liblvgl/extra/widgets/chart/lv_chart.h"
#include "liblvgl/llemu.hpp"
#include "liblvgl/misc/lv_color.h"

namespace falconlib {

    Grapher::Grapher(int width, int height, int x, int y) : 
        graphWidth{width}, graphHeight{height}, xPos{x}, yPos{y}  {}
    
    Grapher::~Grapher() {
        disable();
    }

    void Grapher::init(int min, int max, int pointCount) {
        if (isInit)
            return;

        pros::lcd::shutdown();
        
        chart = lv_chart_create(lv_scr_act());

        lv_obj_set_size(chart, graphWidth, graphHeight);
        lv_obj_align(chart, LV_ALIGN_TOP_LEFT, xPos, yPos);

        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
        lv_chart_set_point_count(chart, pointCount);

        isInit = true;
    }

    void Grapher::disable() {
        if (!isInit)
            return;

        for (auto serPair : serMap) {
            lv_chart_remove_series(chart, serPair.second);
        }
        serMap.clear();
        lv_obj_del(chart);

        pros::lcd::initialize();

        isInit = false;
    }

    void Grapher::newData(int data, lv_palette_t series) {
        if (!isInit)
            return;

        if (!serMap.contains(series))
            serMap.insert({series, lv_chart_add_series(chart, lv_palette_main(series), LV_CHART_AXIS_PRIMARY_Y)});

        lv_chart_set_next_value(chart, serMap[series], data);
        lv_chart_refresh(chart);
    }

    void Grapher::clear() {
        for(auto serPair : serMap)
            lv_chart_set_all_value(chart, serPair.second, LV_CHART_POINT_NONE);
    }

} // namespace falconlib