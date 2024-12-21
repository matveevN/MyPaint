#include "rectangle.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight)
: topLeft(topLeft)
, bottomRight(bottomRight) {
}

QPoint Rectangle::getTopLeft() const {
        return topLeft;
}

void Rectangle::setTopLeft(const QPoint& point) {
        this->topLeft = point;
}

QPoint Rectangle::getBottomRight() const {
        return bottomRight;
}

void Rectangle::setBottomRight(const QPoint& bottomRight) {
        this->bottomRight = bottomRight;
}

QRect Rectangle::getRect() const {
        return QRect(topLeft, bottomRight);
}

QPoint Rectangle::getCenter() const {
        return QPoint((topLeft.x() + bottomRight.x()) / 2,
                      (topLeft.y() + bottomRight.y()) / 2);
}

void Rectangle::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(QRect(topLeft, bottomRight));
}

bool Rectangle::contains(const QPoint& point) const {
        return topLeft.x() <= point.x() && point.x() <= bottomRight.x()
               && topLeft.y() <= point.y() && point.y() <= bottomRight.y();
}

void Rectangle::move(const QPoint& offset) {
        topLeft += offset;
        bottomRight += offset;
}

QString Rectangle::getType() const {
        return "rectangle";
}

QJsonObject Rectangle::toJson() const {
        QJsonObject json;
        json["type"] = getType();
        json["topLeft"] = QJsonArray{topLeft.x(), topLeft.y()};
        json["bottomRight"] = QJsonArray{bottomRight.x(), bottomRight.y()};
        return json;
}

void Rectangle::fromJson(const QJsonObject& json) {
        topLeft = QPoint(json["topLeft"].toArray()[0].toInt(),
                         json["topLeft"].toArray()[1].toInt());
        bottomRight = QPoint(json["bottomRight"].toArray()[0].toInt(),
                             json["bottomRight"].toArray()[1].toInt());
}

void Rectangle::initialize(const QPoint& startPoint) {
        setTopLeft(startPoint);
        setBottomRight(startPoint);
}

void Rectangle::updateShape(const QPoint& currentPoint) {
        setBottomRight(currentPoint);
}
