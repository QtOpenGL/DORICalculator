#include "ModelData.h"

#include <QFile>
#include <QTextStream>

ModelData::ModelData(Model::Type type)
    : mType(type)
{}

ModelData::~ModelData() {}

bool ModelData::create()
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

void ModelData::update() {}

bool ModelData::loadData()
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
                    mVertices << tempVertices[arg[0].toInt() - 1];
                    mUVs << tempUVs[arg[1].toInt() - 1];
                    mNormals << tempNormals[arg[2].toInt() - 1];
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

const QVector<QVector2D> &ModelData::UVs() const { return mUVs; }

QFile ModelData::getModelFile()
{
    switch (mType) {
    case Model::Capsule: return QFile("Resources/Models/Capsule.obj");
    case Model::Cone: return QFile("Resources/Models/Cone.obj");
    case Model::Cube: return QFile("Resources/Models/Cube.obj");
    case Model::Cylinder: return QFile("Resources/Models/Cylinder.obj");
    case Model::Dome: return QFile("Resources/Models/Dome.obj");
    case Model::Plane: return QFile("Resources/Models/Plane.obj");
    case Model::Pyramid: return QFile("Resources/Models/Pyramid.obj");
    case Model::Sphere: return QFile("Resources/Models/Sphere.obj");
    case Model::Suzanne: return QFile("Resources/Models/Suzanne.obj");
    case Model::Tetrahedron: return QFile("Resources/Models/Tetrahedron.obj");
    case Model::Torus: return QFile("Resources/Models/Torus.obj");
    case Model::TorusKnot: return QFile("Resources/Models/TorusKnot.obj");
    }
}
