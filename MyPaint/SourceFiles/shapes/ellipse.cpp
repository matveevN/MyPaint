#include "ellipse.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

namespace Shapes {

Ellipse::Ellipse(const QPoint& center, int radiusX, int radiusY)
: _center(center)
, _radiusX(radiusX)
, _radiusY(radiusY) {
}

Ellipse::~Ellipse() = default;

QPoint Ellipse::getCenter() const {
        return _center;
}

void Ellipse::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawEllipse(_center, _radiusX, _radiusY);
}

bool Ellipse::contains(const QPoint& point) const {
        float dx = point.x() - _center.x();
        float dy = point.y() - _center.y();
        return (dx * dx) / static_cast<float>(_radiusX * _radiusX)
                   + (dy * dy) / static_cast<float>(_radiusY * _radiusY)
               <= 1.0f;
}

void Ellipse::move(const QPoint& offset) {
        _center += offset;
}

QString Ellipse::getType() const {
        return "ellipse";
}

QJsonObject Ellipse::toJson() const {
        QJsonObject json;
        json["type"] = getType();
        json["center"] = QJsonArray{_center.x(), _center.y()};
        json["radiusX"] = _radiusX;
        json["radiusY"] = _radiusY;
        return json;
}

void Ellipse::fromJson(const QJsonObject& json) {
        _center = QPoint(json["center"].toArray()[0].toInt(),
                         json["center"].toArray()[1].toInt());
        _radiusX = json["radiusX"].toInt();
        _radiusY = json["radiusY"].toInt();
}

void Ellipse::initialize(const QPoint& startPoint) {
        _center = startPoint;
        _radiusX = 0;
        _radiusY = 0;
}

void Ellipse::updateShape(const QPoint& currentPoint) {
        _radiusX = std::abs(_center.x() - currentPoint.x());
        _radiusY = std::abs(_center.y() - currentPoint.y());
}

QRect Ellipse::boundingRect() const {
        QRect rect(_center.x() - _radiusX,
                   _center.y() - _radiusY,
                   2 * _radiusX,
                   2 * _radiusY);

        return rect.adjusted(-2, -2, 2, 2);
}
} // namespace Shapes
