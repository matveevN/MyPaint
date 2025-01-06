#pragma once

namespace Shapes {
class IShapes;

}

namespace Shapes {
class FigureFactory {
public:
        [[nodiscard]] static IShapes* createFigure(const QString& type);
};
} // namespace Shapes
