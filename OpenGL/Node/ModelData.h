#ifndef MODELDATA_H
#define MODELDATA_H

#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

class ModelData : protected QOpenGLFunctions
{
public:
    ModelData(QString name);
    void create();
    void bind();
    void release();
    int getVertexCount();
    bool loadDataFromFile(QString filename);

    const QVector<QVector3D> &vertices() const;
    const QVector<QVector3D> &normals() const;
    const QVector<QVector2D> &UVs() const;
    const QString &name() const;

private:
    QString mName;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
    QVector<QVector2D> mUVs;

    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
};

#endif // MODELDATA_H
