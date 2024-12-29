#pragma once

#include <QPoint>
#include "IFigure.h"

class Ellipse final : public IFigure {
public:
        Ellipse(const QPoint& center, int radiusX, int radiusY);
        ~Ellipse();

        QPoint getCenter() const override;

        QString getType() const override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void move(const QPoint& offset) override;

private:
        QPoint _center;
        int _radiusX;
        int _radiusY;
};
