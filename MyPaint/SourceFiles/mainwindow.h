#pragma once

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

namespace Shapes {
class IFigure;

}

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);
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
        QVector<Shapes::IFigure *> _figures;

        QVector<QPair<Shapes::IFigure *, Shapes::IFigure *>> _connections;

        Shapes::IFigure *_movingFigure = nullptr;
        Shapes::IFigure *_selectedFigure = nullptr;
        Shapes::IFigure *_currentFigure = nullptr;
        Shapes::IFigure *_startConnectionFigure = nullptr;

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
