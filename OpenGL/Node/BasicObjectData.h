#ifndef BASICOBJECTDATA_H
#define BASICOBJECTDATA_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

class BasicObjectData : protected QOpenGLFunctions
{
public:
    BasicObjectData();

    virtual void create();
    virtual void bind();
    virtual void release();
    virtual int getVertexCount();

    virtual void createVertexData() = 0;

protected:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
};

#endif // BASICOBJECTDATA_H
