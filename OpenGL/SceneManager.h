#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <OpenGL/Renderer/BasicObjectRenderer.h>
#include <OpenGL/Renderer/ModelRenderer.h>

#include <QMouseEvent>
#include <QTimer>

class SceneManager : public QObject
{
    Q_OBJECT
public:
    explicit SceneManager(QObject *parent = nullptr);
    bool init();
    void render();
    void resize(int w, int h);

public slots:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    void createBasicObjects();
    void createModels();

private:
    BasicObjectRenderer *mBasicObjectRenderer;
    ModelRenderer *mModelRenderer;
    QVector<Node *> mNodes;

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
};

#endif // SCENEMANAGER_H
