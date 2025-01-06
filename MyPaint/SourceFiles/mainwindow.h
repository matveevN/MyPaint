#pragma once

#include <QMainWindow>

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

protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

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
        QVector<Shapes::IShapes *> _figures;

        QVector<QPair<Shapes::IShapes *, Shapes::IShapes *>> _connections;

        Shapes::IShapes *_movingFigure = nullptr;
        Shapes::IShapes *_selectedFigure = nullptr;
        Shapes::IShapes *_currentFigure = nullptr;
        Shapes::IShapes *_startConnectionFigure = nullptr;

        QPoint _startPoint;
        QPoint _moveStartPos;
        QPoint _connectionCursor;

        QPixmap _backgroundPixmap;

        bool _isDrawing = false;
        bool _isMoving = false;
        bool _isMovingMode = false;
        bool _isDeleting = false;
        bool _isConnecting = false;
};
