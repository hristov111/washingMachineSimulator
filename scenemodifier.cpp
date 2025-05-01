#include "scenemodifier.h"

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QFont>
#include <QQuaternion>
#include <QDebug>



SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{}

void SceneModifier::add3DLabel(const QString &text,
                               const QVector3D &position,
                               const QColor   &color,
                               MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *table)
{
    auto *ent = new Qt3DCore::QEntity(m_rootEntity);

    auto *mesh = new Qt3DExtras::QExtrudedTextMesh();
    mesh->setFont(QFont("Arial", 20, QFont::Bold));
    mesh->setText(text);
    mesh->setDepth(0.1f);

    auto *mat = new Qt3DExtras::QPhongMaterial();
    mat->setDiffuse(color);

    auto *xf = new Qt3DCore::QTransform();
    xf->setTranslation(position);
    xf->setScale(0.1f);

    ent->addComponent(mesh);
    ent->addComponent(mat);
    ent->addComponent(xf);

    labelTransforms.append(xf);
    if (table) table->insert(text, mat);
}

void SceneModifier::applyRotationToLabels(const QQuaternion &rotation)
{
    for (auto *t : labelTransforms)
        t->setRotation(rotation);

    for (auto *t : barTransforms)
        t->setRotation(rotation);
}



ProgressBarEntity::ProgressBarEntity(Qt3DCore::QEntity *parent,
                                     const QVector3D &position,
                                     const QColor   &color,
                                     float initialLevel)
    : Qt3DCore::QEntity(parent)
    , groupTransform(new Qt3DCore::QTransform())
{
    this->addComponent(groupTransform);
    groupTransform->setTranslation(position);

    bgEntity   = new Qt3DCore::QEntity(this);
    bgMesh     = new Qt3DExtras::QCuboidMesh();
    bgMesh->setXExtent(2.0f);
    bgMesh->setYExtent(0.2f);
    bgMesh->setZExtent(0.2f);

    bgMaterial = new Qt3DExtras::QPhongMaterial();
    bgMaterial->setDiffuse(QColor(100,100,100));

    bgTransform = new Qt3DCore::QTransform();
    bgTransform->setTranslation(QVector3D(0.0f, -0.4f, 0.0f));

    bgEntity->addComponent(bgMesh);
    bgEntity->addComponent(bgMaterial);
    bgEntity->addComponent(bgTransform);

    fillEntity   = new Qt3DCore::QEntity(this);
    fillMesh     = new Qt3DExtras::QCuboidMesh();
    fillMesh->setYExtent(0.15f);
    fillMesh->setZExtent(0.2f);

    fillMaterial = new Qt3DExtras::QPhongMaterial();
    fillMaterial->setDiffuse(color);

    fillTransform = new Qt3DCore::QTransform();
    fillTransform->setTranslation(QVector3D(-1.0f, -0.4f, 0.01f));

    fillEntity->addComponent(fillMesh);
    fillEntity->addComponent(fillMaterial);
    fillEntity->addComponent(fillTransform);

    setLevel(initialLevel);
}

void ProgressBarEntity::setLevel(float level)
{
    level = qBound(0.0f, level, 1.0f);

    float fullLength = 2.0f * level;
    fillMesh->setXExtent(fullLength);

    QVector3D t = fillTransform->translation();
    fillTransform->setTranslation({
        -1.0f + fullLength * 0.5f,
        t.y(),
        t.z()
    });
}


void SceneModifier::addProgressBars()
{
    if (!m_rootEntity) return;


    const QVector3D basePos(3.5f, 0.0f, 1.0f);

    waterTank = new ProgressBarEntity(m_rootEntity,
                                     basePos,
                                     QColor(67,133,244),
                                     0.0f);
    barTransforms << waterTank->groupTransform;
    addBarLabel("Water Tank", basePos);

    QVector3D dpos = basePos + QVector3D(0, 1.0f, 0);
    lintTrap = new ProgressBarEntity(m_rootEntity,
                                         dpos,
                                         QColor(244,180,67),
                                         0.0f);
    barTransforms << lintTrap->groupTransform;
    addBarLabel("Lint-trap", dpos);

    QVector3D spos = dpos + QVector3D(0, 1.0f, 0);
    detergantTray = new ProgressBarEntity(m_rootEntity,
                                        spos,
                                        QColor(123,201,111),
                                        0.0f);
    barTransforms << detergantTray->groupTransform;
    addBarLabel("Detergant tray", spos);
}

void SceneModifier::updateProgressBars(float waterLevel,
                                       float detergentLevel,
                                       float softenerLevel)
{
    if (waterTank)     waterTank->setLevel(waterLevel);
    if (lintTrap) lintTrap->setLevel(detergentLevel);
    if (detergantTray)  detergantTray->setLevel(softenerLevel);
}

void SceneModifier::addBarLabel(const QString &text, const QVector3D &position)
{
    auto *ent = new Qt3DCore::QEntity(m_rootEntity);

    auto *mesh = new Qt3DExtras::QExtrudedTextMesh();
    mesh->setFont(QFont("Arial", 14, QFont::Bold));
    mesh->setText(text);
    mesh->setDepth(0.2f);

    auto *mat = new Qt3DExtras::QPhongMaterial();
    mat->setDiffuse(Qt::white);

    auto *xf = new Qt3DCore::QTransform();
    xf->setTranslation(position + QVector3D(0.0f, -0.9f, 0.0f));
    xf->setScale(0.12f);

    ent->addComponent(mesh);
    ent->addComponent(mat);
    ent->addComponent(xf);

    barTransforms.append(xf);
}
