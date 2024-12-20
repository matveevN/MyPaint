#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include ".././shapes/IFigure.h"

class FileManager {
public:
        FileManager();
        FileManager(const FileManager&);
        FileManager(FileManager&&) noexcept;
        FileManager& operator=(const FileManager&);
        FileManager& operator=(FileManager&&) noexcept;
        ~FileManager();
        static void saveToImageWithMetadata(
            const QVector<IFigure*>& figures,
            const QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
        static void loadFromImageWithMetadata(
            QVector<IFigure*>& figures,
            QVector<QPair<IFigure*, IFigure*>>& connections,
            const QString& fileName);
};

#endif // FILE_MANAGER_H
