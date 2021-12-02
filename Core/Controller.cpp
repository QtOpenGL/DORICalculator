#include "Controller.h"

#include <Core/Constants.h>
#include <Core/Enums.h>
#include <GUI/CentralWidget.h>
#include <GUI/SideViewWidget.h>
#include <GUI/TopViewWidget.h>
#include <OpenGL/OpenGLWindow.h>

#include <QDebug>
#include <QtMath>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , mLogic(Logic::getInstance())
    , mZoomStepSize(2.0f)
    , mOrigin(368, 368)
    , mGround(Eigen::Vector3f(0, 0, 1), 0)
{
    mLogicParameters = new Logic::Parameters;
    mLogicParameters->camera.height = 5;
    mLogicParameters->target.height = 2;
    mLogicParameters->target.distance = 5;
    mLogicParameters->lowerBoundary.height = 0;
    mLogicParameters->lowerBoundary.distance = 0;

    mLogicParameters->frustum.zNear = 0.1;
    mLogicParameters->frustum.zFar = 1000;

    mLogicParameters->camera.sensor.width = 1920.0f;
    mLogicParameters->camera.sensor.height = 1080.0f;
    mLogicParameters->camera.sensor.aspectRatio = 1920.0f / 1080.0f;
    mLogicParameters->target.fovWidth = 10;

    mLogic.setParameters(mLogicParameters);
    mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(10);

    mCentralWidget = new CentralWidget;

    mSideViewWidget = mCentralWidget->sideViewWidget();
    mSideViewWidgetParameters = new SideViewWidgetParameters;
    mSideViewWidget->setParameters(mSideViewWidgetParameters);

    mTopViewWidget = mCentralWidget->topViewWidget();
    mTopViewWidgetParameters = new TopViewWidgetParameters;
    mTopViewWidget->setParameters(mTopViewWidgetParameters);

    mAxisWidget = mCentralWidget->axisWidget();

    mLeftWidget = mCentralWidget->leftWidget();
    mLeftWidgetParameters = new Logic::Parameters;
    mLeftWidget->setParameters(mLeftWidgetParameters);

    mOpenGLWindowParameters = new OpenGLWindowParameters;
    mOpenGLWindow = new OpenGLWindow;
    mOpenGLWindow->setParameters(mOpenGLWindowParameters);

    // Connections
    connect(mSideViewWidget, &SideViewWidget::dirty, this, &Controller::onDirty);
    connect(mSideViewWidget, &SideViewWidget::zoom, this, &Controller::onZoom);
    connect(mSideViewWidget, &SideViewWidget::pan, this, &Controller::onPan);
    connect(mSideViewWidget, &SideViewWidget::cursorPositionChanged, mLeftWidget, &LeftWidget::onCursorPositionChanged);

    connect(mTopViewWidget, &TopViewWidget::dirty, this, &Controller::onDirty);
    connect(mTopViewWidget, &TopViewWidget::zoom, this, &Controller::onZoom);
    connect(mTopViewWidget, &TopViewWidget::pan, this, &Controller::onPan);
    connect(mTopViewWidget, &TopViewWidget::cursorPositionChanged, mLeftWidget, &LeftWidget::onCursorPositionChanged);

    connect(mLeftWidget, &LeftWidget::dirty, this, &Controller::onDirty);

    setMeterToPixelRatio(8);
    setOrigin(mOrigin);

    update();
}

void Controller::update()
{
    mLogic.calculate();
    updateSideViewWidgetParameters();
    updateTopViewWidgetParameters();
    updateLeftWidgetParameters();
    updateOpenGLWindowParameters();

    mSideViewWidget->refresh();
    mTopViewWidget->refresh();
    mLeftWidget->refresh();
    mOpenGLWindow->refresh();
}

