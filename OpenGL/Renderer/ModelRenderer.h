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
    QStringList getModelNames();
    QString getModelNameFromFilename(QString filename);
    ModelData *loadModel(QString path);

private:
    QMap<QString, ModelData *> mModels;
    bool mInit;
    int mModelColorLocation;
};

#endif // MODELRENDERER_H
