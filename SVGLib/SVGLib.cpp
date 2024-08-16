#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/
/*
namespace {



}  // namespace
*/

namespace shapes {

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
            : p1_(p1)
            , p2_(p2)
            , p3_(p3) {
        }

        // Реализует метод Draw интерфейса svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star : public svg::Drawable { 
    private:
        Point center_;
        double outer_rad_;
        double inner_rad_;
        int num_rays_;

    public:
        Star(Point center, double outer_rad, double inner_rad, int num_rays) : center_(center), outer_rad_(outer_rad), inner_rad_(inner_rad), num_rays_(num_rays) {

        }

        void Draw(svg::ObjectContainer& container) const override {
            Polyline polyline;
            for (int i = 0; i <= num_rays_; ++i) {
                double angle = 2 * M_PI * (i % num_rays_) / num_rays_;
                polyline.AddPoint({ center_.x + outer_rad_ * sin(angle), center_.y - outer_rad_ * cos(angle) });
                if (i == num_rays_) {
                    break;
                }
                angle += M_PI / num_rays_;
                polyline.AddPoint({ center_.x + inner_rad_ * sin(angle), center_.y - inner_rad_ * cos(angle) });
            }

            container.Add(polyline.SetFillColor("red").SetStrokeColor("black"));
        }
    };

    class Snowman : public svg::Drawable {
    private:
        Point center_;
        double head_radius_;

    public:
        Snowman(Point center, double head_radius) : center_(center), head_radius_(head_radius) {

        }

        void Draw(svg::ObjectContainer& container) const override {
           container.Add(svg::Circle().SetCenter(center_).SetRadius(head_radius_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
           container.Add(svg::Circle().SetCenter({ center_.x, center_.y + head_radius_ * 2 }).SetRadius(head_radius_ * 1.5).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
           container.Add(svg::Circle().SetCenter({ center_.x, center_.y + head_radius_ * 5 }).SetRadius(head_radius_ * 2).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
        }
    };

} // namespace shapes


Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({ center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle) });
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({ center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle) });
    }
    return polyline;
}

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

void DrawBlack() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Triangle>(Point{ 100, 20 }, Point{ 120, 50 }, Point{ 80, 40 }));
    picture.emplace_back(make_unique<Star>(Point{ 50.0, 20.0 }, 10.0, 4.0, 5));
    picture.emplace_back(make_unique<Snowman>(Point{ 30, 20 }, 10.0));

    svg::Document doc;
    DrawPicture(picture, doc);

    const Text base_text =  //
        Text()
        .SetFontFamily("Verdana"s)
        .SetFontSize(12)
        .SetPosition({ 10, 100 })
        .SetData("Happy New Year!"s);
    doc.Add(Text{ base_text }
        .SetStrokeColor("yellow"s)
        .SetFillColor("yellow"s)
        .SetStrokeLineJoin(StrokeLineJoin::ROUND)
        .SetStrokeLineCap(StrokeLineCap::ROUND)
        .SetStrokeWidth(3));
    doc.Add(Text{ base_text }.SetFillColor("red"s));

    doc.Render(cout);
}

void DrawColor() {
    using namespace svg;
    using namespace std;

    Color none_color;
    cout << none_color << endl; // none

    Color purple{ "purple"s };
    cout << purple << endl; // purple

    Color rgb = Rgb{ 100, 200, 255 };
    cout << rgb << endl; // rgb(100,200,255)

    Color rgba = Rgba{ 100, 200, 255, 0.5 };
    cout << rgba << endl; // rgba(100,200,255,0.5)

    Circle c;
    c.SetRadius(3.5).SetCenter({ 1.0, 2.0 });
    c.SetFillColor(rgba);
    c.SetStrokeColor(purple);

    Document doc;
    doc.Add(std::move(c));
    doc.Render(cout);
}


uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// Выполняет линейную интерполяцию Rgb цвета от from до to в зависимости от параметра t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return { Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t) };
}

int main() {
    using namespace svg;
    using namespace std;

    Rgb start_color{ 0, 255, 30 };
    Rgb end_color{ 20, 20, 150 };

    const int num_circles = 10;
    Document doc;
    for (int i = 0; i < num_circles; ++i) {
        const double t = double(i) / (num_circles - 1);

        const Rgb fill_color = Lerp(start_color, end_color, t);

        doc.Add(Circle()
            .SetFillColor(fill_color)
            .SetStrokeColor("black"s)
            .SetCenter({ i * 20.0 + 40, 40.0 })
            .SetRadius(15));
    }
    doc.Render(cout);
}

//svg::Color color1;                               // none
//svg::Color color2 = svg::Rgb{ 215, 30, 25 };       // rgb(215,30,25)
//svg::Color color3 = svg::NoneColor;              // none
//svg::Color color4 = svg::Rgba{ 15, 15, 25, 0.7 };  // rgba(15,15,25,0.7)
//svg::Color color5 = "red";                      // red
//svg::Color color6 = svg::Rgb{};                  // rgb(0,0,0)
//svg::Color color7 = svg::Rgba{};                 // rgba(0,0,0,1.0);