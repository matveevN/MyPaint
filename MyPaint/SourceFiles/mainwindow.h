#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
//#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include "./shapes/IFigure.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
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
        Ui::MainWindow *_ui;
        QVector<IFigure *> _figures;
        //QList<IFigure *> figures;
        QVector<QPair<IFigure *, IFigure *>> _connections;

        IFigure *_movingFigure = nullptr;
        IFigure *_selectedFigure = nullptr;
        IFigure *_currentFigure = nullptr;
        IFigure *_startConnectionFigure = nullptr;

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

#endif // MAINWINDOW_H
