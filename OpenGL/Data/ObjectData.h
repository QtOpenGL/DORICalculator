#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include <OpenGL/Object/Object.h>

#include <QFile>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

class ObjectData : protected QOpenGLFunctions
{
public:
    ObjectData(Object::Type type);

    bool create();
    void bind();
    void release();

    int getVertexCount();

private:
    QFile getModelFile();
    bool loadData();

private:
    Object::Type mType;
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
    QVector<QVector2D> mUVs;
};

#endif // OBJECTDATA_H
