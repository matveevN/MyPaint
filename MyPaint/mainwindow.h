#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include "IFigure.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void saveFiguresToJson(const QString &fileName);
        void loadFiguresFromJson(const QString &fileName);

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
        Ui::MainWindow *ui;
        QList<IFigure *> figures;
        QVector<QPair<IFigure *, IFigure *>> connections;

        IFigure *movingFigure = nullptr;
        IFigure *selectedFigure = nullptr;
        IFigure *currentFigure = nullptr;
        IFigure *startConnectionFigure = nullptr;

        QPoint startPoint;
        QPoint moveStartPos;
        QPoint connectionCursor;

        QPixmap backgroundPixmap;

        bool isDrawing = false;
        bool isMoving = false;
        bool isMovingMode = false;
        bool isDeleting = false;
        bool isConnecting = false;
};

#endif // MAINWINDOW_H
