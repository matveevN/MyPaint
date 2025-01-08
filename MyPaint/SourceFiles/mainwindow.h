#pragma once

class ShapeEditorWidget;

namespace Shapes {
class IShapes;
}

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
        Q_OBJECT

public:
        MainWindow();
        ~MainWindow();

private slots:
        void onRectangleButtonClicked();
        void onEllipseButtonClicked();
        void onTriangleButtonClicked();
        void onMoveButtonClicked();
        void onDeleteButtonClicked();
        void onSaveButtonClicked();
        void onLoadButtonClicked();
        void onConnectButtonClicked();

private:
        ShapeEditorWidget *_shapeEditorWidget;
};