void Controller::updateTopViewWidgetParameters()
{
    mTopViewWidgetParameters->target.distance = mLogicParameters->target.distance;
    mTopViewWidgetParameters->target.fovWidth = mLogicParameters->target.fovWidth;
    mTopViewWidgetParameters->camera.tiltAngle = mLogicParameters->camera.tiltAngle;
    mTopViewWidgetParameters->camera.height = mLogicParameters->camera.height;
    mTopViewWidgetParameters->camera.horizontalFov = mLogicParameters->frustum.horizontalFov;
    mTopViewWidgetParameters->camera.verticalFov = mLogicParameters->frustum.verticalFov;

    for (int i = 0; i < 4; ++i) {
        mTopViewWidgetParameters->ground.intersections[i] = mTopViewWidget->mapFrom3d(
            mLogicParameters->frustum.bottomVertices[i]);
        mTopViewWidgetParameters->target.intersections[i] = mTopViewWidget->mapFrom3d(
            mLogicParameters->target.intersections[i]);
        mTopViewWidgetParameters->lowerBoundary.intersections[i] = mTopViewWidget->mapFrom3d(
            mLogicParameters->lowerBoundary.intersections[i]);
    }

    QPolygonF targetRoi;

    if (abs(mLogicParameters->camera.height - mLogicParameters->target.height) < 0.0001f) {
        targetRoi.append(mTopViewWidget->mapFrom3d(mTopViewWidgetParameters->target.distance,
                                                   0.5f * mTopViewWidgetParameters->target.fovWidth));
        targetRoi.append(mTopViewWidget->mapFrom3d(0, 0));
        targetRoi.append(mTopViewWidget->mapFrom3d(mTopViewWidgetParameters->target.distance,
                                                   -0.5f * mTopViewWidgetParameters->target.fovWidth));
    } else if (mLogicParameters->camera.height < mLogicParameters->target.height) {
        targetRoi.append(mTopViewWidget->mapFrom3d(0, 0));
        targetRoi.append(mTopViewWidgetParameters->target.intersections[0]);
        targetRoi.append(mTopViewWidgetParameters->target.intersections[3]);
    } else {
        targetRoi.append(mTopViewWidgetParameters->target.intersections[0]);
        targetRoi.append(mTopViewWidgetParameters->target.intersections[1]);
        targetRoi.append(mTopViewWidgetParameters->target.intersections[2]);
        targetRoi.append(mTopViewWidgetParameters->target.intersections[3]);
    }

    QPolygonF lowerBoundaryRoi;
    lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[0]);
    lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[1]);
    lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[2]);
    lowerBoundaryRoi.append(mTopViewWidgetParameters->lowerBoundary.intersections[3]);

    QPolygonF roi = lowerBoundaryRoi.intersected(targetRoi);

    for (int i = 0; i < 7; ++i) {
        Eigen::Hyperplane<float, 3> plane(Eigen::Vector3f(0, 0, 1).normalized(),
                                          -mLogicParameters->lowerBoundary.height);
        QVector<Eigen::Vector3f> intersections = mLogic.findIntersection(mLogicParameters->regions[i], plane);

        QPolygonF region;
        for (int i = 0; i < intersections.size(); ++i) {
            region.append(mTopViewWidget->mapFrom3d(intersections[i]));
        }

        region = region.intersected(roi);
        mTopViewWidgetParameters->regions[i].region = region;
        mTopViewWidgetParameters->regions[i].visible = !region.isEmpty();
    }
}

void Controller::updateLeftWidgetParameters() { *mLeftWidgetParameters = *mLogicParameters; }

void Controller::updateOpenGLWindowParameters()
{
    for (int i = 0; i < 7; i++) {
        QVector<Eigen::Vector3f> vertices = mLogic.findIntersection(mLogicParameters->regions[i], mGround);
        mOpenGLWindowParameters->regions[i].vertices = convertToOpenGLConvention(vertices);
        mOpenGLWindowParameters->regions[i].intersectsGround = intersectsGround(mOpenGLWindowParameters->regions[i]);
    }

    mOpenGLWindowParameters->frustumEdgeVertices = createFrustumEdgeVerticesForOpenGLWindow(mLogicParameters->frustum);
    mOpenGLWindowParameters->cameraHeight = mLogicParameters->camera.height;
    mOpenGLWindowParameters->tiltAngle = mLogicParameters->camera.tiltAngle;
}

