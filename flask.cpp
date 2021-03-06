#include "flask.h"

Particles::Particles()
    : diameter(-1)
{}

Particles::Particles(QBrush brush, QPen pen, float diameter)
    : brush(brush),
      pen(pen),
      diameter(diameter)
{}

void Particles::draw(QPainter &painter)
{
    painter.setBrush(this->brush);
    painter.setPen(this->pen);
    if (this->diameter == -1){
        return;
    } else if (this->diameter != 0){
        for(auto& particle : this->particlesPositions){
            painter.drawEllipse(QRectF(particle.rx(), particle.ry(), this->diameter, this->diameter));
        }
    } else{
        painter.drawPoints(this->particlesPositions);
    }
}

void Particles::clearParticlesPositions()
{
    this->particlesPositions.clear();
}

void Particles::appendParticlesPositions(const QPointF& positionToAppend)
{
    this->particlesPositions.append(positionToAppend);
}


ParticlesStats::ParticlesStats(QMap<ParticleType, Particles> particlesAtBeginning)
{
    for(const auto& keyAndValue : particlesAtBeginning.toStdMap()){
        this->allParticlesSizeAtBeginning.insert(keyAndValue.first, keyAndValue.second.particlesPositions.size());
    }
}

QMap<ParticleType, int> ParticlesStats::getPercentageOfSeparated(const QMap<ParticleType, int>& allParticlesSizeCurrent)
{
    QMap<ParticleType, int> percentageOfSeparated;
    for(const auto& keyAndValue : this->allParticlesSizeAtBeginning.toStdMap()){
        percentageOfSeparated.insert(keyAndValue.first,
                                     this->getPercentageOfSeparated(keyAndValue.first, allParticlesSizeCurrent[keyAndValue.first]));
    }
    return percentageOfSeparated;
}


int ParticlesStats::getPercentageOfSeparated(const ParticleType& particleType, const int& particleCurrentAmount)
{
    return 100 - (particleCurrentAmount * 100 / this->allParticlesSizeAtBeginning[particleType]);
}


GroupOfWall::GroupOfWall(QVector<QPointF> edgePositions, QBrush brush, QPen pen)
    : edgePositions(edgePositions),
      brush(brush),
      pen(pen)
{}

void GroupOfWall::draw(QPainter& painter)
{
    painter.setPen(this->pen);
    painter.setBrush(this->brush);
    painter.drawPolygon(this->edgePositions);
}

ConfigReader::ConfigReader(QString pathToConfig)
{
    QFile fileObject(pathToConfig);
    if (!fileObject.open(QIODevice::ReadOnly)) {
        QMessageBox(QMessageBox::Information, "Warning!!!", "Can't find config file:\n" + pathToConfig).exec();
        exit(1);
    }

    QTextStream fileText(&fileObject);
    QString jsonString = fileText.readAll();
    fileObject.close();

    auto jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit());
    this->configData = jsonDocument.object().toVariantMap();

    if (jsonDocument.isNull() or !jsonDocument.isObject()) {
        QMessageBox(QMessageBox::Information, "Warning!!!", "Failed to create config from JSON\n").exec();
        exit(1);
    }
}

