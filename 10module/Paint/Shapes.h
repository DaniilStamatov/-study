#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) : type_(type) {

    }

    void SetPosition(Point pos) {
        position_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        Size image_size = GetImageSize(image);
        Size texture_size = texture_ ? texture_->GetSize() : Size{ 0, 0 };
        int x_max = std::min(position_.x + size_.width, image_size.width);
        int y_max = std::min(position_.y + size_.height, image_size.height);

        for (int x = position_.x; x < x_max; ++x) {
            for (int y = position_.y; y < y_max; ++y) {
                int point_x = x - position_.x;
                int point_y = y - position_.y;
                if (type_ == ShapeType::ELLIPSE && IsPointInEllipse({ point_x, point_y }, size_)) {
                    if (point_x < texture_.get()->GetSize().width && point_y < texture_.get()->GetSize().height) {
                        image[y][x] = texture_.get()->GetPixelColor({ point_x, point_y });
                    }
                    else {
                        image[y][x] = '.';
                    }
                }
                if (type_ == ShapeType::RECTANGLE && IsPointInRectangle({ point_x, point_y }, size_)) {
                    if (point_x < texture_.get()->GetSize().width && point_y < texture_.get()->GetSize().height) {
                        image[y][x] = texture_.get()->GetPixelColor({ point_x, point_y });
                    }
                    else {
                        image[y][x] = '.';
                    }
                }
            }
        }
    }

private:
    ShapeType type_;
    Size size_;
    std::shared_ptr<Texture> texture_;
    Point position_;
};