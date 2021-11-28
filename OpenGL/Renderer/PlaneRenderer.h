#ifndef PLANERENDERER_H
#define PLANERENDERER_H

#include "Renderer.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class PlaneRenderer : Renderer
{
public:
    PlaneRenderer();
    bool initialize() override;
    void render(QVector<Object *> objects, const Camera *camera, const Light *light) override;

protected:
    const float *getVertexData() const override;
    const int getVertexDataByteSize() const override;

private:
    bool mInitialized;
    static const float VERTEX_DATA[36];
};

#endif // PLANERENDERER_H
