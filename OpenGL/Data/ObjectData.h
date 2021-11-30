#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

class ObjectData
{
public:
    ObjectData();
    virtual ~ObjectData();

    virtual bool create() = 0;
    virtual void update() = 0;
    virtual void bind();
    virtual void release();

    virtual QVector<QVector3D> vertices() const;
    virtual void setVertices(const QVector<QVector3D> newVertices);

    virtual QVector<QVector3D> normals() const;
    virtual void setNormals(const QVector<QVector3D> newNormals);

    virtual int getVertexCount();

protected:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
};

#endif // OBJECTDATA_H
