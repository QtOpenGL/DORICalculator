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
    RegionData();

    bool create();
    void update();
    void bind();
    void release();
    int getVertexCount();

    const QVector<QVector3D> &vertices() const;
    void setVertices(const QVector<QVector3D> &newVertices);

    bool intersectsGround() const;
    void setIntersectsGround(bool newIntersectsGround);

private:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QVector<QVector3D> mVertices;
    bool mIntersectsGround;
};

#endif // REGIONDATA_H
