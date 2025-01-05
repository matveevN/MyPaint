#pragma once

namespace Shapes {
class IFigure;

}

namespace Shapes {
class FigureFactory {
public:
        [[nodiscard]] static IFigure* createFigure(const QString& type);
};
} // namespace Shapes