bool Controller::intersectsGround(const OpenGLWindowRegion &region)
{
    for (int i = 0; i < region.vertices.size(); i++) {
        if (region.vertices[i].y() >= 0)
            return true;
    }

    return false;
}

QVector<QVector3D> Controller::convertToOpenGLConvention(const QVector<Eigen::Vector3f> &vectors)
{
    QVector<QVector3D> result;
    int size = vectors.size();

    for (int i = 1; i < size; i += 2) {
        result << QVector3D(vectors[i - 1].x(), vectors[i - 1].z(), -vectors[i - 1].y());
        result << QVector3D(vectors[i].x(), vectors[i].z(), -vectors[i].y());
        result << QVector3D(vectors[(i + 1) % size].x(), vectors[(i + 1) % size].z(), -vectors[(i + 1) % size].y());
    }

    return result;
}

QVector<QVector3D> Controller::createFrustumEdgeVerticesForOpenGLWindow(const Logic::Frustum &frustum)
{
    QVector<QVector3D> result;

    for (int i = 0; i < 4; i++) {
        result << QVector3D(frustum.topVertices[i].x(), frustum.topVertices[i].z(), -frustum.topVertices[i].y());
        result << QVector3D(frustum.topVertices[(i + 1) % 4].x(),
                            frustum.topVertices[(i + 1) % 4].z(),
                            -frustum.topVertices[(i + 1) % 4].y());
    }

    for (int i = 0; i < 4; i++) {
        result << QVector3D(frustum.topVertices[i].x(), frustum.topVertices[i].z(), -frustum.topVertices[i].y());
        result << QVector3D(frustum.bottomVertices[i].x(),
                            frustum.bottomVertices[i].z(),
                            -frustum.bottomVertices[i].y());
    }

    for (int i = 0; i < 4; i++) {
        result << QVector3D(frustum.bottomVertices[i].x(),
                            frustum.bottomVertices[i].z(),
                            -frustum.bottomVertices[i].y());
        result << QVector3D(frustum.bottomVertices[(i + 1) % 4].x(),
                            frustum.bottomVertices[(i + 1) % 4].z(),
                            -frustum.bottomVertices[(i + 1) % 4].y());
    }
    return result;
}

QVector<QVector3D> Controller::createVerticesForOpenGLWindow(const Logic::Region &region)
{
    QVector<QVector3D> vertices;

    for (int i = 0; i < 3; i++) {
        vertices << QVector3D(region.topVertices[i].x(), region.topVertices[i].z(), -region.topVertices[i].y());
    }

    for (int i = 2; i < 5; i++) {
        vertices << QVector3D(region.topVertices[i % 4].x(),
                              region.topVertices[i % 4].z(),
                              -region.topVertices[i % 4].y());
    }

    for (int i = 0; i < 4; i++) {
        vertices << QVector3D(region.topVertices[i].x(), region.topVertices[i].z(), -region.topVertices[i].y());
        vertices << QVector3D(region.bottomVertices[i].x(), region.bottomVertices[i].z(), -region.bottomVertices[i].y());
        vertices << QVector3D(region.bottomVertices[(i + 1) % 4].x(),
                              region.bottomVertices[(i + 1) % 4].z(),
                              -region.bottomVertices[(i + 1) % 4].y());
        vertices << QVector3D(region.bottomVertices[(i + 1) % 4].x(),
                              region.bottomVertices[(i + 1) % 4].z(),
                              -region.bottomVertices[(i + 1) % 4].y());
        vertices << QVector3D(region.topVertices[(i + 1) % 4].x(),
                              region.topVertices[(i + 1) % 4].z(),
                              -region.topVertices[(i + 1) % 4].y());
        vertices << QVector3D(region.topVertices[i % 4].x(),
                              region.topVertices[i % 4].z(),
                              -region.topVertices[i % 4].y());
    }

    for (int i = 0; i < 3; i++) {
        vertices << QVector3D(region.bottomVertices[i].x(), region.bottomVertices[i].z(), -region.bottomVertices[i].y());
    }

    for (int i = 2; i < 5; i++) {
        vertices << QVector3D(region.bottomVertices[i % 4].x(),
                              region.bottomVertices[i % 4].z(),
                              -region.bottomVertices[i % 4].y());
    }

    return vertices;
}

