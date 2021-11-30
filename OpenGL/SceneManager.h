#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QMouseEvent>
#include <QTimer>

class BasicObjectRenderer;
class ModelRenderer;
class RegionRenderer;
class Camera;
class Light;
class Node;
class RegionData;

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
    RegionRenderer *mRegionRenderer;

    QVector<Node *> mNodes;
    QVector<RegionData *> mRegions;

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;
    QTimer mSlowTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
};

#endif // SCENEMANAGER_H
