#include "rectangle.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight)
: _topLeft(topLeft)
, _bottomRight(bottomRight) {
}

Rectangle::Rectangle(const Rectangle&) = default;
Rectangle::Rectangle(Rectangle&&) noexcept = default;
Rectangle& Rectangle::operator=(const Rectangle&) = default;
Rectangle& Rectangle::operator=(Rectangle&&) noexcept = default;
Rectangle::~Rectangle() = default;

QPoint Rectangle::getTopLeft() const {
        return _topLeft;
}

void Rectangle::setTopLeft(const QPoint& point) {
        _topLeft = point;
}

QPoint Rectangle::getBottomRight() const {
        return _bottomRight;
}

void Rectangle::setBottomRight(const QPoint& bottomRight) {
        _bottomRight = bottomRight;
}

QRect Rectangle::getRect() const {
        return QRect(_topLeft, _bottomRight);
}

QPoint Rectangle::getCenter() const {
        return QPoint((_topLeft.x() + _bottomRight.x()) / 2,
                      (_topLeft.y() + _bottomRight.y()) / 2);
}

void Rectangle::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(QRect(_topLeft, _bottomRight));
}

bool Rectangle::contains(const QPoint& point) const {
        return _topLeft.x() <= point.x() && point.x() <= _bottomRight.x()
               && _topLeft.y() <= point.y() && point.y() <= _bottomRight.y();
}

void Rectangle::move(const QPoint& offset) {
        _topLeft += offset;
        _bottomRight += offset;
}

QString Rectangle::getType() const {
        return "rectangle";
}

QJsonObject Rectangle::toJson() const {
        QJsonObject json;
        json["type"] = getType();
        json["topLeft"] = QJsonArray{_topLeft.x(), _topLeft.y()};
        json["bottomRight"] = QJsonArray{_bottomRight.x(), _bottomRight.y()};
        return json;
}

void Rectangle::fromJson(const QJsonObject& json) {
        _topLeft = QPoint(json["topLeft"].toArray()[0].toInt(),
                          json["topLeft"].toArray()[1].toInt());
        _bottomRight = QPoint(json["bottomRight"].toArray()[0].toInt(),
                              json["bottomRight"].toArray()[1].toInt());
}

void Rectangle::initialize(const QPoint& startPoint) {
        setTopLeft(startPoint);
        setBottomRight(startPoint);
}

void Rectangle::updateShape(const QPoint& currentPoint) {
        setBottomRight(currentPoint);
}