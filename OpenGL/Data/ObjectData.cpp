#include "ObjectData.h"

ObjectData::ObjectData() {}

ObjectData::~ObjectData() {}

void ObjectData::bind() { mVertexArray.bind(); }

void ObjectData::release() { mVertexArray.release(); }

int ObjectData::getVertexCount() { return mVertices.count(); }

QVector<QVector3D> ObjectData::vertices() const { return mVertices; }

void ObjectData::setVertices(const QVector<QVector3D> newVertices) { mVertices = newVertices; }

QVector<QVector3D> ObjectData::normals() const { return mNormals; }

void ObjectData::setNormals(const QVector<QVector3D> newNormals) { mNormals = newNormals; }
