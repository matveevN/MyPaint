#include "mainwindow.h"
#include "IFigure.h"
#include "ellipse.h"
#include "file_manager.h"
#include "rectangle.h"
#include "triangle.h"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent) {
        setFixedSize(800, 600);
        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* rectangleButton = new QPushButton("Прямоугольник", this);
        QPushButton* ellipseButton = new QPushButton("Эллипс", this);
        QPushButton* triangleButton = new QPushButton("Треугольник", this);
        QPushButton* moveButton = new QPushButton("Переместить", this);
        QPushButton* deleteButton = new QPushButton("Удалить", this);
        QPushButton* saveButton = new QPushButton("Сохранить", this);
        QPushButton* loadButton = new QPushButton("Загрузить", this);
        QPushButton* connectButton = new QPushButton("Связь", this);

        QString buttonStyle = R"(
        QPushButton {
            background-color: #333333; 
            color: white; 
            border-radius: 5px; 
            padding: 5px;
        }
        QPushButton:hover {
            background-color: #444444;
        }
        QPushButton:pressed {
            background-color: #555555;
        }
    )";

        rectangleButton->setStyleSheet(buttonStyle);
        ellipseButton->setStyleSheet(buttonStyle);
        triangleButton->setStyleSheet(buttonStyle);
        moveButton->setStyleSheet(buttonStyle);
        deleteButton->setStyleSheet(buttonStyle);
        saveButton->setStyleSheet(buttonStyle);
        loadButton->setStyleSheet(buttonStyle);
        connectButton->setStyleSheet(buttonStyle);

        buttonLayout->addWidget(rectangleButton);
        buttonLayout->addWidget(triangleButton);
        buttonLayout->addWidget(ellipseButton);
        buttonLayout->addWidget(connectButton);
        buttonLayout->addWidget(moveButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(loadButton);
        buttonLayout->addWidget(saveButton);

        mainLayout->addLayout(buttonLayout);
        mainLayout->addStretch();

        centralWidget->setLayout(mainLayout);

        connect(rectangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onRectangleButtonClicked);
        connect(ellipseButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onEllipseButtonClicked);
        connect(triangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onTriangleButtonClicked);
        connect(moveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onMoveButtonClicked);
        connect(deleteButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onDeleteButtonClicked);
        connect(saveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onSaveButtonClicked);
        connect(loadButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onLoadButtonClicked);
        connect(connectButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onConnectButtonClicked);
}
MainWindow::~MainWindow() {
        qDeleteAll(_figures);
}

void MainWindow::onRectangleButtonClicked() {
        _currentFigure = new Shapes::Rectangle(QPoint(), QPoint());
        _isDrawing = true;
}

void MainWindow::onEllipseButtonClicked() {
        _currentFigure = new Shapes::Ellipse(QPoint(), 0, 0);
        _isDrawing = true;
}

void MainWindow::onTriangleButtonClicked() {
        _currentFigure = new Shapes::Triangle(QPoint(), 0);
        _isDrawing = true;
}

void MainWindow::onMoveButtonClicked() {
        _isMoving = !_isMoving;
        setCursor(_isMoving ? Qt::ClosedHandCursor : Qt::ArrowCursor);
        _movingFigure = !_isMoving ? nullptr : _movingFigure;
}

void MainWindow::onDeleteButtonClicked() {
        _isDeleting = true;
}

void MainWindow::onConnectButtonClicked() {
        _isConnecting = true;
        _startConnectionFigure = nullptr;
}

void MainWindow::onSaveButtonClicked() {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Сохранить рисунок"),
                                                        "",
                                                        tr(" Files (*.png)"));
        if (fileName.isEmpty())
                return;

        Utils::FileManager::saveToImageWithMetadata(_figures,
                                                    _connections,
                                                    fileName);
}

