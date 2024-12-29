#pragma once

#include <QPainter>
#include <QPoint>
#include "IFigure.h"

class Triangle final : public IFigure {
public:
        Triangle(const QPoint& center, int size);

        ~Triangle();

        QPoint getCenter() const override;

        bool contains(const QPoint& point) const override;

        void draw(QPainter& painter) const override;

        void move(const QPoint& offset) override;

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
