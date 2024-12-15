#include "mainwindow.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow) {
        ui->setupUi(this);

        connect(ui->rectangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onRectangleButtonClicked);
        connect(ui->ellipseButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onEllipseButtonClicked);
        connect(ui->triangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onTriangleButtonClicked);
        connect(ui->moveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onMoveButtonClicked);
        connect(ui->deleteButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onDeleteButtonClicked);
        connect(ui->saveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onSaveButtonClicked);
        connect(ui->loadButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onLoadButtonClicked);
        connect(ui->connectButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onConnectButtonClicked);
}
MainWindow::~MainWindow() {
}

void MainWindow::onRectangleButtonClicked() {
        currentFigure = new Rectangle(QPoint(), QPoint());
        isDrawing = true;
}

void MainWindow::onEllipseButtonClicked() {
        currentFigure = new Ellipse(QPoint(), 0, 0);
        isDrawing = true;
}

void MainWindow::onTriangleButtonClicked() {
        currentFigure = new Triangle(QPoint(), 0);
        isDrawing = true;
}

void MainWindow::onMoveButtonClicked() {
        isMoving = !isMoving;
        if (isMoving) {
                setCursor(Qt::ClosedHandCursor);
        } else {
                setCursor(Qt::ArrowCursor);
        }
        if (!isMoving) {
                movingFigure = nullptr;
        }
}

void MainWindow::onDeleteButtonClicked() {
        isDeleting = true;
}

