#include "Ellipse.h"

Ellipse::Ellipse(const QPoint& center, int radiusX, int radiusY)
: center(center)
, radiusX(radiusX)
, radiusY(radiusY) {
}

void Ellipse::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawEllipse(center, radiusX, radiusY);
}

void Ellipse::setRadius(int radiusX, int radiusY) {
        this->radiusX = radiusX;
        this->radiusY = radiusY;
}

QPoint Ellipse::getCenter() const {
        return center;
}

bool Ellipse::contains(const QPoint& point) const {
        int dx = point.x() - center.x();
        int dy = point.y() - center.y();
        return (dx * dx) / (radiusX * radiusX) + (dy * dy) / (radiusY * radiusY)
               <= 1;
}

void Ellipse::setRadiusX(int radius) {
        radiusX = radius;
}

void Ellipse::setRadiusY(int radius) {
        radiusY = radius;
}

int Ellipse::getRadiusX() const {
        return radiusX;
}

int Ellipse::getRadiusY() const {
        return radiusY;
}

void Ellipse::setCenter(const QPoint& newCenter) {
        center = newCenter;
}

void Ellipse::move(const QPoint& offset) {
        center += offset;
}
