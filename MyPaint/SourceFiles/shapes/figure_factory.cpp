#include "figure_factory.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"

IFigure* FigureFactory::createFigure(const QString& type) {
        if (type == "rectangle") {
                return new Rectangle(QPoint(), QPoint());
        } else if (type == "ellipse") {
                return new Ellipse(QPoint(), 0, 0);
        } else if (type == "triangle") {
                return new Triangle(QPoint(), 0);
        }
        return nullptr;
}
