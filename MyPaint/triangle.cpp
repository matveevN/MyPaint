#include "Triangle.h"

Triangle::Triangle(const QPoint& center, int size)
: center(center)
, size(size) {
}

void Triangle::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);

        QPoint p1 = center + QPoint(size, 0);
        QPoint p2 = center + QPoint(-size, 0);
        QPoint p3 = center + QPoint(0, -size);

        QPolygon polygon;
        polygon << p1 << p2 << p3;
        painter.drawPolygon(polygon);
}

bool Triangle::contains(const QPoint& point) const {
        auto area = [](const QPoint& p1, const QPoint& p2, const QPoint& p3) {
                return abs(p1.x() * (p2.y() - p3.y())
                           + p2.x() * (p3.y() - p1.y())
                           + p3.x() * (p1.y() - p2.y()))
                       / 2.0;
        };

        double fullArea = area(point1, point2, point3);

        double area1 = area(point, point2, point3);
        double area2 = area(point1, point, point3);
        double area3 = area(point1, point2, point);

        return abs(fullArea - (area1 + area2 + area3)) < 1e-5;
}

void Triangle::move(const QPoint& offset) {
        center += offset;
}

void Triangle::setCenter(const QPoint& newCenter) {
        center = newCenter;
}

void Triangle::setSize(int newSize) {
        size = newSize;
}

QPoint Triangle::getPoint1() const {
        return point1;
}
QPoint Triangle::getPoint2() const {
        return point2;
}

QPoint Triangle::getPoint3() const {
        return point3;
}

int Triangle::getSize() const {
        return size;
}

QPoint Triangle::getCenter() const {
        int centerX = (point1.x() + point2.x() + point3.x()) / 3;
        int centerY = (point1.y() + point2.y() + point3.y()) / 3;
        return center;
}
