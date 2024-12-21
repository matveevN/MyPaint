#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPainter>
#include <QPoint>
#include "IFigure.h"

class Triangle final : public IFigure {
public:
        Triangle(const QPoint& center, int size);

        Triangle(const Triangle&);
        Triangle(Triangle&&) noexcept;
        Triangle& operator=(const Triangle&);
        Triangle& operator=(Triangle&&) noexcept;
        ~Triangle();

        QPoint getCenter() const override;
        int getSize() const;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;
        void setThirdPoint(const QPoint& point);
        void move(const QPoint& offset) override;
        void setCenter(const QPoint& newCenter);
        void setSize(int newSize);
        void initialize(const QPoint& startPoint) override;

        void updateShape(const QPoint& currentPoint) override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject& json) override;

        QString getType() const override;

private:
        QPoint _center;
        int _size;

        void calculateVertices(QPoint& p1, QPoint& p2, QPoint& p3) const;
};

#endif // TRIANGLE_H
