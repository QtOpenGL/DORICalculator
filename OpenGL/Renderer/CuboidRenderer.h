#ifndef CUBOIDRENDERER_H
#define CUBOIDRENDERER_H

#include "Renderer.h"

#include <OpenGL/Camera.h>
#include <OpenGL/Light.h>
#include <OpenGL/Object/Cuboid.h>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class CuboidRenderer : public Renderer
{
public:
    CuboidRenderer();

    bool initialize() override;
    void render(QVector<Object *> objects, const Camera *camera, const Light *light) override;

protected:
    const float *getVertexData() const override;
    const int getVertexDataByteSize() const override;

private:
    bool mInitialized;
    static const float VERTEX_DATA[216];
};

#endif // CUBOIDRENDERER_H
