#pragma once


#include <zs/editor/UI/AABB.h>


namespace zs::editor::UI {


struct Font {
    std::unique_ptr<FTFont> font;
    std::unique_ptr<FTSimpleLayout> layout;
    float fixed_height = -1;

    Font(const uint8_t *data, size_t size);
    Font &set_font_size(float font_size);
    Font &set_fixed_width(float width, FTGL::TextAlignment align = FTGL::ALIGN_CENTER);
    Font &set_fixed_height(float height);
    AABB calc_bounding_box(std::string const &str) const;
    Font &render(float x, float y, std::string const &str);
};


Font get_default_font();


}  // namespace zs::editor::UI
