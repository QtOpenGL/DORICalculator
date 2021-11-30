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
    bool loadDataFromFile(QString filename);

    const QVector<QVector2D> &UVs() const { return mUVs; }
    const QVector<QVector3D> &normals() const { return mNormals; }
    const QVector<QVector3D> &vertices() const { return mVertices; }

    void create();
    void release() { mVertexArray.release(); }
    void bind() { mVertexArray.bind(); }

    int getVertexCount() { return mVertices.size(); }
    const QString &name() const { return mName; }

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
