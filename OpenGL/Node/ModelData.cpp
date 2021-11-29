#include "ModelData.h"

#include <QFile>
#include <QTextStream>

ModelData::ModelData(QString name)
    : mName(name)
{}

void ModelData::create()
{
    initializeOpenGLFunctions();

    mVertexArray.create();
    mVertexArray.bind();

    // Vertices
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVertexBuffer.allocate(mVertices.constData(), sizeof(QVector3D) * mVertices.size());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized?
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    mVertexBuffer.release();

    // Normals
    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mNormalBuffer.allocate(mNormals.constData(), sizeof(QVector3D) * mNormals.size());

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized?
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );

    mNormalBuffer.release();

    mVertexArray.release();
}

void ModelData::release()
{
    mVertexArray.release();
}

void ModelData::bind()
{
    mVertexArray.bind();
}

int ModelData::getVertexCount()
{
    return mVertices.size();
}

const QString &ModelData::name() const
{
    return mName;
}

bool ModelData::loadDataFromFile(QString filename)
{
    QVector<QVector3D> tempVertices;
    QVector<QVector3D> tempNormals;
    QVector<QVector2D> tempUVs;

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream fileText(&file);
            while (!fileText.atEnd()) {
                QString fileLine = fileText.readLine();
                if (fileLine.startsWith("vn ")) {
                    QStringList lineList = fileLine.split(" ");
                    tempNormals << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
                } else if (fileLine.startsWith("vt ")) {
                    QStringList lineList = fileLine.split(" ");
                    tempUVs << QVector2D(lineList[1].toFloat(), lineList[2].toFloat());
                } else if (fileLine.startsWith("v ")) {
                    QStringList lineList = fileLine.split(" ");
                    tempVertices << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
                } else if (fileLine.startsWith("f ")) {
                    QStringList lineList = fileLine.split(" ");

                    for (int i = 1; i <= 3; i++) {
                        QStringList arg = lineList[i].split("/");
                        mVertices << tempVertices[arg[0].toInt() - 1];
                        mUVs << tempUVs[arg[1].toInt() - 1];
                        mNormals << tempNormals[arg[2].toInt() - 1];
                    }

                } else if (fileLine.startsWith("mtllib ")) {
                }
            }
        } else {
            qWarning() << QString("Could not open file '%1'.").arg(filename);
            file.close();
            return false;
        }
        file.close();
    } else {
        qWarning() << "Filename is empty.";
        return false;
    }

    return true;
}

const QVector<QVector2D> &ModelData::UVs() const
{
    return mUVs;
}

const QVector<QVector3D> &ModelData::normals() const
{
    return mNormals;
}

const QVector<QVector3D> &ModelData::vertices() const
{
    return mVertices;
}
