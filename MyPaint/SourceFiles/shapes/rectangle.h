#pragma once

#include <QPainter>
#include <QPoint>
#include <QRect>
#include "IFigure.h"

class Rectangle final : public IFigure {
public:
        Rectangle(const QPoint& topLeft, const QPoint& bottomRight);

        ~Rectangle();

        QPoint getCenter() const override;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        QString getType() const override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;
        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setBottomRight(const QPoint& bottomRight);
        void setTopLeft(const QPoint& point);
        void move(const QPoint& offset) override;

private:
        QPoint _topLeft;
        QPoint _bottomRight;
};
