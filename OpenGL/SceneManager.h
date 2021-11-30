#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QMouseEvent>
#include <QTimer>

#include <OpenGL/Object/Object.h>

#include <OpenGL/Renderer/Renderer.h>

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
    Renderer *mRenderer;
    QVector<Object *> mObjects;

    QVector<RegionData *> mRegionData;

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;
    QTimer mSlowTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
};

#endif // SCENEMANAGER_H
