#ifndef COMMANDS_H
#define COMMANDS_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include "IFigure.h"

class Commands {
public:
        Commands() = default;
        Commands(const Commands&) = default;
        Commands(Commands&&) noexcept = default;
        Commands& operator=(const Commands&) = default;
        Commands& operator=(Commands&&) noexcept = default;
        ~Commands() = default;
        static void saveToImageWithMetadata(
            const QVector<IFigure*>& figures,
            const QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
        static void loadFromImageWithMetadata(
            QVector<IFigure*>& figures,
            QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
};

#endif // COMMANDS_H
