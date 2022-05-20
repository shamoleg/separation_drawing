#include "drawwidget.h"
#include <QMessageBox>

DrawWidget::DrawWidget(QWidget *parent)
    : QWidget(parent),
      flask(new Flask),
      drawTimer(new QTimer(this))
{
    this->stepCurrent = 0 + this->flask->dumpStep*3;
    connect(this->drawTimer.data(), SIGNAL(timeout()), this, SLOT(increasStepCurrentAndUpdate()));
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (this->flask->updateAllParticlesFromDumpByStep(this->stepCurrent)){
        this->flask->draw(painter);
    } else{
        this->flask->updateAllParticlesFromDumpByStep(this->stepCurrent - this->flask->dumpStep);
        this->flask->draw(painter);
        this->stopDrawTimer();
    }
}

void DrawWidget::increasStepCurrentAndUpdate()
{
   this->stepCurrent += this->flask.data()->dumpStep;
   this->update();
}

void DrawWidget::stopDrawTimer()
{
    this->stepCurrent = 0 + this->flask->dumpStep*3;
    this->drawTimer->stop();
}

void DrawWidget::startDrawTimer()
{
    this->drawTimer->start(20);
}