QVector<QVector3D> Controller::createNormalsForOpenGLWindow(const QVector<QVector3D> &vertices)
{
    QVector<QVector3D> normals;

    for (int i = 1; i < vertices.size(); i += 3) {
        QVector3D u = vertices[i - 1] - vertices[i];
        QVector3D v = vertices[i + 1] - vertices[i];
        u.normalize();
        v.normalize();

        QVector3D vxu = QVector3D::crossProduct(v, u);
        vxu.normalize();
        normals << vxu;
        normals << vxu;
        normals << vxu;
    }

    return normals;
}

void Controller::updateSideViewWidgetParameters()
{
    mSideViewWidgetParameters->camera.tiltAngle = mLogicParameters->camera.tiltAngle;
    mSideViewWidgetParameters->camera.height = mLogicParameters->camera.height;
    mSideViewWidgetParameters->camera.horizontalFov = mLogicParameters->frustum.horizontalFov;
    mSideViewWidgetParameters->camera.verticalFov = mLogicParameters->frustum.verticalFov;
    mSideViewWidgetParameters->camera.position = mSideViewWidget->mapFrom3d(0, mLogicParameters->camera.height);
    mSideViewWidgetParameters->target.height = mLogicParameters->target.height;
    mSideViewWidgetParameters->target.distance = mLogicParameters->target.distance;
    mSideViewWidgetParameters->target.position = mSideViewWidget->mapFrom3d(mLogicParameters->target.distance,
                                                                            mLogicParameters->target.height);
    mSideViewWidgetParameters->lowerBoundary.height = mLogicParameters->lowerBoundary.height;
    mSideViewWidgetParameters->lowerBoundary.distance = mLogicParameters->lowerBoundary.distance;
    mSideViewWidgetParameters->lowerBoundary.position
        = mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->lowerBoundary.height);

    for (int i = 0; i < 4; ++i) {
        mSideViewWidgetParameters->ground.intersections[i] = mSideViewWidget->mapFrom3d(
            mLogicParameters->frustum.bottomVertices[i]);
        mSideViewWidgetParameters->target.intersections[i] = mSideViewWidget->mapFrom3d(
            mLogicParameters->target.intersections[i]);
        mSideViewWidgetParameters->lowerBoundary.intersections[i] = mSideViewWidget->mapFrom3d(
            mLogicParameters->lowerBoundary.intersections[i]);
    }

    mSideViewWidgetParameters->bisectorIntersection = mSideViewWidget->mapFrom3d(
        mLogicParameters->frustum.oppositeBisectorRay);
    mSideViewWidgetParameters->oppositeBisectorIntersection = mSideViewWidget->mapFrom3d(
        mLogicParameters->frustum.bisectorRay);

    QPolygonF roi;

    roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->target.distance, mLogicParameters->target.height));
    roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->target.distance, mLogicParameters->lowerBoundary.height));
    roi.append(
        mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->lowerBoundary.height));
    roi.append(mSideViewWidget->mapFrom3d(mLogicParameters->lowerBoundary.distance, mLogicParameters->target.height));

    for (int i = 0; i < 7; ++i) {
        QPolygonF region;

        region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].bottomVertices[0]));
        region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].topVertices[0]));
        region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].topVertices[2]));
        region.append(mSideViewWidget->mapFrom3d(mLogicParameters->regions[i].bottomVertices[2]));

        region = region.intersected(roi);
        mSideViewWidgetParameters->regions[i].region = region;
        mSideViewWidgetParameters->regions[i].visible = !region.isEmpty();
    }
}

