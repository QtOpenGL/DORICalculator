#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include "OpenGL/Node/ModelData.h"
#include "Renderer.h"

class ModelRenderer : public Renderer
{
public:
    ModelRenderer();

    virtual bool init() override;
    virtual void render(QVector<Node *> nodes, const Camera *camera, const Light *light) override;

private:
    QMap<QString, ModelData *> mModels;
    bool mInit;
};

#endif // MODELRENDERER_H
