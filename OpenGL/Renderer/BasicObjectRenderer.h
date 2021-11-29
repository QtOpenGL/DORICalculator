#ifndef BASICOBJECTRENDERER_H
#define BASICOBJECTRENDERER_H

#include "../Camera.h"
#include "../Light.h"
#include "../Node/BasicObject.h"
#include "../Node/BasicObjectData.h"
#include "Renderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>

class BasicObjectRenderer : public Renderer
{
public:
    BasicObjectRenderer();

    virtual bool init() override;
    virtual void render(QVector<Node *> nodes, const Camera *camera, const Light *light) override;

private:
    QMap<BasicObject::Type, BasicObjectData *> mObjectData;

    int mObjectColorLocation;

    bool mInit;
};

#endif // BASICOBJECTRENDERER_H
