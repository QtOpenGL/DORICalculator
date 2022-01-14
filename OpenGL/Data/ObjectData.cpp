#include "ObjectData.h"

#include <QFile>
#include <QTextStream>

ObjectData::ObjectData(Object::Type type)
    : mType(type)
{}

bool ObjectData::create()
{
    initializeOpenGLFunctions();

    qInfo() << "Loading model data for" << getModelFile().fileName();

    if (!loadData())
        return false;

    qDebug() << "Creating VAOs and VBOs" << mVertices.size() << mNormals.size();

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
                          GL_TRUE,           // Normalized?
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );

    mNormalBuffer.release();

    mVertexArray.release();

    return true;
}

bool ObjectData::loadData()
{
    QVector<QVector3D> tempVertices;
    QVector<QVector3D> tempNormals;
    QVector<QVector2D> tempUVs;

    QFile file = getModelFile();
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
                    if (arg.size() == 2) {
                        mVertices << tempVertices[arg[0].toInt() - 1];
                        mNormals << tempNormals[arg[1].toInt() - 1];
                    } else if (arg.size() == 3) {
                        mVertices << tempVertices[arg[0].toInt() - 1];
                        mUVs << tempUVs[arg[1].toInt() - 1];
                        mNormals << tempNormals[arg[2].toInt() - 1];
                    }
                }

            } else if (fileLine.startsWith("mtllib ")) {
            }
        }
    } else {
        qWarning() << QString("Could not open file '%1'.").arg(file.fileName());
        file.close();
        return false;
    }
    file.close();

    return true;
}

QFile ObjectData::getModelFile()
{
    switch (mType) {
    case Object::Camera:
        return QFile(":/Resources/Models/Camera.obj");
    case Object::Capsule:
        return QFile(":/Resources/Models/Capsule.obj");
    case Object::Cone:
        return QFile(":/Resources/Models/Cone.obj");
    case Object::Cube:
        return QFile(":/Resources/Models/Cube.obj");
    case Object::Cylinder:
        return QFile(":/Resources/Models/Cylinder.obj");
    case Object::Dome:
        return QFile(":/Resources/Models/Dome.obj");
    case Object::Plane:
        return QFile(":/Resources/Models/Plane.obj");
    case Object::Pyramid:
        return QFile(":/Resources/Models/Pyramid.obj");
    case Object::Sphere:
        return QFile(":/Resources/Models/Sphere.obj");
    case Object::Suzanne:
        return QFile(":/Resources/Models/Suzanne.obj");
    case Object::Tetrahedron:
        return QFile(":/Resources/Models/Tetrahedron.obj");
    case Object::Torus:
        return QFile(":/Resources/Models/Torus.obj");
    case Object::TorusKnot:
        return QFile(":/Resources/Models/TorusKnot.obj");
    }
}

void ObjectData::bind()
{
    mVertexArray.bind();
}

void ObjectData::release()
{
    mVertexArray.release();
}

int ObjectData::getVertexCount()
{
    return mVertices.count();
}
