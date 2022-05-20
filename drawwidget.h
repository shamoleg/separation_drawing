#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QScopedPointer>

#include <QPainter>
#include <QTimer>

#include "flask.h"


class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event) override;

    QScopedPointer<Flask>  flask;
    QScopedPointer<QTimer> drawTimer;

    int stepCurrent;

public slots:
    void increasStepCurrentAndUpdate();

    void startDrawTimer();
    void stopDrawTimer();

};


#endif // DRAWWIDGET_H
