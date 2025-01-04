#pragma once

#include <QPoint>
#include "IFigure.h"

namespace Shapes {
class Ellipse final : public IFigure {
public:
        Ellipse(const QPoint& center, int radiusX, int radiusY);
        ~Ellipse();

        [[nodiscard]] QPoint getCenter() const override;
        [[nodiscard]] QString getType() const override;
        [[nodiscard]] QJsonObject toJson() const override;
        [[nodiscard]] bool contains(const QPoint& point) const override;
        [[nodiscard]] QRect boundingRect() const override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        void fromJson(const QJsonObject& json) override;

        void draw(QPainter& painter) const override;

        void move(const QPoint& offset) override;

private:
        QPoint _center;
        int _radiusX;
        int _radiusY;
};
} // namespace Shapes
