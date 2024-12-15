#ifndef IFIGURE_H
#define IFIGURE_H

#include <qpainter.h>
class IFigure {
public:
        virtual QPoint getCenter() const = 0;

        virtual bool contains(const QPoint& point) const = 0;

        virtual void draw(QPainter& painter) const = 0;
        virtual void move(const QPoint& offset) = 0;

        virtual ~IFigure() = default;
};

#endif // IFIGURE_H
