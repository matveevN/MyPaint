#ifndef IFIGURE_H
#define IFIGURE_H

#include <qpainter.h>
class IFigure {
public:
        virtual QPoint getCenter() const = 0;

        virtual bool contains(const QPoint& point) const = 0;

        virtual void draw(QPainter& painter) const = 0;
        virtual void move(const QPoint& offset) = 0;

        virtual void initialize(const QPoint& startPoint) = 0;

        virtual void updateShape(const QPoint& currentPoint) = 0;

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject& json) = 0;

        virtual QString getType() const = 0;

        virtual ~IFigure() = default;
};

#endif // IFIGURE_H
