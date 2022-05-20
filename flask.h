#ifndef FLASK_H
#define FLASK_H

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QPointF>
#include <QVector>
#include <QVariant>
#include <QPainter>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSharedPointer>
#include <QScopedPointer>


class DrawObject
{
public:
    virtual void draw(QPainter& painter) = 0;
};


enum ParticleType
{
    wall = 1,
    plasma = 2,
    platelets = 3,
    erythrocytes = 4,
};


class Particles : DrawObject
{
public:
    Particles();
    Particles(QBrush brush, QPen pen, float diameter);

    void draw(QPainter& painter) override;

    void clearParticlesPositions();
    void appendParticlesPositions(const QPointF& positionToAppend);
    void updateParticlesPosition(QSharedPointer<QVector<QPointF>> newParticlesPositions);

    QVector<QPointF> particlesPositions;
private:

    QBrush brush;
    QPen pen;
    float diameter;
};


class ParticlesStats
{
public:
    ParticlesStats(QMap<ParticleType, Particles> particlesAtBeginning);

    QMap<ParticleType, int> getPercentageOfSeparated(const QMap<ParticleType, int>& allParticlesSizeCurrent);
    int getPercentageOfSeparated(const ParticleType& particleType, const int& particleCurrentAmount);
    QMap<ParticleType, int> allParticlesSizeAtBeginning;
};


class GroupOfWall : DrawObject
{
public:
    GroupOfWall(){};
    GroupOfWall(QVector<QPointF> edgePositions, QBrush brush, QPen pen);

    void draw(QPainter& painter) override;

private:
    QVector<QPointF> edgePositions;
    QBrush brush;
    QPen pen;
};

class ConfigReader
{
public:
    ConfigReader(QString pathToConfig);

    QVariantMap configData;
};

class Flask
{
public:
    Flask();

    void draw(QPainter& painter);

    bool updateAllParticlesFromDumpByStep(const int& dumpStep);

    int dumpStep;
    QString dumpPath;
    QString dumpFileName;

private:
    QMap<QString, GroupOfWall> allWalls;
    QMap<ParticleType, Particles> allParticles;

    QSharedPointer<ParticlesStats> particlesStats;
    QSharedPointer<ConfigReader> configReader;
};

#endif // FLASK_H
