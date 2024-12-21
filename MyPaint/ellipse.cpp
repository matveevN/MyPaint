#include "ellipse.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

Ellipse::Ellipse(const QPoint& center, int radiusX, int radiusY)
: center(center)
, radiusX(radiusX)
, radiusY(radiusY) {
}

Ellipse::Ellipse(const Ellipse&) = default;
Ellipse::Ellipse(Ellipse&&) noexcept = default;
Ellipse& Ellipse::operator=(const Ellipse&) = default;
Ellipse& Ellipse::operator=(Ellipse&&) noexcept = default;
Ellipse::~Ellipse() = default;

QPoint Ellipse::getCenter() const {
        return center;
}

void Ellipse::setCenter(const QPoint& newCenter) {
        center = newCenter;
}

int Ellipse::getRadiusX() const {
        return radiusX;
}

void Ellipse::setRadiusX(int radius) {
        radiusX = radius;
}

int Ellipse::getRadiusY() const {
        return radiusY;
}

void Ellipse::setRadiusY(int radius) {
        radiusY = radius;
}

void Ellipse::setRadius(int radiusX, int radiusY) {
        this->radiusX = radiusX;
        this->radiusY = radiusY;
}

void Ellipse::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawEllipse(center, radiusX, radiusY);
}

bool Ellipse::contains(const QPoint& point) const {
        float dx = point.x() - center.x();
        float dy = point.y() - center.y();
        return (dx * dx) / static_cast<float>(radiusX * radiusX)
                   + (dy * dy) / static_cast<float>(radiusY * radiusY)
               <= 1.0f;
}

void Ellipse::move(const QPoint& offset) {
        center += offset;
}

QString Ellipse::getType() const {
        return "ellipse";
}

QJsonObject Ellipse::toJson() const {
        QJsonObject json;
        json["type"] = getType();
        json["center"] = QJsonArray{center.x(), center.y()};
        json["radiusX"] = radiusX;
        json["radiusY"] = radiusY;
        return json;
}

void Ellipse::fromJson(const QJsonObject& json) {
        center = QPoint(json["center"].toArray()[0].toInt(),
                        json["center"].toArray()[1].toInt());
        radiusX = json["radiusX"].toInt();
        radiusY = json["radiusY"].toInt();
}

void Ellipse::initialize(const QPoint& startPoint) {
        center = startPoint;
        radiusX = 0;
        radiusY = 0;
}

void Ellipse::updateShape(const QPoint& currentPoint) {
        radiusX = std::abs(center.x() - currentPoint.x());
        radiusY = std::abs(center.y() - currentPoint.y());
}
