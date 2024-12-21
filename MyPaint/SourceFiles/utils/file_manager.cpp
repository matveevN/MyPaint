#include "file_manager.h"
#include ".././shapes/ellipse.h"
#include ".././shapes/rectangle.h"
#include ".././shapes/triangle.h"

FileManager::FileManager() = default;
FileManager::FileManager(const FileManager&) = default;
FileManager::FileManager(FileManager&&) noexcept = default;
FileManager& FileManager::operator=(const FileManager&) = default;
FileManager& FileManager::operator=(FileManager&&) noexcept = default;
FileManager::~FileManager() = default;

void FileManager::saveToImageWithMetadata(
    const QVector<IFigure*>& figures,
    const QVector<QPair<IFigure*, IFigure*>>& connections,
    const QString& fileName) {
        QPixmap canvas(800, 600);
        canvas.fill(Qt::white);
        QPainter painter(&canvas);

        for (const auto& figure : figures) {
                figure->draw(painter);
        }

        painter.setPen(Qt::black);
        for (const auto& connection : connections) {
                IFigure* startFigure = connection.first;
                IFigure* endFigure = connection.second;

                if (startFigure && endFigure) {
                        painter.drawLine(startFigure->getCenter(),
                                         endFigure->getCenter());
                }
        }
        painter.end();

        if (!canvas.save(fileName, "PNG")) {
                qWarning() << "Не удалось сохранить изображение!";
                return;
        }

        QJsonObject rootObject;
        QJsonArray figuresArray;

        for (const auto& figure : figures) {
                figuresArray.append(figure->toJson());
        }

        QJsonArray connectionsArray;
        for (const auto& connection : connections) {
                QJsonObject connectionObject;
                int startIdx = figures.indexOf(connection.first);
                int endIdx = figures.indexOf(connection.second);

                if (startIdx != -1 && endIdx != -1) {
                        connectionObject["startIndex"] = startIdx;
                        connectionObject["endIndex"] = endIdx;
                        connectionsArray.append(connectionObject);
                }
        }

        rootObject["figures"] = figuresArray;
        rootObject["connections"] = connectionsArray;

        QJsonDocument doc(rootObject);
        QByteArray jsonData = doc.toJson();

        QImage image(fileName);
        image.setText("FigureData", QString::fromUtf8(jsonData));

        if (!image.save(fileName, "PNG")) {
                qWarning() << "Не удалось сохранить изображение с метаданными!";
        }
}

void FileManager::loadFromImageWithMetadata(
    QVector<IFigure*>& figures,
    QVector<QPair<IFigure*, IFigure*>>& connections,
    const QString& fileName) {
        QImage image(fileName);
        if (image.isNull()) {
                qWarning() << "Не удалось загрузить изображение!";
                return;
        }

        QString jsonData = image.text("FigureData");
        if (jsonData.isEmpty()) {
                qWarning() << "Метаданные не найдены!";
                return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject rootObject = doc.object();

        QJsonArray figuresArray = rootObject["figures"].toArray();
        figures.clear();
        for (const QJsonValue& value : std::as_const(figuresArray)) {
                QJsonObject figureObject = value.toObject();
                QString type = figureObject["type"].toString();

                IFigure* figure = nullptr;
                if (type == "rectangle") {
                        figure = new Rectangle(QPoint(), QPoint());
                } else if (type == "ellipse") {
                        figure = new Ellipse(QPoint(), 0, 0);
                } else if (type == "triangle") {
                        figure = new Triangle(QPoint(), 0);
                }

                if (figure) {
                        figure->fromJson(figureObject);
                        figures.append(figure);
                }
        }

        QJsonArray connectionsArray = rootObject["connections"].toArray();
        connections.clear();
        for (const QJsonValue& value : std::as_const(connectionsArray)) {
                QJsonObject connectionObject = value.toObject();
                int startIdx = connectionObject["startIndex"].toInt();
                int endIdx = connectionObject["endIndex"].toInt();

                if (startIdx >= 0 && startIdx < figures.size() && endIdx >= 0
                    && endIdx < figures.size()) {
                        connections.append(
                            qMakePair(figures[startIdx], figures[endIdx]));
                }
        }
}
