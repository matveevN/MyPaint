#include "triangle.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

namespace Shapes {
Triangle::Triangle(const QPoint& center, int size)
: _center(center)
, _size(size) {
}

Triangle::~Triangle() = default;

QPoint Triangle::getCenter() const {
        return _center;
}

///проблема к центру треугольника
void Triangle::calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const {
        // Высота равностороннего треугольника
        int height = static_cast<int>(_size * sqrt(3) / 2);

        // Смещение вершин относительно центра
        p1 = _center + QPoint(0, -2 * height / 3);     // Верхняя
        p2 = _center + QPoint(-_size / 2, height / 3); // Левая
        p3 = _center + QPoint(_size / 2, height / 3);  // Правая
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
        _center += offset;
}

QString Triangle::getType() const {
        return "triangle";
}

QJsonObject Triangle::toJson() const {
        QJsonObject json;
        json["type"] = getType();
        json["center"] = QJsonArray{_center.x(), _center.y()};
        json["size"] = _size;
        return json;
}

void Triangle::fromJson(const QJsonObject& json) {
        _center = QPoint(json["center"].toArray()[0].toInt(),
                         json["center"].toArray()[1].toInt());
        _size = json["size"].toInt();
}

void Triangle::initialize(const QPoint& startPoint) {
        _center = startPoint;
        _size = 0;
}

void Triangle::updateShape(const QPoint& currentPoint) {
        _size = static_cast<int>(QPointF(_center).manhattanLength()
                                 - QPointF(currentPoint).manhattanLength());
}

QRect Triangle::boundingRect() const {
        QPoint p1, p2, p3;
        calculateVertices(p1, p2, p3);

        int minX = std::min({p1.x(), p2.x(), p3.x()});
        int minY = std::min({p1.y(), p2.y(), p3.y()});
        int maxX = std::max({p1.x(), p2.x(), p3.x()});
        int maxY = std::max({p1.y(), p2.y(), p3.y()});

        QRect rect(QPoint(minX, minY), QPoint(maxX, maxY));

        return rect.adjusted(-2, -2, 2, 2);
}
} // namespace Shapes
