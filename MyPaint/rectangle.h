#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QPainter>
#include <QPoint>
#include <QRect>
#include "IFigure.h"

class Rectangle : public IFigure {
public:
        Rectangle(const QPoint& topLeft, const QPoint& bottomRight);
        Rectangle(const Rectangle&) = default;
        Rectangle(Rectangle&&) noexcept = default;
        Rectangle& operator=(const Rectangle&) = default;
        Rectangle& operator=(Rectangle&&) noexcept = default;
        ~Rectangle() = default;

        QRect getRect() const;
        QPoint getBottomRight() const;
        QPoint getCenter() const override;
        QPoint getTopLeft() const;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        QRect boundingBox() const {
                return QRect(topLeft, bottomRight);
        }
        QString getType() const override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;
        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setBottomRight(const QPoint& bottomRight);
        void setTopLeft(const QPoint& point);
        void move(const QPoint& offset) override;

private:
        QPoint topLeft;
        QPoint bottomRight;
};

#endif // RECTANGLE_H
