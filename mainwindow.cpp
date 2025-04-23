#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include "scenemodifier.h"
#include "interactive3dwindow.h"

#include <QTextEdit>
#include <QtWidgets/QApplication>
#include <QVBoxLayout>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>

#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DRender/QPointLight>
#include <QMouseEvent>
#include <QDebug>
#include <QStackedLayout>

#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QEntity>
#include <QFont>

#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 3D Window setup
    Interactive3DWindow *view = new Interactive3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    // create the table
    colorTable = new  MyHashtable<QString,Qt3DExtras::QPhongMaterial*>();

    // Embed the 3D view into the QWidget container
    container = QWidget::createWindowContainer(view, ui->container3DWidget);
    container->setFocusPolicy(Qt::StrongFocus);


    // set up a stacked layout

    QStackedLayout  *stackedLayout = new QStackedLayout (ui->container3DWidget);
    stackedLayout->setContentsMargins(0,0,0,0);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    ui->container3DWidget->setLayout(stackedLayout);
    stackedLayout->addWidget(container);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    Qt3DCore::QTransform *transfrom = new Qt3DCore::QTransform();
    transfrom->setScale(5.0f);
    //rootEntity->addComponent(transfrom);
    auto addLight = [&](const QVector3D &position, const QColor &color, float intensity) {
        Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
        auto *light = new Qt3DRender::QPointLight(lightEntity);
        light->setColor(color);
        light->setIntensity(intensity);
        lightEntity->addComponent(light);

        auto *transform = new Qt3DCore::QTransform();
        transform->setTranslation(position);
        lightEntity->addComponent(transform);
    };
    addLight(QVector3D(10, 0, 0), QColor(200, 200, 200), 0.5f);   // Right
    addLight(QVector3D(-10, 0, 0), QColor(200, 200, 200), 0.6f);  // Left
    addLight(QVector3D(0, 10, 0), QColor(220, 220, 220), 0.3f);   // Top
    addLight(QVector3D(0, -10, 0), QColor(180, 180, 180), 0.3f);  // Bottom
    addLight(QVector3D(0, 0, -10), QColor(180, 180, 180), 0.5f);  // Back

    Qt3DCore::QEntity *dirLightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *dirLight = new Qt3DRender::QDirectionalLight(dirLightEntity);
    dirLight->setWorldDirection(QVector3D(-1.0f,-1.0f,-1.0f));
    dirLight->setColor("white");
    dirLight->setIntensity(1.5f);
    dirLightEntity->addComponent(dirLight);

    Qt3DCore::QTransform *dirTransform = new Qt3DCore::QTransform();
    dirTransform->setTranslation(QVector3D(10.0f, 10.0f, 10.0f));
    dirLightEntity->addComponent(dirTransform);

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.0f, 0.0f, 7.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));


    // Load 3D Model
    modelTransform = new Qt3DCore::QTransform();
    view->setTargetTransform(modelTransform);
    Qt3DRender::QSceneLoader *loader = new Qt3DRender::QSceneLoader(rootEntity);
    loader->setSource(QUrl::fromLocalFile("C:\\Users\\vboxuser\\Documents\\managementsystem\\assets\\scene.gltf"));

    auto *modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelTransform->setScale(5.0f);
    // coordinates of the object
    modelTransform->setTranslation(QVector3D(0.0f, -2.0f, 0.0f));
    modelEntity->addComponent(loader);
    modelEntity->addComponent(modelTransform);

    // label start positions
    auto InsertPowderPosition =QVector3D(-3.0f,1.7f, 2.0f);
    auto selectProgramPosition = QVector3D(-3.0f, 1.4f, 2.0f);
    auto startProgramPosition = QVector3D(-3.0f, 1.1f, 2.0f);
    auto stopProgramPositons = QVector3D(-3.0f, 0.8f, 2.0f);


    // lets place drawer label
    SceneModifier *modifier = new SceneModifier(rootEntity);
    // Insert Powder labhel
    modifier->add3DLabel("1. Insert Powder",InsertPowderPosition , QColor(Qt::green), colorTable);
    // Select program label
    modifier->add3DLabel("2. Select Program" ,selectProgramPosition, QColor(Qt::red), colorTable);
    // start program
    modifier->add3DLabel("3. Start Program" ,startProgramPosition, QColor(Qt::red), colorTable);
    // emegrency stop
    modifier->add3DLabel("4. Emergency stop" ,stopProgramPositons, QColor(Qt::red), colorTable);

    // add the label to the table
    view->setModifier(modifier);

    // Set root entity
    view->setRootEntity(rootEntity);
    //connect(ui->pushButton, &QPushButton::clicked,this, &MainWindow::addFunc);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFunc()
{

}



void MainWindow::resizeEvent(QResizeEvent * event){
    QMainWindow::resizeEvent(event);
    if(overlay){
        overlay->resize(ui->container3DWidget->size());
    }
}


void MainWindow::on_pushButton_2_clicked()
{

}




