#include "svg.h"
using namespace svg;

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);

        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<circle cx=\"" << center_.x << "\" cy=\"" << center_.y << "\" ";
        out << "r=\"" << radius_ << "\" ";
        RenderAttrs(context.out);

        out << "/>";
    }

    void Polyline::RenderObject(const RenderContext& context) const
    {
        auto& out = context.out;
        out << "<polyline points =\"";

        for (auto& point : points_) {
            out << point.x << "," << point.y << " ";
        }

        out << "\"";
        RenderAttrs(context.out);
        out << "/>"sv;
    }

    Polyline& Polyline::AddPoint(Point point)
    {
        points_.push_back(point);
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const
    {

        auto& out = context.out;

        out << "<text";
        RenderAttrs(context.out);
        out << "x = \"" << position_.x << "\" y=\"" << position_.y << "\" ";
        out << "dx =\"" << offset_.x << "\" dy=\"" << offset_.y << "\" ";
        out << "font-size=\"" << font_size_ << "\" ";
        out << "font-family=\"" << font_name_ << "\" ";
        out << "font-weight=\"" << font_weight_ << "\" ";
        out << ">";
        out << data_ << "</text>";
    }

    Text& Text::SetPosition(Point pos)
    {
        position_ = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset)
    {
        offset_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size)
    {
        font_size_ = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family)
    {
        font_name_ = font_family;
        return *this;

    }

    Text& Text::SetFontWeight(std::string font_weight)
    {
        font_weight_ = font_weight;
        return *this;
    }

    Text& Text::SetData(std::string data)
    {
        data_ = data;
        return *this;
    }

    void Document::AddPtr(std::unique_ptr<Object>&& obj)
    {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;
        for (auto& object : objects_) {
            object->Render(out);
        }

        out << "</svg>";
    }

    std::ostream& operator<<(std::ostream& os, StrokeLineCap line_Cap)
    {
        switch (line_Cap)
        {
        case svg::StrokeLineCap::BUTT:
            os << "butt";
            break;
        case svg::StrokeLineCap::ROUND:
            os << "round";
            break;
        case svg::StrokeLineCap::SQUARE:
            os << "square";
            break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, StrokeLineJoin line_join)
    {
        switch (line_join)
        {
        case svg::StrokeLineJoin::ARCS:
            os << "arcs";
            break;
        case svg::StrokeLineJoin::BEVEL:
            os << "bevel";
            break;
        case svg::StrokeLineJoin::MITER:
            os << "miter";
            break;
        case svg::StrokeLineJoin::MITER_CLIP:
            os << "miter_clip";
            break;
        case svg::StrokeLineJoin::ROUND:
            os << "round";
            break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& out, Color& color)
    {
        std::visit(ColorOperators{ out }, color);
        return out;
        // TODO: вставьте здесь оператор return
    }

}  // namespace svg