void MainWindow::onLoadButtonClicked() {
        QString fileName
            = QFileDialog::getOpenFileName(this,
                                           tr("Загрузить рисунок"),
                                           "",
                                           tr("PNG Files (*.png)"));
        if (fileName.isEmpty())
                return;

        QRect updateRect(0, 0, 0, 0);

        Utils::FileManager::loadFromImageWithMetadata(_figures,
                                                      _connections,
                                                      fileName);

        for (const auto& figure : std::as_const(_figures)) {
                updateRect = updateRect.united(figure->boundingRect());
        }

        for (const auto& connection : std::as_const(_connections)) {
                updateRect = updateRect.united(
                    QRect(connection.first->boundingRect().center(),
                          connection.second->boundingRect().center()));
        }

        update(updateRect);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
                if (_isDrawing) {
                        _startPoint = event->pos();
                        if (_currentFigure)
                                _currentFigure->initialize(_startPoint);
                        return;
                } else if (_isMoving) {
                        _movingFigure = nullptr;
                        for (auto& figure : _figures) {
                                if (figure->contains(event->pos())) {
                                        _movingFigure = figure;
                                        _moveStartPos = event->pos();
                                        break;
                                }
                        }
                        return;
                } else if (_isConnecting) {
                        Shapes::IFigure* clickedFigure = nullptr;

                        for (auto& figure : _figures) {
                                if (figure->contains(event->pos())) {
                                        clickedFigure = figure;
                                        break;
                                }
                        }

                        if (!_startConnectionFigure) {
                                _startConnectionFigure = clickedFigure;
                                _connectionCursor = event->pos();
                        } else if (clickedFigure
                                   && clickedFigure != _startConnectionFigure) {
                                _connections.append(
                                    qMakePair(_startConnectionFigure,
                                              clickedFigure));

                                QRect startRect = _startConnectionFigure
                                                      ->boundingRect();
                                QRect endRect = clickedFigure->boundingRect();

                                QRect connectionRect = startRect.united(endRect);
                                update(QRegion(connectionRect));

                                _startConnectionFigure = nullptr;
                                _isConnecting = false;
                        }
                        return;
                } else if (_isDeleting) {
                        for (int i = 0; i < _figures.size(); ++i) {
                                if (_figures[i]->contains(event->pos())) {
                                        QRect updateRect;

                                        for (int j = 0; j < _connections.size();
                                             ++j) {
                                                if (_connections[j].first
                                                        == _figures[i]
                                                    || _connections[j].second
                                                           == _figures[i]) {
                                                        QPoint start
                                                            = _connections[j]
                                                                  .first
                                                                  ->getCenter();
                                                        QPoint end
                                                            = _connections[j]
                                                                  .second
                                                                  ->getCenter();
                                                        QRect connectionRect
                                                            = QRect(start, end)
                                                                  .normalized();
                                                        updateRect
                                                            = updateRect.united(
                                                                connectionRect);

                                                        _connections.removeAt(j);
                                                        --j;
                                                }
                                        }

                                        QRect figureRect = _figures[i]
                                                               ->boundingRect();
                                        updateRect = updateRect.united(
                                            figureRect);

                                        delete _figures[i];
                                        _figures.removeAt(i);

                                        _isDeleting = false;

                                        update(QRegion(updateRect));
                                        break;
                                }
                        }
                        return;
                }

        } else if (event->button() == Qt::RightButton) {
                if (_isDrawing && _currentFigure) {
                        QRect oldRect = _currentFigure->boundingRect();
                        _currentFigure = nullptr;
                        _isDrawing = false;

                        update(QRegion(oldRect));
                } else if (_isMoving && _movingFigure) {
                        QRect oldRect = _movingFigure->boundingRect();
                        _isMoving = false;
                        _movingFigure = nullptr;
                        setCursor(Qt::ArrowCursor);

                        update(QRegion(oldRect));
                }

        } else if (_isConnecting && _startConnectionFigure) {
                QRect startRect = _startConnectionFigure->boundingRect();
                _startConnectionFigure = nullptr;
                _isConnecting = false;

                update(QRegion(startRect));
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
        if (_isDrawing && _currentFigure) {
                QRect oldRect = _currentFigure->boundingRect();
                _currentFigure->updateShape(event->pos());
                QRect newRect = _currentFigure->boundingRect();
                update(QRegion(oldRect.united(newRect)));

        } else if (_isMoving && _movingFigure) {
                QRect oldRect = _movingFigure->boundingRect().adjusted(-5,
                                                                       -5,
                                                                       5,
                                                                       5);
                QPoint offset = event->pos() - _moveStartPos;
                _movingFigure->move(offset);
                QRect newRect = _movingFigure->boundingRect().adjusted(-5,
                                                                       -5,
                                                                       5,
                                                                       5);
                _moveStartPos = event->pos();

                QRegion updateRegion = QRegion(oldRect.united(newRect));

                for (const auto& connection : std::as_const(_connections)) {
                        if (connection.first == _movingFigure
                            || connection.second == _movingFigure) {
                                QRect connectionRect
                                    = QRect(connection.first->boundingRect()
                                                .center(),
                                            connection.second->boundingRect()
                                                .center())
                                          .normalized();
                                connectionRect = connectionRect.adjusted(-5,
                                                                         -5,
                                                                         5,
                                                                         5);
                                updateRegion = updateRegion.united(
                                    QRegion(connectionRect));
                        }
                }

                update(updateRegion);

        } else if (_isConnecting && _startConnectionFigure) {
                _connectionCursor = event->pos();
                QRect startFigureRect = _startConnectionFigure->boundingRect();
                QRect lineRect = QRect(startFigureRect.center(),
                                       _connectionCursor)
                                     .normalized();
                QRect cursorRect = QRect(_connectionCursor - QPoint(5, 5),
                                         QSize(10, 10));
                QRect updateRect = startFigureRect.united(lineRect).united(
                    cursorRect);
                update(QRegion(updateRect));
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
        if (_isDrawing && event->button() == Qt::LeftButton) {
                _figures.append(_currentFigure);

                QRect figureRect = _currentFigure->boundingRect();

                _currentFigure = nullptr;
                _isDrawing = false;

                update(QRegion(figureRect));
        } else if (_isMoving && event->button() == Qt::LeftButton) {
                QRect oldRect = _movingFigure->boundingRect();

                _isMoving = false;
                _movingFigure = nullptr;
                setCursor(Qt::ArrowCursor);

                update(QRegion(oldRect));
        }
}

void MainWindow::paintEvent(QPaintEvent* event) {
        QPainter painter(this);

        if (!_backgroundPixmap.isNull()) {
                painter.drawPixmap(0, 0, _backgroundPixmap);
        } else {
                painter.fillRect(rect(), Qt::white);
        }

        for (const auto& figure : std::as_const(_figures)) {
                figure->draw(painter);
        }

        painter.setPen(QPen(Qt::black, 2));
        for (const auto& connection : std::as_const(_connections)) {
                Shapes::IFigure* start = connection.first;
                Shapes::IFigure* end = connection.second;
                if (start && end) {
                        painter.drawLine(start->getCenter(), end->getCenter());
                }
        }

        if (_isDrawing && _currentFigure) {
                _currentFigure->draw(painter);
        }

        if (_isConnecting && _startConnectionFigure) {
                QPoint startPoint = _startConnectionFigure->getCenter();
                painter.setPen(QPen(Qt::black, 2));
                painter.drawLine(startPoint, _connectionCursor);
        }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape) {
                if (_isDrawing) {
                        _currentFigure = nullptr;
                        _isDrawing = false;
                        update();
                } else if (_isMoving) {
                        _isMoving = false;
                        _movingFigure = nullptr;
                        setCursor(Qt::ArrowCursor);
                        update();
                } else if (_isConnecting) {
                        _startConnectionFigure = nullptr;
                        _isConnecting = false;
                        update();
                }
        }

        QMainWindow::keyPressEvent(event);
}
