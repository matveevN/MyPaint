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
        Commands();
        Commands(const Commands&);
        Commands(Commands&&) noexcept;
        Commands& operator=(const Commands&);
        Commands& operator=(Commands&&) noexcept;
        ~Commands();
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
