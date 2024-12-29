#pragma once

#include "IFigure.h"
namespace Shapes {
class FigureFactory {
public:
        //  фабричный метод для создания фигур
        [[nodiscard]] static IFigure* createFigure(const QString& type);
};
} // namespace Shapes
