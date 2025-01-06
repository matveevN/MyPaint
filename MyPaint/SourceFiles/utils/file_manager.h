#pragma once

namespace Shapes {
class IShapes;

}

namespace Utils {
class FileManager final {
public:
        static void saveToImageWithMetadata(
            const QVector<Shapes::IShapes*>& figures,
            const QVector<QPair<Shapes::IShapes*, Shapes::IShapes*>>& connections,
            const QString& fileName,
            int width,
            int height);
        static void loadFromImageWithMetadata(
            QVector<Shapes::IShapes*>& figures,
            QVector<QPair<Shapes::IShapes*, Shapes::IShapes*>>& connections,
            const QString& fileName);
};
} // namespace Utils
