#include "mainwindow.h"
#include "./shapes/ellipse.h"
#include "./shapes/rectangle.h"
#include "./shapes/triangle.h"
#include "./utils/file_manager.h"

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
        _currentFigure = new Rectangle(QPoint(), QPoint());
        _isDrawing = true;
}

void MainWindow::onEllipseButtonClicked() {
        _currentFigure = new Ellipse(QPoint(), 0, 0);
        _isDrawing = true;
}

void MainWindow::onTriangleButtonClicked() {
        _currentFigure = new Triangle(QPoint(), 0);
        _isDrawing = true;
}

void MainWindow::onMoveButtonClicked() {
        _isMoving = !_isMoving;
        if (_isMoving) {
                setCursor(Qt::ClosedHandCursor);
        } else {
                setCursor(Qt::ArrowCursor);
        }
        if (!_isMoving) {
                _movingFigure = nullptr;
        }
}

void MainWindow::onDeleteButtonClicked() {
        _isDeleting = true;
}

void MainWindow::onConnectButtonClicked() {
        _isConnecting = true;
        _startConnectionFigure = nullptr;
        update();
}

void MainWindow::onSaveButtonClicked() {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Сохранить рисунок"),
                                                        "",
                                                        tr(" Files (*.png)"));
        if (fileName.isEmpty())
                return;

        FileManager::saveToImageWithMetadata(_figures, _connections, fileName);
}

void MainWindow::onLoadButtonClicked() {
        QString fileName
            = QFileDialog::getOpenFileName(this,
                                           tr("Загрузить рисунок"),
                                           "",
                                           tr("PNG Files (*.png)"));
        if (fileName.isEmpty())
                return;

        FileManager::loadFromImageWithMetadata(_figures, _connections, fileName);
        update();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
                if (_isDeleting) {
                        for (int i = 0; i < _figures.size(); ++i) {
                                if (_figures[i]->contains(event->pos())) {
                                        for (int j = 0; j < _connections.size();
                                             ++j) {
                                                if (_connections[j].first
                                                        == _figures[i]
                                                    || _connections[j].second
                                                           == _figures[i]) {
                                                        _connections.removeAt(j);
                                                        --j;
                                                }
                                        }

                                        delete _figures[i];
                                        _figures.removeAt(i);
                                        _isDeleting = false;
                                        update();
                                        break;
                                }
                        }
                } else if (_isDrawing) {
                        _startPoint = event->pos();

                        if (_currentFigure) {
                                _currentFigure->initialize(_startPoint);
                        }
                } else if (_isConnecting) {
                        IFigure* clickedFigure = nullptr;

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
                                _startConnectionFigure = nullptr;
                                _isConnecting = false;
                        } else {
                                _startConnectionFigure = nullptr;
                                _isConnecting = false;
                        }

                        update();
                } else if (_isMoving) {
                        _movingFigure = nullptr;
                        for (auto& figure : _figures) {
                                if (figure->contains(event->pos())) {
                                        _movingFigure = figure;
                                        _moveStartPos = event->pos();
                                        break;
                                }
                        }
                }
        } else if (event->button() == Qt::RightButton) {
                if (_isMoving) {
                        _isMoving = false;
                        _movingFigure = nullptr;
                        setCursor(Qt::ArrowCursor);
                        update();
                } else if (_isDrawing) {
                        _currentFigure = nullptr;
                        _isDrawing = false;
                        update();
                } else if (_isConnecting) {
                        _startConnectionFigure = nullptr;
                        _isConnecting = false;
                        update();
                }
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
        if (_isDrawing) {
                if (_currentFigure) {
                        _currentFigure->updateShape(event->pos());
                }
                update();
        } else if (_isConnecting && _startConnectionFigure) {
                _connectionCursor = event->pos();
                update();
        } else if (_isMoving && _movingFigure) {
                QPoint offset = event->pos() - _moveStartPos;
                _movingFigure->move(offset);
                _moveStartPos = event->pos();
                update();
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
        if (_isDrawing && event->button() == Qt::LeftButton) {
                _figures.append(_currentFigure);
                _currentFigure = nullptr;
                _isDrawing = false;
                update();
        } else if (_isMoving && event->button() == Qt::LeftButton) {
                _isMoving = false;
                _movingFigure = nullptr;
                setCursor(Qt::ArrowCursor);
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
                IFigure* start = connection.first;
                IFigure* end = connection.second;
                if (start && end) {
                        painter.drawLine(start->getCenter(), end->getCenter());
                }
        }

        if (_isConnecting && _startConnectionFigure) {
                QPoint startPoint = _startConnectionFigure->getCenter();
                painter.setPen(QPen(Qt::black, 2));
                painter.drawLine(startPoint, _connectionCursor);
        }

        if (_isDrawing && _currentFigure) {
                _currentFigure->draw(painter);
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
