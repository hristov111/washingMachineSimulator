#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <QVector>
#include <QQuaternion>
#include <QVector3D>
#include <QColor>
#include "myhashtable.h"

//-----------------------------------------------------------------------------
// A little 3D progress‐bar entity (background + fill), with its transforms
// exposed so we can rotate it in sync with the rest of the scene.
//-----------------------------------------------------------------------------
class ProgressBarEntity : public Qt3DCore::QEntity {
public:
    explicit ProgressBarEntity(Qt3DCore::QEntity *parent,
                               const QVector3D &position,
                               const QColor   &color,
                               float initialLevel = 0.0f);

    // adjust fill level [0.0–1.0]
    void setLevel(float level);

    // exposed so SceneModifier can rotate the entire bar
    Qt3DCore::QTransform *groupTransform;

private:
    // background
    Qt3DCore::QEntity           *bgEntity    = nullptr;
    Qt3DExtras::QCuboidMesh     *bgMesh      = nullptr;
    Qt3DExtras::QPhongMaterial  *bgMaterial  = nullptr;
    Qt3DCore::QTransform        *bgTransform = nullptr;

    // fill
    Qt3DCore::QEntity           *fillEntity    = nullptr;
    Qt3DExtras::QCuboidMesh     *fillMesh      = nullptr;
    Qt3DExtras::QPhongMaterial  *fillMaterial  = nullptr;
    Qt3DCore::QTransform        *fillTransform = nullptr;
};

//-----------------------------------------------------------------------------
// SceneModifier now also manages three ProgressBarEntity instances.
//-----------------------------------------------------------------------------
class SceneModifier {
public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);

    // 3D text labels
    void add3DLabel(const QString &text,
                    const QVector3D &position,
                    const QColor   &color = Qt::yellow,
                    MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *table = nullptr);

    // Rotate both labels and progress bars
    void applyRotationToLabels(const QQuaternion &rotation);

    // Adds the three bars (water, detergent, softener) plus their labels
    void addProgressBars();

    // Update fill‐levels (fractions in [0,1])
    void updateProgressBars(float waterLevel,
                            float detergentLevel,
                            float softenerLevel);

private:
    Qt3DCore::QEntity             *m_rootEntity   = nullptr;
    QVector<Qt3DCore::QTransform*> labelTransforms;
    QVector<Qt3DCore::QTransform*> barTransforms;

    ProgressBarEntity             *waterTank       = nullptr;
    ProgressBarEntity             *lintTrap   = nullptr;
    ProgressBarEntity             *detergantTray    = nullptr;

    // Helper to place a 3D text label under a bar
    void addBarLabel(const QString &text, const QVector3D &position);
};

#endif // SCENEMODIFIER_H
