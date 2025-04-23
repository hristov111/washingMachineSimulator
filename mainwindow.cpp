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
    // make buttons

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
    modifier->add3DLabel("1. Insert Powder",InsertPowderPosition , QColor(Qt::red), colorTable);
    // Select program label
    modifier->add3DLabel("2. Select Program" ,selectProgramPosition, QColor(Qt::red), colorTable);
    // start program
    modifier->add3DLabel("3. Start Program" ,startProgramPosition, QColor(Qt::red), colorTable);
    // emegrency stop
    modifier->add3DLabel("4. Emergency stop" ,stopProgramPositons, QColor(Qt::red), colorTable);

    // make buttons for every program




    // add the label to the table
    view->setModifier(modifier);

    // Set root entity
    view->setRootEntity(rootEntity);
    //connect(ui->pushButton, &QPushButton::clicked,this, &MainWindow::addFunc);
    connect(ui->pushButton,  &QPushButton::clicked, this,&MainWindow::insertPowder_button);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFunc()
{

}
void makeButtonGreen(std::optional<Qt3DExtras::QPhongMaterial*> res){
    if(res.has_value()){
        res.value()->setDiffuse(QColor(Qt::green));
    }
}


void MainWindow::resizeEvent(QResizeEvent * event){
    QMainWindow::resizeEvent(event);
    if(overlay){
        overlay->resize(ui->container3DWidget->size());
    }
}


void MainWindow::insertPowder_button()
{
    // get the button that is currently clicked


    QPushButton *insertPowderButton= qobject_cast<QPushButton*>(sender());
    // and hide it
    ui->insertPowderPage->hide();
    ui->programsPage->show();
    qDebug() << "Clicked Insert button ";
    auto res = colorTable->get("1. Insert Powder");
    makeButtonGreen(res);
    // show the other step buttons

}






void MainWindow::on_cottonsButton_clicked()
{
    qDebug() << "Cottons clicked!";

    // hide the buttons
    //
}


void MainWindow::on_cottonsEcoButton_clicked()
{
}


void MainWindow::on_syntheticsButton_clicked()
{

}


void MainWindow::on_woolSilkButton_clicked()
{

}


void MainWindow::on_antiAllergyButton_clicked()
{

}


void MainWindow::on_nonStopButton_clicked()
{

}


void MainWindow::on_antiCreaseButton_clicked()
{

}


void MainWindow::on_refreshButton_clicked()
{

}

float adjustMinutes(float timeInHours, int minutesDelta){
    if(minutesDelta == 0)return timeInHours;
    int hours = static_cast<int>(timeInHours);
    int minutes = static_cast<int>((timeInHours - hours) * 100);

    int totalMinutes = hours * 60 + minutes + minutesDelta;

    if(totalMinutes < 0) totalMinutes = 0;

    int newHours = totalMinutes / 60;
    int newMinutes = totalMinutes % 60;


    float result = newHours + (newMinutes / 100.0f);
    return result;
}


float MainWindow::calculateEndtime(){
    float first = adjustMinutes(endTime,tempTime);
    float second = adjustMinutes(first, rinseTime);
    float third = adjustMinutes(second, ecoTime);
    float fourth = adjustMinutes(third,spinTime);
    return fourth;
}

// FOR COTTONS ---------------------------------------------------------------------
void MainWindow::on_temperatureBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;

    if(arg1 == "90°C") tempTime = 20;
    else if(arg1 == "40°C")  tempTime = -20;
    else if(arg1 == "30°C")tempTime = -25;
    else if(arg1 == "20°C") tempTime = -30;
    else tempTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));

}


void MainWindow::on_spinBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "1400RPM")  spinTime = 5;
    else if(arg1 == "1000RPM") spinTime = -5;
    else if(arg1 == "800RPM") spinTime = -10;
    else if(arg1 == "600RPM") spinTime = -15;
    else spinTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));

}


void MainWindow::on_rinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "1")rinseTime = -10;
    else rinseTime = 0;

    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_ecomodeBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = 30;
    else ecoTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}

// FOR COTTONS ECO --------------------------------------------------------------

void MainWindow::on_temperatureBox_2_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;

    if(arg1 == "60°C")  tempTime = 10;
    else if(arg1 == "30°C")tempTime = -10;
    else if(arg1 == "20°C") tempTime = -15;
    else tempTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_spinBox_2_currentTextChanged(const QString &arg1)
{
    // the same as cottons
    on_spinBox_currentTextChanged(arg1);
    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_rinseBox_2_currentTextChanged(const QString &arg1)
{
    on_rinseBox_currentTextChanged(arg1);

    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_ecomodeBox_2_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "Off")ecoTime = -40;
    else ecoTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_startButton_2_clicked()
{

}


void MainWindow::on_goBackButton_2_clicked()
{

}


// FOR SYNTHETYCS ----------------------------------------------------------

void MainWindow::on_temperatureBox_3_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;

    if(arg1 == "60°C")  tempTime = 10;
    else if(arg1 == "40°C")tempTime = 5;
    else if(arg1 == "20°C") tempTime = -10;
    else tempTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_spinBox_3_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "1200RPM")  spinTime = 10;
    else if(arg1 == "1000RPM") spinTime = 5;
    else if(arg1 == "600RPM") spinTime = -5;
    else spinTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_rinseBox_3_currentTextChanged(const QString &arg1)
{
    on_rinseBox_currentTextChanged(arg1);

    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_ecomodeBox_3_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = 10;
    else ecoTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_startButton_3_clicked()
{

}


void MainWindow::on_goBackButton_3_clicked()
{

}

// FOR WOOL/SILK PAGE --------------------------------------------------
void MainWindow::on_temperatureBox_4_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;

    if(arg1 == "30°C")  tempTime = 5;
    else if(arg1 == "40°C")tempTime = 10;
    else tempTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_spinBox_4_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "600RPM") spinTime = 3;
    else if(arg1 == "800RPM") spinTime = 5;
    else spinTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_rinseBox_4_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "2")rinseTime = 5;
    else if(arg1 == "0")rinseTime = -5;
    else rinseTime = 0;

    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_ecomodeBox_4_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = -5;
    else ecoTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_startButton_4_clicked()
{

}


void MainWindow::on_goBackButton_4_clicked()
{

}

// FOR ANTI-ALLERGY PROGRAM

void MainWindow::on_temperatureBox_5_currentTextChanged(const QString &arg1)
{
    if(arg1 == "70°C")  tempTime = 10;
    else if(arg1 == "90°C")tempTime = 20;
    else tempTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_spinBox_5_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "1200RPM") spinTime = 5;
    else if(arg1 == "1400RPM") spinTime = 10;
    else spinTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_rinseBox_5_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "4")rinseTime = 20;
    else rinseTime = 0;

    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_ecomodeBox_5_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = -20;
    else ecoTime = 0;


    ui->hourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_steamBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")steamTime= -20;
    else steamTime = 0;

    float res = adjustMinutes(calculateEndtime(), steamTime);

    ui->hourLabel->setText(QString::number( res, 'f', 2));
}


void MainWindow::on_startButton_5_clicked()
{

}


void MainWindow::on_goBackButton_5_clicked()
{

}

// NON STOP page

void MainWindow::on_temperatureBox_6_currentTextChanged(const QString &arg1)
{

}


void MainWindow::on_spinBox_6_currentTextChanged(const QString &arg1)
{

}

