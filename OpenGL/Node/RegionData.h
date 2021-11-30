#ifndef REGIONDATA_H
#define REGIONDATA_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

class RegionData : protected QOpenGLFunctions
{
public:
    RegionData(const QVector<QVector3D> &vertices, const QVector<QVector3D> &normals);

    void create();
    void update();
    void bind() { mVertexArray.bind(); };
    void release() { mVertexArray.release(); };
    int getVertexCount() { return mVertices.count(); };

    const QVector3D &color() const { return mColor; };
    void setColor(const QVector3D &newColor) { mColor = newColor; };

    QVector<QVector3D> vertices() const { return mVertices; };
    void setVertices(const QVector<QVector3D> newVertices) { mVertices = newVertices; };

    QVector<QVector3D> normals() const { return mNormals; };
    void setNormals(const QVector<QVector3D> newNormals) { mNormals = newNormals; };

protected:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
    QVector3D mColor;
};

#endif // REGIONDATA_H
