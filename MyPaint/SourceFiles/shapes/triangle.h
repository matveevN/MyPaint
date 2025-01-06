#pragma once

#include "IShape.h"

namespace Shapes {
class Triangle final : public Shapes::IShapes {
public:
        Triangle(const QPoint& center, int size);

        ~Triangle();

        [[nodiscard]] QPoint getCenter() const override;
        [[nodiscard]] bool contains(const QPoint& point) const override;
        [[nodiscard]] QJsonObject toJson() const override;
        [[nodiscard]] QString getType() const override;
        [[nodiscard]] QRect boundingRect() const override;

        void draw(QPainter& painter) const override;

        void move(const QPoint& offset) override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        void fromJson(const QJsonObject& json) override;

private:
        QPoint _center;
        int _size;

        void calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const;
};
} // namespace Shapes
