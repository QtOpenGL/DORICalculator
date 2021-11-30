#ifndef MODELDATA_H
#define MODELDATA_H

#include "ObjectData.h"
#include <OpenGL/Object/Model.h>

#include <QFile>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

class ModelData : public ObjectData, protected QOpenGLFunctions
{
public:
    ModelData(Model::Type type);
    ~ModelData();

    bool create() override;
    void update() override;
    const QVector<QVector2D> &UVs() const;

private:
    QFile getModelFile();
    bool loadData();

private:
    Model::Type mType;
    QVector<QVector2D> mUVs;
};

#endif // MODELDATA_H
