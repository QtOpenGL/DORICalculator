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

    const QVector3D &color() const;
    void setColor(const QVector3D &newColor);

    bool visible() const;
    void setVisible(bool newVisible);

private:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QVector<QVector3D> mVertices;
    QVector3D mColor;
    bool mVisible;
};

#endif // REGIONDATA_H
