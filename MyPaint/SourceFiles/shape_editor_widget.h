#pragma once

namespace Shapes {
class IShapes;

}

class ShapeEditorWidget final : public QWidget {
        Q_OBJECT

public:
        ShapeEditorWidget();
        [[nodiscard]] QVector<Shapes::IShapes *> &getFigures();
        void setCurrentFigure(Shapes::IShapes *figure);
        void setIsDrawing(bool isDrawing);
        void setIsDeleting(bool isDeleting);
        [[nodiscard]] QVector<QPair<Shapes::IShapes *, Shapes::IShapes *>> &
        getConnections();
        void setIsConnecting(bool isConnecting);

        void setStartConnectionFigure(Shapes::IShapes *figure);
        void setIsMoving(bool isMoving);
        [[nodiscard]] bool getIsMoving() const;

protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

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
