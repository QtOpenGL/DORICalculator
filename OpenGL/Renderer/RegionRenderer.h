#ifndef REGIONRENDERER_H
#define REGIONRENDERER_H

#include "Renderer.h"

#include <OpenGL/Node/RegionData.h>

class RegionRenderer : public Renderer
{
public:
    RegionRenderer();

    bool init() override;
    void render(QVector<Node *> nodes, const Camera *camera, const Light *light) override{};
    void render(QVector<RegionData *> regions, const Camera *camera, const Light *light);

private:
    bool mInit;
};

#endif // REGIONRENDERER_H
