#pragma once

#include "IFigure.h"

class FigureFactory {
public:
        //  фабричный метод для создания фигур
        static IFigure* createFigure(const QString& type);
};
