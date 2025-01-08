#include "mainwindow.h"
#include "ellipse.h"
#include "file_manager.h"
#include "rectangle.h"
#include "shape_editor_widget.h"
#include "triangle.h"

MainWindow::MainWindow()
: _shapeEditorWidget(new ShapeEditorWidget()) {
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

        setCentralWidget(_shapeEditorWidget);
        _shapeEditorWidget->setMinimumSize(800, 600);

        QVBoxLayout* mainLayout = new QVBoxLayout();
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* rectangleButton = new QPushButton("Прямоугольник", this);
        rectangleButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(rectangleButton);
        connect(rectangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onRectangleButtonClicked);

        QPushButton* ellipseButton = new QPushButton("Эллипс", this);
        ellipseButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(ellipseButton);
        connect(ellipseButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onEllipseButtonClicked);

        QPushButton* triangleButton = new QPushButton("Треугольник", this);
        triangleButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(triangleButton);
        connect(triangleButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onTriangleButtonClicked);

        QPushButton* connectButton = new QPushButton("Связь", this);
        connectButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(connectButton);
        connect(connectButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onConnectButtonClicked);

        QPushButton* moveButton = new QPushButton("Переместить", this);
        moveButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(moveButton);
        connect(moveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onMoveButtonClicked);

        QPushButton* deleteButton = new QPushButton("Удалить", this);
        deleteButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(deleteButton);
        connect(deleteButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onDeleteButtonClicked);

        QPushButton* loadButton = new QPushButton("Загрузить", this);
        loadButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(loadButton);
        connect(loadButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onLoadButtonClicked);

        QPushButton* saveButton = new QPushButton("Сохранить", this);
        saveButton->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(saveButton);
        connect(saveButton,
                &QPushButton::clicked,
                this,
                &MainWindow::onSaveButtonClicked);

        mainLayout->addLayout(buttonLayout);
        mainLayout->addStretch();
        _shapeEditorWidget->setLayout(mainLayout);
}

MainWindow::~MainWindow() {
        delete _shapeEditorWidget;
}

void MainWindow::onRectangleButtonClicked() {
        _shapeEditorWidget->setCurrentFigure(
            new Shapes::Rectangle(QPoint(), QPoint()));

        _shapeEditorWidget->setIsDrawing(true);
}

void MainWindow::onEllipseButtonClicked() {
        _shapeEditorWidget->setCurrentFigure(
            new Shapes::Ellipse(QPoint(), 0, 0));
        _shapeEditorWidget->setIsDrawing(true);
}

void MainWindow::onTriangleButtonClicked() {
        _shapeEditorWidget->setCurrentFigure(new Shapes::Triangle(QPoint(), 0));

        _shapeEditorWidget->setIsDrawing(true);
}

void MainWindow::onMoveButtonClicked() {
        _shapeEditorWidget->setIsMoving(!_shapeEditorWidget->getIsMoving());
}

void MainWindow::onDeleteButtonClicked() {
        _shapeEditorWidget->setIsDeleting(true);
}

void MainWindow::onConnectButtonClicked() {
        _shapeEditorWidget->setIsConnecting(true);
        _shapeEditorWidget->setStartConnectionFigure(nullptr);
}

void MainWindow::onSaveButtonClicked() {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Сохранить рисунок"),
                                                        "",
                                                        tr(" Files (*.png)"));
        if (fileName.isEmpty())
                return;

        Utils::FileManager::saveToImageWithMetadata(
            _shapeEditorWidget->getFigures(),
            _shapeEditorWidget->getConnections(),
            fileName,
            width(),
            height());
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

        Utils::FileManager::loadFromImageWithMetadata(
            _shapeEditorWidget->getFigures(),
            _shapeEditorWidget->getConnections(),
            fileName);

        for (const auto& figure :
             std::as_const(_shapeEditorWidget->getFigures())) {
                updateRect = updateRect.united(figure->boundingRect());
        }

        for (const auto& connection :
             std::as_const(_shapeEditorWidget->getConnections())) {
                updateRect = updateRect.united(
                    QRect(connection.first->boundingRect().center(),
                          connection.second->boundingRect().center()));
        }

        update(updateRect);
}
