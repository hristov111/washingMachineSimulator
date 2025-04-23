#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <QVector>
#include "myhashtable.h"


class SceneModifier{
public:
    SceneModifier(Qt3DCore::QEntity *rootEntity);

    void add3DLabel(const QString &text , const QVector3D &position, const QColor &color = Qt::yellow, MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *table  =nullptr);
    void applyRotationToLabels(const QQuaternion &rotation);

private:
    Qt3DCore::QEntity *m_rootEntity = nullptr;
    QVector<Qt3DCore::QTransform*> labelTransforms;

};



#endif // SCENEMODIFIER_H
