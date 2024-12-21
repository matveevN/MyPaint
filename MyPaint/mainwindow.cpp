#include "mainwindow.h"
#include "Triangle.h"
#include "commands.h"
#include "ellipse.h"
#include "rectangle.h"
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
        qDeleteAll(figures);
        delete ui;
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

void MainWindow::onSaveButtonClicked() {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Сохранить рисунок"),
                                                        "",
                                                        tr(" Files (*.png)"));
        if (fileName.isEmpty())
                return;

        Commands::saveToImageWithMetadata(figures, connections, fileName);
}

void MainWindow::onLoadButtonClicked() {
        QString fileName
            = QFileDialog::getOpenFileName(this,
                                           tr("Загрузить рисунок"),
                                           "",
                                           tr("PNG Files (*.png)"));
        if (fileName.isEmpty())
                return;

        Commands::loadFromImageWithMetadata(figures, connections, fileName);
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

                        if (currentFigure) {
                                currentFigure->initialize(startPoint);
                        }
                } else if (isConnecting) {
                        IFigure* clickedFigure = nullptr;

                        for (auto& figure : figures) {
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
                        for (auto& figure : figures) {
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
                if (currentFigure) {
                        currentFigure->updateShape(event->pos());
                }
                update();
        } else if (isConnecting && startConnectionFigure) {
                connectionCursor = event->pos();
                update();
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

        for (const auto& figure : std::as_const(figures)) {
                figure->draw(painter);
        }

        painter.setPen(QPen(Qt::black, 2));
        for (const auto& connection : std::as_const(connections)) {
                IFigure* start = connection.first;
                IFigure* end = connection.second;
                if (start && end) {
                        painter.drawLine(start->getCenter(), end->getCenter());
                }
        }

        if (isConnecting && startConnectionFigure) {
                QPoint startPoint = startConnectionFigure->getCenter();
                painter.setPen(QPen(Qt::black, 2));
                painter.drawLine(startPoint, connectionCursor);
        }

        if (isDrawing && currentFigure) {
                currentFigure->draw(painter);
        }
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
