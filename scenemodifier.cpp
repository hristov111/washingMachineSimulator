#include "scenemodifier.h"
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    :m_rootEntity(rootEntity){

}

void SceneModifier::add3DLabel(const QString &text, const QVector3D &position, const QColor &color ,  MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *table){


    // Vissible labelentity
    auto *labelEntity = new Qt3DCore::QEntity(m_rootEntity);

    auto *mesh = new Qt3DExtras::QExtrudedTextMesh();
    QFont font("Arial", 20, QFont::Bold);
    mesh->setFont(font);
    mesh->setText(text);
    mesh->setDepth(0.1f);

    auto *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    auto *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    transform->setScale(0.1f);

    labelTransforms.append(transform);

    // add the transfrom to the table
    table->insert(text, material);


    labelEntity->addComponent(mesh);
    labelEntity->addComponent(material);
    labelEntity->addComponent(transform);
}

void SceneModifier::applyRotationToLabels(const QQuaternion &rotation){
    for(auto *transform : labelTransforms){
        transform->setRotation(rotation);
    }
}
