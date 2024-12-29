#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include "IFigure.h"

namespace Utils {
class FileManager final {
public:
        static void saveToImageWithMetadata(
            const QVector<Shapes::IFigure*>& figures,
            const QVector<QPair<Shapes::IFigure*, Shapes::IFigure*>>& connections,
            const QString& fileName);
        static void loadFromImageWithMetadata(
            QVector<Shapes::IFigure*>& figures,
            QVector<QPair<Shapes::IFigure*, Shapes::IFigure*>>& connections,
            const QString& fileName);
};
} // namespace Utils
