#pragma once

#include <QPainter>
#include <QPoint>
#include <QRect>
#include "IFigure.h"

namespace Shapes {
class Rectangle final : public Shapes::IFigure {
public:
        Rectangle(const QPoint& topLeft, const QPoint& bottomRight);

        ~Rectangle();

        [[nodiscard]] QPoint getCenter() const override;
        [[nodiscard]] QString getType() const override;
        [[nodiscard]] QJsonObject toJson() const override;
        [[nodiscard]] bool contains(const QPoint& point) const override;
        [[nodiscard]] QRect boundingRect() const override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        void fromJson(const QJsonObject& json) override;

        void draw(QPainter& painter) const override;
        void setBottomRight(const QPoint& bottomRight);
        void setTopLeft(const QPoint& point);
        void move(const QPoint& offset) override;

private:
        QPoint _topLeft;
        QPoint _bottomRight;
};
} // namespace Shapes
