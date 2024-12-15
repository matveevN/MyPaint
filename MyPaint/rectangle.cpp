#include "rectangle.h"

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight)
: topLeft(topLeft)
, bottomRight(bottomRight) {
}

// Реализация метода draw() из IFigure
void Rectangle::draw(QPainter& painter) const {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(QRect(topLeft, bottomRight));
}

QRect Rectangle::getRect() const {
        return QRect(topLeft, bottomRight);
}

void Rectangle::setBottomRight(const QPoint& bottomRight) {
        this->bottomRight = bottomRight;
}

void Rectangle::setTopLeft(const QPoint& point) {
        this->topLeft = point;
}

bool Rectangle::contains(const QPoint& point) const {
        return topLeft.x() <= point.x() && point.x() <= bottomRight.x()
               && topLeft.y() <= point.y() && point.y() <= bottomRight.y();
}

void Rectangle::move(const QPoint& offset) {
        topLeft += offset;
        bottomRight += offset;
}

QPoint Rectangle::getCenter() const {
        return QPoint((topLeft.x() + bottomRight.x()) / 2,
                      (topLeft.y() + bottomRight.y()) / 2);
}

QPoint Rectangle::getTopLeft() const {
        return topLeft;
}

QPoint Rectangle::getBottomRight() const {
        return bottomRight;
}
