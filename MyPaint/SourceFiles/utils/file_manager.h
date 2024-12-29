#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include ".././shapes/IFigure.h"

class FileManager final {
public:
        static void saveToImageWithMetadata(
            const QVector<IFigure*>& figures,
            const QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
        static void loadFromImageWithMetadata(
            QVector<IFigure*>& figures,
            QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
};
