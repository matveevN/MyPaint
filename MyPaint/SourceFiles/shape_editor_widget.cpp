#include "shape_editor_widget.h"
#include "IShape.h"

ShapeEditorWidget::ShapeEditorWidget() {
}

void ShapeEditorWidget::mousePressEvent(QMouseEvent* event) {
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
                        Shapes::IShapes* clickedFigure = nullptr;

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

                                        std::swap(_figures[i], _figures.back());

                                        delete _figures.back();
                                        _figures.pop_back();

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

void ShapeEditorWidget::mouseMoveEvent(QMouseEvent* event) {
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
                                         QSize(5, 5));
                QRect updateRect = startFigureRect.united(lineRect).united(
                    cursorRect);
                update(QRegion(updateRect));
        }
}

void ShapeEditorWidget::mouseReleaseEvent(QMouseEvent* event) {
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

void ShapeEditorWidget::keyPressEvent(QKeyEvent* event) {
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

        ShapeEditorWidget::keyPressEvent(event);
}

void ShapeEditorWidget::paintEvent(QPaintEvent* event) {
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
                Shapes::IShapes* start = connection.first;
                Shapes::IShapes* end = connection.second;
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

QVector<Shapes::IShapes*>& ShapeEditorWidget::getFigures() {
        return _figures;
}

void ShapeEditorWidget::setCurrentFigure(Shapes::IShapes* figure) {
        _currentFigure = figure;
}

void ShapeEditorWidget::setIsDrawing(bool isDrawing) {
        _isDrawing = isDrawing;
}

void ShapeEditorWidget::setIsDeleting(bool isDeleting) {
        _isDeleting = isDeleting;
}

QVector<QPair<Shapes::IShapes*, Shapes::IShapes*>>&
ShapeEditorWidget::getConnections() {
        return _connections;
}

void ShapeEditorWidget::setIsConnecting(bool isConnecting) {
        _isConnecting = isConnecting;
}

void ShapeEditorWidget::setStartConnectionFigure(Shapes::IShapes* figure) {
        _startConnectionFigure = figure;
}

void ShapeEditorWidget::setIsMoving(bool isMoving) {
        _isMoving = isMoving;

        if (_isMoving) {
                QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        } else {
                QApplication::restoreOverrideCursor();
        }
}

bool ShapeEditorWidget::getIsMoving() const {
        return _isMoving;
}
