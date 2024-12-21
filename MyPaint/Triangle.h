#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPainter>
#include <QPoint>
#include "IFigure.h"

class Triangle : public IFigure {
public:
        Triangle(const QPoint& center, int size);
        Triangle(const Triangle&) = default;
        Triangle(Triangle&&) noexcept = default;
        Triangle& operator=(const Triangle&) = default;
        Triangle& operator=(Triangle&&) noexcept = default;
        ~Triangle() = default;

        QPoint getCenter() const override;
        int getSize() const;

        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;
        QString getType() const override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setThirdPoint(const QPoint& point);
        void move(const QPoint& offset) override;
        void setCenter(const QPoint& newCenter);
        void setSize(int newSize);

private:
        QPoint center;
        int size;

        void calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const;
};

#endif // TRIANGLE_H
