#include "Triangle.h"

Triangle::Triangle(const QPoint& center, int size)
: center(center)
, size(size) {
}

int Triangle::getSize() const {
        return size;
}

void Triangle::setSize(int newSize) {
        size = newSize;
}

QPoint Triangle::getCenter() const {
        return center;
}

void Triangle::setCenter(const QPoint& newCenter) {
        center = newCenter;
}

///проблема к центру треугольника
void Triangle::calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const {
        // Высота равностороннего треугольника
        int height = static_cast<int>(size * sqrt(3) / 2);

        // Смещение вершин относительно центра
        p1 = center + QPoint(0, -2 * height / 3);    // Верхняя
        p2 = center + QPoint(-size / 2, height / 3); // Левая
        p3 = center + QPoint(size / 2, height / 3);  // Правая
}

void Triangle::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);

        QPoint p1, p2, p3;
        calculateVertices(p1, p2, p3);

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

        QPoint p1, p2, p3;
        calculateVertices(p1, p2, p3);

        double fullArea = area(p1, p2, p3);
        double area1 = area(point, p2, p3);
        double area2 = area(p1, point, p3);
        double area3 = area(p1, p2, point);

        return std::abs(fullArea - (area1 + area2 + area3)) < 1e-5;
}

void Triangle::move(const QPoint& offset) {
        center += offset;
}
