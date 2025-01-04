#pragma once

#include <qpainter.h>

namespace Shapes {
class IFigure;

}

namespace Shapes {
class FigureFactory {
public:
        [[nodiscard]] static IFigure* createFigure(const QString& type);
};
} // namespace Shapes
