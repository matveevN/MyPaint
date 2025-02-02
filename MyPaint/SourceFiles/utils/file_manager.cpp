#include "file_manager.h"
#include "IShape.h"
#include "figure_factory.h"

namespace Utils {
void FileManager::saveToImageWithMetadata(
    const QVector<Shapes::IShapes*>& figures,
    const QVector<QPair<Shapes::IShapes*, Shapes::IShapes*>>& connections,
    const QString& fileName,
    int width,
    int height) {
        QPixmap canvas(width, height);
        canvas.fill(Qt::white);
        QPainter painter(&canvas);
        painter.setPen(Qt::black);

        for (const auto& figure : figures) {
                figure->draw(painter);
        }

        for (const auto& connection : connections) {
                if (connection.first && connection.second) {
                        painter.drawLine(connection.first->getCenter(),
                                         connection.second->getCenter());
                }
        }
        painter.end();

        if (!canvas.save(fileName, "PNG")) {
                qWarning() << "Не удалось сохранить изображение!";
                return;
        }

        QJsonArray figuresArray;
        QJsonArray connectionsArray;

        for (const auto& figure : figures) {
                figuresArray.append(figure->toJson());
        }

        for (const auto& connection : connections) {
                int startIdx = figures.indexOf(connection.first);
                int endIdx = figures.indexOf(connection.second);

                if (startIdx != -1 && endIdx != -1) {
                        QJsonObject connectionObject;
                        connectionObject["startIndex"] = startIdx;
                        connectionObject["endIndex"] = endIdx;
                        connectionsArray.append(connectionObject);
                }

                // bool condition = (startIdx != -1) && (endIdx != -1);
                // connectionsArray.append(
                //     QJsonObject({{"startIndex", startIdx * condition},
                //                  {"endIndex", endIdx * condition}}));

                // если startIdx or endIdx == -1 В массив connectionsArray
                // добавится объект с нулевыми значениями:

                // Invalid operands to binary expression ('bool' and 'QJsonObject')
                // connectionsArray += condition
                //                     * QJsonObject({{"startIndex", startIdx},
                //                                    {"endIndex", endIdx}});
        }

        QJsonObject rootObject;
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
    QVector<Shapes::IShapes*>& figures,
    QVector<QPair<Shapes::IShapes*, Shapes::IShapes*>>& connections,
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

                Shapes::IShapes* figure = Shapes::FigureFactory::createFigure(
                    type);
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
} // namespace Utils