void MainWindow::onConnectButtonClicked() {
        isConnecting = true;
        startConnectionFigure = nullptr;
        update();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
                if (isDeleting) {
                        for (int i = 0; i < figures.size(); ++i) {
                                if (figures[i]->contains(event->pos())) {
                                        for (int j = 0; j < connections.size();
                                             ++j) {
                                                if (connections[j].first
                                                        == figures[i]
                                                    || connections[j].second
                                                           == figures[i]) {
                                                        connections.removeAt(j);
                                                        --j;
                                                }
                                        }

                                        delete figures[i];
                                        figures.removeAt(i);
                                        isDeleting = false;
                                        update();
                                        break;
                                }
                        }
                } else if (isDrawing) {
                        startPoint = event->pos();
                        if (auto rect = dynamic_cast<Rectangle*>(
                                currentFigure)) {
                                rect->setTopLeft(startPoint);
                                rect->setBottomRight(startPoint);
                        } else if (auto ellipse = dynamic_cast<Ellipse*>(
                                       currentFigure)) {
                                ellipse->setCenter(startPoint);
                        } else if (auto triangle = dynamic_cast<Triangle*>(
                                       currentFigure)) {
                                triangle->setCenter(startPoint);
                                triangle->setSize(0);
                        }
                } else if (isConnecting) {
                        IFigure* clickedFigure = nullptr;

                        for (auto figure : figures) {
                                if (figure->contains(event->pos())) {
                                        clickedFigure = figure;
                                        break;
                                }
                        }

                        if (!startConnectionFigure) {
                                startConnectionFigure = clickedFigure;
                                connectionCursor = event->pos();
                        } else if (clickedFigure
                                   && clickedFigure != startConnectionFigure) {
                                connections.append(
                                    qMakePair(startConnectionFigure,
                                              clickedFigure));
                                startConnectionFigure = nullptr;
                                isConnecting = false;
                        } else {
                                startConnectionFigure = nullptr;
                                isConnecting = false;
                        }
                        update();
                } else if (isMoving) {
                        movingFigure = nullptr;
                        for (auto figure : figures) {
                                if (figure->contains(event->pos())) {
                                        movingFigure = figure;
                                        moveStartPos = event->pos();
                                        break;
                                }
                        }
                }
        } else if (event->button() == Qt::RightButton) {
                if (isMoving) {
                        isMoving = false;
                        movingFigure = nullptr;
                        setCursor(Qt::ArrowCursor);
                        update();
                } else if (isDrawing) {
                        currentFigure = nullptr;
                        isDrawing = false;
                        update();
                } else if (isConnecting) {
                        startConnectionFigure = nullptr;
                        isConnecting = false;
                        update();
                }
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
        if (isDrawing) {
                if (auto rect = dynamic_cast<Rectangle*>(currentFigure)) {
                        rect->setBottomRight(event->pos());
                } else if (auto ellipse = dynamic_cast<Ellipse*>(
                               currentFigure)) {
                        QPoint center = startPoint;
                        int radiusX = abs(center.x() - event->pos().x());
                        int radiusY = abs(center.y() - event->pos().y());
                        ellipse->setRadiusX(radiusX);
                        ellipse->setRadiusY(radiusY);
                } else if (auto triangle = dynamic_cast<Triangle*>(
                               currentFigure)) {
                        int size = static_cast<int>(
                            QPointF(startPoint).manhattanLength()
                            - QPointF(event->pos()).manhattanLength());
                        triangle->setSize(size);
                }
                update();
        } else if (isConnecting && startConnectionFigure) {
                connectionCursor = event->pos();
        } else if (isMoving && movingFigure) {
                QPoint offset = event->pos() - moveStartPos;
                movingFigure->move(offset);
                moveStartPos = event->pos();
                update();
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
        if (isDrawing && event->button() == Qt::LeftButton) {
                figures.append(currentFigure);
                currentFigure = nullptr;
                isDrawing = false;
                update();
        } else if (isMoving && event->button() == Qt::LeftButton) {
                isMoving = false;
                movingFigure = nullptr;
                setCursor(Qt::ArrowCursor);
        }
}

void MainWindow::paintEvent(QPaintEvent* event) {
        QPainter painter(this);

        if (!backgroundPixmap.isNull()) {
                painter.drawPixmap(0, 0, backgroundPixmap);
        } else {
                painter.fillRect(rect(), Qt::white);
        }

        for (const auto& figure : figures) {
                figure->draw(painter);
        }

        painter.setPen(Qt::black);
        for (const auto& connection : connections) {
                IFigure* start = connection.first;
                IFigure* end = connection.second;
                if (start && end) {
                        painter.drawLine(start->getCenter(), end->getCenter());
                }
        }

        if (isConnecting && startConnectionFigure) {
                painter.setPen(Qt::DashLine);
                if (connectionCursor != QPoint()) {
                        QPoint startPoint = startConnectionFigure->getCenter();
                        painter.drawLine(startPoint, connectionCursor);
                }
        }

        if (isDrawing && currentFigure) {
                currentFigure->draw(painter);
        }
}

void MainWindow::onSaveButtonClicked() {
        QString fileName
            = QFileDialog::getSaveFileName(this,
                                           tr("Сохранить рисунок"),
                                           "",
                                           tr("JSON Files (*.json)"));
        if (fileName.isEmpty())
                return;

        saveFiguresToJson(fileName);
}

void MainWindow::onLoadButtonClicked() {
        QString fileName
            = QFileDialog::getOpenFileName(this,
                                           tr("Загрузить рисунок"),
                                           "",
                                           tr("JSON Files (*.json)"));
        if (fileName.isEmpty())
                return;

        loadFiguresFromJson(fileName);
}

void MainWindow::saveFiguresToJson(const QString& fileName) {
        QJsonObject rootObject;

        QJsonArray figuresArray;

        for (const auto& figure : figures) {
                QJsonObject figureObject;

                if (auto rect = dynamic_cast<Rectangle*>(figure)) {
                        figureObject["type"] = "rectangle";
                        figureObject["topLeft"]
                            = QJsonArray{rect->getTopLeft().x(),
                                         rect->getTopLeft().y()};
                        figureObject["bottomRight"]
                            = QJsonArray{rect->getBottomRight().x(),
                                         rect->getBottomRight().y()};
                } else if (auto ellipse = dynamic_cast<Ellipse*>(figure)) {
                        figureObject["type"] = "ellipse";
                        figureObject["center"]
                            = QJsonArray{ellipse->getCenter().x(),
                                         ellipse->getCenter().y()};
                        figureObject["radiusX"] = ellipse->getRadiusX();
                        figureObject["radiusY"] = ellipse->getRadiusY();
                } else if (auto triangle = dynamic_cast<Triangle*>(figure)) {
                        figureObject["type"] = "triangle";
                        figureObject["center"]
                            = QJsonArray{triangle->getCenter().x(),
                                         triangle->getCenter().y()};
                        figureObject["size"] = triangle->getSize();
                }

                figuresArray.append(figureObject);
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

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
                QJsonDocument doc(rootObject);
                file.write(doc.toJson());
        }
}

void MainWindow::loadFiguresFromJson(const QString& fileName) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
                return;
        }

        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject rootObject = doc.object();

        QJsonArray figuresArray = rootObject["figures"].toArray();
        figures.clear();

        for (const QJsonValue& value : figuresArray) {
                QJsonObject figureObject = value.toObject();
                QString type = figureObject["type"].toString();

                if (type == "rectangle") {
                        QPoint topLeft(
                            figureObject["topLeft"].toArray()[0].toInt(),
                            figureObject["topLeft"].toArray()[1].toInt());
                        QPoint bottomRight(
                            figureObject["bottomRight"].toArray()[0].toInt(),
                            figureObject["bottomRight"].toArray()[1].toInt());
                        figures.append(new Rectangle(topLeft, bottomRight));
                } else if (type == "ellipse") {
                        QPoint
                            center(figureObject["center"].toArray()[0].toInt(),
                                   figureObject["center"].toArray()[1].toInt());
                        int radiusX = figureObject["radiusX"].toInt();
                        int radiusY = figureObject["radiusY"].toInt();
                        figures.append(new Ellipse(center, radiusX, radiusY));
                } else if (type == "triangle") {
                        QPoint
                            center(figureObject["center"].toArray()[0].toInt(),
                                   figureObject["center"].toArray()[1].toInt());
                        int size = figureObject["size"].toInt();
                        figures.append(new Triangle(center, size));
                }
        }

        QJsonArray connectionsArray = rootObject["connections"].toArray();
        for (const QJsonValue& value : connectionsArray) {
                QJsonObject connectionObject = value.toObject();
                int startIdx = connectionObject["startIndex"].toInt();
                int endIdx = connectionObject["endIndex"].toInt();

                if (startIdx >= 0 && startIdx < figures.size() && endIdx >= 0
                    && endIdx < figures.size()) {
                        IFigure* startFigure = figures[startIdx];
                        IFigure* endFigure = figures[endIdx];
                        connections.append(qMakePair(startFigure, endFigure));
                }
        }

        update();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape) {
                if (isDrawing) {
                        currentFigure = nullptr;
                        isDrawing = false;
                        update();
                } else if (isMoving) {
                        isMoving = false;
                        movingFigure = nullptr;
                        setCursor(Qt::ArrowCursor);
                        update();
                } else if (isConnecting) {
                        startConnectionFigure = nullptr;
                        isConnecting = false;
                        update();
                }
        }

        QMainWindow::keyPressEvent(event);
}
