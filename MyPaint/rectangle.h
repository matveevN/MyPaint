#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QPainter>
#include <QPoint>
#include <QRect>
#include "IFigure.h"

class Rectangle : public IFigure {
public:
        Rectangle(const QPoint& topLeft, const QPoint& bottomRight);

        QRect getRect() const;
        QPoint getBottomRight() const;
        QPoint getCenter() const override;
        QPoint getTopLeft() const;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setBottomRight(const QPoint& bottomRight);
        void setTopLeft(const QPoint& point);
        void move(const QPoint& offset) override;

private:
        QPoint topLeft;
        QPoint bottomRight;
};

#endif // RECTANGLE_H
