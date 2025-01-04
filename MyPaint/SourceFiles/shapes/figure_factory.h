#pragma once

#include "IFigure.h"
namespace Shapes {
class FigureFactory {
public:
        [[nodiscard]] static IFigure* createFigure(const QString& type);
};
} // namespace Shapes
