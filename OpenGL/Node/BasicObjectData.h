#ifndef BASICOBJECTDATA_H
#define BASICOBJECTDATA_H

#include "BasicObject.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

class BasicObjectData : protected QOpenGLFunctions
{
public:
    BasicObjectData(BasicObject::Type type);

    BasicObject::Type type();

    void create();
    void bind();
    void release();
    int getVertexCount();

protected:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
    BasicObject::Type mType;
};

#endif // BASICOBJECTDATA_H
