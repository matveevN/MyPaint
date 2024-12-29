#pragma once

#include <qpainter.h>
namespace Shapes {
class IFigure {
public:
        [[nodiscard]] virtual QPoint getCenter() const = 0;

        [[nodiscard]] virtual bool contains(const QPoint& point) const = 0;

        virtual void draw(QPainter& painter) const = 0;
        virtual void move(const QPoint& offset) = 0;

        virtual void initialize(const QPoint& startPoint) = 0;

        virtual void updateShape(const QPoint& currentPoint) = 0;

        [[nodiscard]] virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;

        [[nodiscard]] virtual QString getType() const = 0;

        [[nodiscard]] virtual QRect boundingRect() const = 0;

        virtual ~IFigure() = default;
};
} // namespace Shapes
