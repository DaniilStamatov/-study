#pragma once

/*
 * ����� ��� ����� svg-����������
 */#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <optional>
namespace svg {

    struct Rgb {
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        Rgb()
            : red(0)
            , green(0)
            , blue(0) {
        }


        Rgb(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {

        }
    };

    struct Rgba : public Rgb {
        double alpha;

        Rgba()
            : Rgb()
            , alpha(1.0) {
        }

        Rgba(uint8_t red, uint8_t green, uint8_t blue, double alpha) : Rgb(red, green, blue), alpha(alpha) {

        }
    };


    using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
    inline const Color NoneColor{ "none" };

    class Object;

    enum class StrokeLineCap {
        BUTT,
        ROUND,
        SQUARE,
    };

    enum class StrokeLineJoin {
        ARCS,
        BEVEL,
        MITER,
        MITER_CLIP,
        ROUND,
    };

    std::ostream& operator<<(std::ostream& os, StrokeLineCap line_Cap);
    std::ostream& operator<<(std::ostream& os, StrokeLineJoin line_Cap);


    class ObjectContainer {
    public:
        template <typename T>
        void Add(T obj) {
            AddPtr(std::make_unique<T>(std::move(obj)));
        }

        virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

    protected:
        ~ObjectContainer() = default;
    };


    class Drawable {
    public:

        ~Drawable() = default;

        virtual void Draw(ObjectContainer& oc) const = 0;
    };
    struct Point {
        Point(double x, double y) : x(x), y(y) {

        }

        double x = 0;
        double y = 0;

    };


    struct RenderContext {
        RenderContext(std::ostream& out)
            : out(out) {
        }

        RenderContext(std::ostream& out, int indent_step, int indent = 0)
            : out(out)
            , indent_step(indent_step)
            , indent(indent) {
        }

        RenderContext Indented() const {
            return { out, indent_step, indent + indent_step };
        }

        void RenderIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        std::ostream& out;
        int indent_step = 0;
        int indent = 0;
    };

    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext& context) const = 0;
    };


    std::ostream& operator<<(std::ostream& out, Color& color);

    struct ColorOperators {
        std::ostream& out;

        void operator()(std::monostate) const {
            out << "none";
        }

        void operator()(std::string str) const {
            out << str;
        }

        void operator()(Rgb rgb) const {
            out << "rgb(" << static_cast<int>(rgb.red) << "," << static_cast<int>(rgb.green) << "," << static_cast<int>(rgb.blue) << ")";
        }

        void operator()(Rgba rgba) const {
            out << "rgba(" << static_cast<int>(rgba.red) << "," << static_cast<int>(rgba.green) << "," << static_cast<int>(rgba.blue) << rgba.alpha << ")";

        }
    };
    template <typename Owner>
    class PathProps {
    private:
        std::optional<Color> color_;
        std::optional<Color> stroke_color_;
        std::optional<double> stroke_width_;
        std::optional<StrokeLineCap> line_cap_;
        std::optional<StrokeLineJoin> line_join_;

        Owner& AsOwner() {
            //static cast ��������� ����������� *this � Owner ���� ����� Owner - ��������� ������ PathProps
            return static_cast<Owner&>(*this);
        }

    protected:
        void RenderAttrs(std::ostream& out) const {
            if (color_) {
                out << " fill =\"";
                std::visit(ColorOperators{ out }, *color_);
                out << "\" ";//equal to *color_
            }

            if (stroke_color_) {
                out << " stroke =\"";
                std::visit(ColorOperators{ out }, *stroke_color_);
                out << "\" ";//equal to *color_
            }

            if (stroke_width_) {
                out << " stroke-width =\"" << *stroke_width_ << "\" ";
            }

            if (line_cap_) {
                out << " stroke-linecap =\"" << *line_cap_ << "\" ";
            }

            if (line_join_) {
                out << "stroke-linejoin =\"" << *line_join_ << "\" ";
            }
        }

    public:
        Owner& SetFillColor(Color color) {
            color_ = color;
            return AsOwner();
        }

        Owner& SetStrokeColor(Color color) {
            stroke_color_ = color;
            return AsOwner();
        }

        Owner& SetStrokeWidth(double width) {
            stroke_width_ = width;
            return AsOwner();
        }

        Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
            line_cap_ = line_cap;
            return AsOwner();
        }

        Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
            line_join_ = line_join;
            return AsOwner();
        }
    };

    /*
     * ����� Circle ���������� ������� <circle> ��� ����������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
     */
    class Circle final : public Object, public PathProps<Circle> {
    public:
        Circle& SetCenter(Point center);
        Circle& SetRadius(double radius);

    private:
        void RenderObject(const RenderContext& context) const override;

        Point center_{ 0.0, 0.0 };
        double radius_ = 1.0;
    };
    /*
     * ����� Polyline ���������� ������� <polyline> ��� ����������� ������� �����
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
     */
    class Polyline final : public Object, public PathProps<Polyline> {
    private:
        std::vector<Point> points_;

        void RenderObject(const RenderContext& context) const override;
    public:
        // ��������� ��������� ������� � ������� �����
        Polyline& AddPoint(Point point);
    };

    /*
     * ����� Text ���������� ������� <text> ��� ����������� ������
     * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
     */
    class Text final : public Object, public PathProps<Text> {
    private:
        void RenderObject(const RenderContext& context) const override;
        Point position_ = { 0.0, 0.0 };
        Point offset_ = { 0.0, 0.0 };
        uint32_t font_size_ = 1;
        std::string font_name_;
        std::string font_weight_;
        std::string data_;

    public:
        // ����� ���������� ������� ����� (�������� x � y)
        Text& SetPosition(Point pos);

        // ����� �������� ������������ ������� ����� (�������� dx, dy)
        Text& SetOffset(Point offset);

        // ����� ������� ������ (������� font-size)
        Text& SetFontSize(uint32_t size);

        // ����� �������� ������ (������� font-family)
        Text& SetFontFamily(std::string font_family);

        // ����� ������� ������ (������� font-weight)
        Text& SetFontWeight(std::string font_weight);

        // ����� ��������� ���������� ������� (������������ ������ ���� text)
        Text& SetData(std::string data);

        // ������ ������ � ������, ����������� ��� ���������� �������� <text>
    };


    class Document : public ObjectContainer {
    public:
        // ��������� � svg-�������� ������-��������� svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj) override;

        // ������� � ostream svg-������������� ���������
        void Render(std::ostream& out) const;

    private:
        std::vector<std::unique_ptr<Object>> objects_;
    };


}  // namespace svg

