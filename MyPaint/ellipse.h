#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPoint>
#include "IFigure.h"

class Ellipse : public IFigure {
public:
        Ellipse(const QPoint& center, int radiusX, int radiusY);

        QPoint getCenter() const override;

        int getRadiusX() const;
        int getRadiusY() const;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setRadius(int radiusX, int radiusY);
        void move(const QPoint& offset) override;
        void setRadiusX(int radius);
        void setRadiusY(int radius);
        void setCenter(const QPoint& newCenter);

private:
        QPoint center;
        int radiusX;
        int radiusY;
};

#endif // ELLIPSE_H