void Controller::onDirty()
{
    QObject *sender = QObject::sender();
    if (sender == mSideViewWidget) {
        mLogicParameters->target.height = mLogic.validateTargetHeight(mSideViewWidgetParameters->target.height);
        mLogicParameters->target.distance = mLogic.validateTargetDistance(mSideViewWidgetParameters->target.distance);
        mLogicParameters->camera.height = mLogic.validateCameraHeight(mSideViewWidgetParameters->camera.height);
        mLogicParameters->lowerBoundary.height = mSideViewWidgetParameters->lowerBoundary.height;

    } else if (sender == mTopViewWidget) {
        mLogicParameters->target.distance = mLogic.validateTargetDistance(mTopViewWidgetParameters->target.distance);
        mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(
            mTopViewWidgetParameters->target.fovWidth);
        mLogicParameters->target.fovWidth = mTopViewWidgetParameters->target.fovWidth;

    } else if (sender == mLeftWidget) {
        mLogicParameters->camera.height = mLogic.validateCameraHeight(mLeftWidgetParameters->camera.height);
        mLogicParameters->camera.sensor.width = mLeftWidgetParameters->camera.sensor.width;
        mLogicParameters->camera.sensor.height = mLeftWidgetParameters->camera.sensor.height;

        mLogicParameters->frustum.zNear = mLeftWidgetParameters->frustum.zNear;
        mLogicParameters->frustum.zFar = mLeftWidgetParameters->frustum.zFar;

        mLogicParameters->target.height = mLeftWidgetParameters->target.height;
        mLogicParameters->target.distance = mLeftWidgetParameters->target.distance;

        mLogicParameters->lowerBoundary.height = mLeftWidgetParameters->lowerBoundary.height;
        mLogicParameters->lowerBoundary.distance = mLeftWidgetParameters->lowerBoundary.distance;

        if (!qFuzzyCompare(mLogicParameters->target.fovWidth, mLeftWidgetParameters->target.fovWidth))
            mLogicParameters->frustum.horizontalFov = mLogic.calculateHorizontalFovForGivenFovWidth(
                mLeftWidgetParameters->target.fovWidth);
        else
            mLogicParameters->frustum.horizontalFov = mLeftWidgetParameters->frustum.horizontalFov;
    }

    update();
}

void Controller::onZoom(int i)
{
    if (i < 0) {
        setMeterToPixelRatio(mZoomStepSize * mMeterToPixelRatio);
    } else if (i > 0) {
        setMeterToPixelRatio(mMeterToPixelRatio / mZoomStepSize);
    }
}

void Controller::onPan(int x, int y) { setOrigin(QPointF(mOrigin.x() + x, mOrigin.y() + y)); }

void Controller::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (newMeterToPixelRatio < 2.0 || newMeterToPixelRatio > 512.0f) {
        return;
    }

    mMeterToPixelRatio = newMeterToPixelRatio;

    mSideViewWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mTopViewWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mAxisWidget->setMeterToPixelRatio(newMeterToPixelRatio);
    mAxisWidget->refresh();

    update();
}

void Controller::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;

    mSideViewWidget->setOrigin(newOrigin);
    mAxisWidget->setOrigin(newOrigin);
    mTopViewWidget->setOrigin(newOrigin);

    mAxisWidget->refresh();
    update();
}

OpenGLWindow *Controller::openGLWindow() const { return mOpenGLWindow; }

CentralWidget *Controller::centralWidget() { return mCentralWidget; }