Flask::Flask()
{   
    QVector<QPointF> outerWallsPoints{{0.125, 0.185}, {0.21, 0.73}, {0.3,0.81}, {0.7, 0.81}, {0.79, 0.73}, {0.875, 0.185},};
    QPen outerWallsPen(QColor(89, 128, 166, 255));
    outerWallsPen.setWidthF(0.017);
    QBrush outerWallsBrush(Qt::BrushStyle::NoBrush);

    QVector<QPointF> maskWallsPoints{{0.125, 0.185}, {0.21, 0.73}, {0.3,0.81}, {0.7, 0.81}, {0.79, 0.73}, {0.875, 0.185},
                                     {0.875, 0.9}, {0.125, 0.9}, {0.125, 0.185}, {0.125, 0}, {0.875, 0}, {0.875, 0.185},};
    QPen maskWallsPen(Qt::white);
    maskWallsPen.setWidthF(0.02);
    QBrush maskWallsBrush(Qt::BrushStyle::SolidPattern);
    maskWallsBrush.setColor(Qt::white);

    QVector<QPointF> maskYellowWallsPoints{{0.51, 0.30}, {0.51, 0.8}, {0.49, 0.8}, {0.49, 0.30}};
    QPen maskYellowWallsPen(QColor(255, 255, 204, 255));
    maskYellowWallsPen.setWidthF(0.006);
    QBrush maskYellowWallsBrush(Qt::BrushStyle::SolidPattern);
    maskYellowWallsBrush.setColor(QColor(255, 255, 204, 255));

    QVector<QPointF> innerWallsPoints{{0.31,0.69}, {0.69, 0.69}, {0.715, 0.66}, {0.77, 0.305},
                                      {0.52, 0.305}, {0.52, 0.81}, {0.48, 0.81}, {0.48, 0.305},
                                      {0.23, 0.305}, {0.278, 0.66},};
    QPen innerWallsPen(QColor(89, 128, 166, 255));
    innerWallsPen.setWidthF(0.017);
    QBrush innerWallsBrush(Qt::BrushStyle::SolidPattern);
    innerWallsBrush.setColor(Qt::white);

    this->allWalls.insert("mask", GroupOfWall(maskWallsPoints, maskWallsBrush, maskWallsPen));
    this->allWalls.insert("maskYellow", GroupOfWall(maskYellowWallsPoints, maskYellowWallsBrush, maskYellowWallsPen));
    this->allWalls.insert("outerWall", GroupOfWall(outerWallsPoints, outerWallsBrush, outerWallsPen));
    this->allWalls.insert("innerWall",  GroupOfWall(innerWallsPoints, innerWallsBrush, innerWallsPen));

    const QBrush plasmaBrush(QColor(255, 255, 0, 255), Qt::BrushStyle::SolidPattern);
    const QPen plasmaPen(plasmaBrush, 0);
    const float plasmaDiametr = 0;

    const QBrush plateletsBrush(QColor(0, 0, 0, 255), Qt::BrushStyle::SolidPattern);
    const QPen plateletsPen(plateletsBrush, 0);
    const float plateletsDiametr = 0.006;

    const QBrush erythrocytesBrush(QColor(255, 0, 0, 255), Qt::BrushStyle::SolidPattern);
    const QPen erythrocytesPen(erythrocytesBrush, 0);
    const float erythrocytesDiametr = 0.006;

    this->allParticles.insert(ParticleType::plasma, Particles(plasmaBrush, plasmaPen, plasmaDiametr));
    this->allParticles.insert(ParticleType::platelets, Particles(plateletsBrush, plateletsPen, plateletsDiametr));
    this->allParticles.insert(ParticleType::erythrocytes, Particles(erythrocytesBrush, erythrocytesPen, erythrocytesDiametr));

    this->configReader = QSharedPointer<ConfigReader>(new ConfigReader("config.json"));
    const auto dumpConfig = this->configReader->configData["dumpConfig"].toMap();

    this->dumpStep = dumpConfig["dumpStep"].toInt();
    this->dumpPath = dumpConfig["dumpPath"].toString();
    this->dumpFileName = dumpConfig["dumpFileName"].toString();


    const int startDumpStep = 0;
    if (not this->updateAllParticlesFromDumpByStep(startDumpStep)){
        const QString dumpFullPath = this->dumpPath + this->dumpFileName + QString::number(this->dumpStep) + ".atom";
        QMessageBox(QMessageBox::Information, "Warning!!!", "Can't find file:\n" + dumpFullPath).exec();
    }

    this->particlesStats = QSharedPointer<ParticlesStats>(new ParticlesStats(this->allParticles));
}

void Flask::draw(QPainter &painter)
{
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setWindow(0, -1, 1, 1);

    QTransform transform;
    transform.translate(-0.15, 0.15);
    transform.scale(1, -1);
    transform.scale(1.3, 1.3);
    painter.setWorldTransform(transform);

    for(auto& paticles : this->allParticles){
        paticles.draw(painter);
    }

    this->allWalls["mask"].draw(painter);
    this->allWalls["innerWall"].draw(painter);
    this->allWalls["maskYellow"].draw(painter);
    this->allWalls["outerWall"].draw(painter);

    QMap<ParticleType, int> allParticlesCount;
    for(const auto& keyAndValue : this->allParticles.toStdMap()){
         allParticlesCount[keyAndValue.first] = keyAndValue.second.particlesPositions.size();
    }


    painter.resetTransform();

    const auto percentageOfSeparated = this->particlesStats->getPercentageOfSeparated(allParticlesCount);
    const QString textPlasma = "plasma: " + QString::number(percentageOfSeparated[ParticleType::plasma])+ "%";
    const QString textPlatelets =  + "platelets: " + QString::number(percentageOfSeparated[ParticleType::platelets]) + "%";
    const QString textErythrocytes = "erythrocytes: " + QString::number(percentageOfSeparated[ParticleType::erythrocytes]) + "%";

    painter.drawText(10, 20, textPlatelets);
    painter.drawText(10, 35, textErythrocytes);
}


bool Flask::updateAllParticlesFromDumpByStep(const int& dumpStep)
{
    const QString dumpFullPath = this->dumpPath + this->dumpFileName + QString::number(dumpStep) + ".atom";
    QFile file(dumpFullPath);
    if (!file.open(QFile::ReadOnly)){
        return false;
    }
    QTextStream in(&file);

    int lineToSkip = 9;
    while(!in.atEnd() and lineToSkip > 0){
        in.readLine();
        lineToSkip--;
    }

    for(auto& particles : this->allParticles){
        particles.clearParticlesPositions();
    }

    while(!in.atEnd()){
        const QString string = in.readLine();
        const auto elements = string.split(" ");

        const ParticleType id = static_cast<ParticleType>(elements[1].toInt());
        const QPointF position(elements[2].toFloat(), elements[3].toFloat());
        if(id == ParticleType::wall){
            continue;
        }
        this->allParticles[id].appendParticlesPositions(position);
    }

    file.close();
    return true;
}



