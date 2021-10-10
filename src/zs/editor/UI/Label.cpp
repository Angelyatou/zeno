#include <zs/editor/UI/Label.h>


namespace zs::editor::UI {


Label::Label() {
    bbox = {0, 0, 300, 40};
}


void Label::paint() const {
    glColor3f(0.25f, 0.25f, 0.25f);
    glRectf(bbox.x0, bbox.y0, bbox.x0 + bbox.nx, bbox.y0 + bbox.ny);

    auto font = get_default_font();
    font.set_font_size(font_size);
    font.set_fixed_width(bbox.nx - BW * 2, alignment);
    font.set_fixed_height(bbox.ny);
    glColor3f(1.f, 1.f, 1.f);

    font.render(bbox.x0 + BW, bbox.y0, text);
}


}
