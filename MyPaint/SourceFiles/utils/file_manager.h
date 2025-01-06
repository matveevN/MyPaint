#pragma once

namespace Shapes {
class IFigure;

}

namespace Utils {
class FileManager final {
public:
        static void saveToImageWithMetadata(
            const QVector<Shapes::IFigure*>& figures,
            const QVector<QPair<Shapes::IFigure*, Shapes::IFigure*>>& connections,
            const QString& fileName,
            int width,
            int height);
        static void loadFromImageWithMetadata(
            QVector<Shapes::IFigure*>& figures,
            QVector<QPair<Shapes::IFigure*, Shapes::IFigure*>>& connections,
            const QString& fileName);
};
} // namespace Utils
