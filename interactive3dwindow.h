#ifndef INTERACTIVE3DWINDOW_H
#define INTERACTIVE3DWINDOW_H

#include "scenemodifier.h"
#include "myhashtable.h"
#include <Qt3DExtras/Qt3DWindow>
#include <QPoint>
#include <Qt3DCore/QTransform>
#include <QQuaternion>


class Interactive3DWindow : public Qt3DExtras::Qt3DWindow{
    Q_OBJECT
public:
    explicit Interactive3DWindow(QScreen *screen = nullptr, MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *myTable = nullptr);

    void setTargetTransform(Qt3DCore::QTransform *transform);
    void setModifier(SceneModifier *mod);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


private:
    QPoint lastMousePos;
    Qt3DCore::QTransform *targetTransform = nullptr;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    SceneModifier *modifier = nullptr;

     MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *table = nullptr;



    // points o
};

#endif // INTERACTIVE3DWINDOW_H
