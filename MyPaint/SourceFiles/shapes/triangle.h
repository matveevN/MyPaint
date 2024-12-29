#pragma once

#include <QPainter>
#include <QPoint>
#include "IFigure.h"

namespace Shapes {
class Triangle final : public Shapes::IFigure {
public:
        Triangle(const QPoint& center, int size);

        ~Triangle();

        [[nodiscard]] QPoint getCenter() const override;

        [[nodiscard]] bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;

        void move(const QPoint& offset) override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        [[nodiscard]] QJsonObject toJson() const override;

        void fromJson(const QJsonObject& json) override;

        [[nodiscard]] QString getType() const override;

private:
        QPoint _center;
        int _size;

        void calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const;
};
} // namespace Shapes
