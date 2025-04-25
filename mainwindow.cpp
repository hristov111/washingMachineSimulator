#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include "scenemodifier.h"
#include "interactive3dwindow.h"
#include <QMovie>

#include <QTextEdit>
#include <QtWidgets/QApplication>
#include <QVBoxLayout>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>

#include <QTimer>

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

    // connect the buttons
    // for insertPowder
    connect(ui->pushButton,  &QPushButton::clicked, this,&MainWindow::insertPowder_button);
    // for cottons clicked
    connect(ui->cottonsButton, &QPushButton::clicked,this, &MainWindow::on_cottonsButton_clicked);
    // for cottons eco clicked
    connect(ui->cottonsEcoButton, &QPushButton::clicked, this, &MainWindow::on_cottonsEcoButton_clicked);
    // for synthetics clicked
    connect(ui->syntheticsButton, &QPushButton::clicked,this, &MainWindow::on_syntheticsButton_clicked );
    // for wool/silk clicked
    connect(ui->woolSilkButton, &QPushButton::clicked,this, &MainWindow::on_woolSilkButton_clicked );
    // for anti allergy button
    connect(ui->antiAllergyButton, &QPushButton::clicked,this, &MainWindow::on_antiAllergyButton_clicked );
    // for anticrease button
    connect(ui->antiCreaseButton, &QPushButton::clicked,this, &MainWindow::on_antiCreaseButton_clicked );
    // for refresh page
    connect(ui->refreshButton, &QPushButton::clicked,this, &MainWindow::on_refreshButton_clicked );

    // connect go back buttons
    connect(ui->cottonsBackButton, &QPushButton::clicked,this, [=]() {
        backToPrograms([=] (){on_cottonsBackButton_clicked();});
    });
    connect(ui->cottonsEcoBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_cottonsEcoBackButton_clicked();});
    });

    connect(ui->syntheticsBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_syntheticsBackButton_clicked();});
    });

    connect(ui->woolSilkBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_woolSilkBackButton_clicked();});});

    connect(ui->antiAllergyBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_antiAllergyBackButton_clicked();});
    });

    connect(ui->nonStopBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_nonStopBackButton_clicked();});
    });

    connect(ui->goBackButton_7, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_antiCreaseBackButton_clicked();});
    });

    connect(ui->refreshBackButt, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_refreshBackButt_clicked();});
    });


    // timers connecting
    waterFillingSliderTimer = new QTimer(this);
    connect(waterFillingSliderTimer, &QTimer::timeout, this, &MainWindow::moveWaterFillingSlider);

    heatingWaterSliderTimer = new QTimer(this);
    connect(heatingWaterSliderTimer , &QTimer::timeout, this, &MainWindow::moveHeatingWaterSlider);


    // when we start the program we need to collect first the program name and all the comboboxes
    // then reset everything





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
void makeButtonGreen(std::optional<Qt3DExtras::QPhongMaterial*> res){
    if(res.has_value()){
        res.value()->setDiffuse(QColor(Qt::green));
    }
}

void makeButtonRed(std::optional<Qt3DExtras::QPhongMaterial*> res){
    if(res.has_value()){
        res.value()->setDiffuse(QColor(Qt::red));
    }
}


void MainWindow::resizeEvent(QResizeEvent * event){
    QMainWindow::resizeEvent(event);
    if(overlay){
        overlay->resize(ui->container3DWidget->size());
    }
}

void switchToPageByName(QStackedWidget * stackedwidget, const QString& name){
    QWidget *page = stackedwidget->findChild<QWidget*>(name);
    if(page)stackedwidget->setCurrentWidget(page);
}


void MainWindow::insertPowder_button()
{

    qDebug() << "Clicked Insert button ";
    auto res = colorTable->get("1. Insert Powder");
    makeButtonGreen(res);
    switchToPageByName(ui->stackedWidget, "programsPage");

}

// TIMER FUNCTIONS

void MainWindow::moveWaterFillingSlider(){
    int value = ui->fillingWaterSlider->value();
    int maxValue = ui->fillingWaterSlider->maximum();
    int percentage = (value * 100) / maxValue;
    ui->fillingWaterLabel->setText("Filling Water " + QString::number(percentage) + " %");
    if(value < maxValue){
        ui->fillingWaterSlider->setValue(value +1);
    }else {
        waterFillingSliderTimer->stop();
        waterFillingSpinnerMovie->stop();
        waterFillingLabel->hide();
        ui->fillingWaterSpinnerHolderLabel->setText("✔");
        // starting heating water
        startSlider(heatingWaterLabel,ui->heatingWaterSpinningHolderLabel,heatingWaterSpinnerMovie,heatingWaterSliderTimer);
    }
}

void MainWindow::moveHeatingWaterSlider(){
    QString temp = ui->cottonsTempBox->currentText();
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp);
    ui->heatingWaterSlider->setMinimum(0);
    if(temp == "Cold"){
        ui->heatingWaterSlider->setMaximum(0);
    }else ui->heatingWaterSlider->setMaximum(temp.left(2).toInt());
    int value = ui->heatingWaterSlider->value();
    int maxValue = ui->heatingWaterSlider->maximum();
    if(value < maxValue){
        ui->heatingWaterSlider->setValue(value +1);
    }else {
        heatingWaterSliderTimer->stop();
        heatingWaterSpinnerMovie->stop();
        heatingWaterLabel->hide();
        ui->heatingWaterSpinningHolderLabel->setText("✔");
        // starting

    }
}

// PENDING CIRCLE FUNCTION
void MainWindow::startSlider(QLabel* label, QLabel* holderLabel, QMovie * spinnerMovie,QTimer* timer) {
    if(!label){
        label = new QLabel(holderLabel);
        label->setGeometry(0,0,holderLabel->width(),holderLabel->height());
        label->setAlignment(Qt::AlignCenter);

        // Load the spinner movie
        spinnerMovie = new QMovie("C:\\Users\\vboxuser\\Documents\\managementsystem\\assets\\Spinner@1x-1.0s-60px-60px (1).gif");
        label->setMovie(spinnerMovie);
        spinnerMovie->start();
    }
    // show the label
    label->show();
    // start filling the slider
    timer->start(100);
}





void MainWindow::on_cottonsButton_clicked()
{
    qDebug() << "Cottons clicked!";

    // switch page
    switchToPageByName(ui->stackedWidget, "cottonsDescPage");
    makeButtonGreen(colorTable->get("2. Select Program"));

}


void MainWindow::on_cottonsEcoButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "cottonsEcoPage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}


void MainWindow::on_syntheticsButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "syntheticsPage");
    makeButtonGreen(colorTable->get("2. Select Program"));

}


void MainWindow::on_woolSilkButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "woolSilkPage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}


void MainWindow::on_antiAllergyButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "antiAllergyPage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}


void MainWindow::on_nonStopButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "nonStopPage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}


void MainWindow::on_antiCreaseButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "antiCreasePage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}


void MainWindow::on_refreshButton_clicked()
{
    switchToPageByName(ui->stackedWidget, "refreshPage");
    makeButtonGreen(colorTable->get("2. Select Program"));


}

void MainWindow::backToPrograms(std::function<void()> callback){
    switchToPageByName(ui->stackedWidget, "programsPage");
    makeButtonRed(colorTable->get("2. Select Program"));
    tempTime = 0;
    spinTime = 0;
    ecoTime = 0;
    rinseTime = 0;
    steamTime = 0;
    dryTime = 0;
    anticreaseTime = 0;
    callback();
}

float adjustMinutes(float timeInHours, int minutesDelta){
    if(minutesDelta == 0) return timeInHours;

    int hours = static_cast<int>(timeInHours);  // Get the hour part
    int minutes = static_cast<int>(round((timeInHours - hours) * 100));  // Get the "minutes" part

    int totalMinutes = hours * 60 + minutes + minutesDelta;

    if(totalMinutes < 0) totalMinutes = 0;

    int newHours = totalMinutes / 60;
    int newMinutes = totalMinutes % 60;

    float result = newHours + (newMinutes / 100.0f);  // Keep your custom format
    return result;
}


float MainWindow::calculateEndtime(){
    float res = adjustMinutes(endTime,tempTime);
    res= adjustMinutes(res, rinseTime);
    res = adjustMinutes(res, ecoTime);
    res = adjustMinutes(res ,spinTime);
    res = adjustMinutes(res,steamTime);
    res = adjustMinutes(res, dryTime);
    res = adjustMinutes(res,anticreaseTime);
    return res;
}

// FOR COTTONS ---------------------------------------------------------------------
void MainWindow::on_cottonsTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    qDebug() << "Cottons will change";
    if(arg1 == "90°C") tempTime = 20;
    else if(arg1 == "40°C")  tempTime = -20;
    else if(arg1 == "30°C")tempTime = -25;
    else if(arg1 == "20°C") tempTime = -30;
    else tempTime = 0;


    ui->cottonsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));

}


void MainWindow::on_spinBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "1400RPM")  spinTime = 5;
    else if(arg1 == "1000RPM") spinTime = -5;
    else if(arg1 == "800RPM") spinTime = -10;
    else if(arg1 == "600RPM") spinTime = -15;
    else spinTime = 0;


    ui->cottonsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));

}


void MainWindow::on_cottonsRinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "1")rinseTime = -10;
    else rinseTime = 0;

    ui->cottonsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = 30;
    else ecoTime = 0;


    ui->cottonsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsStartButt_clicked()
{
    // collect the name of the program
    QString program = ui->cottonsTitle->text();
    QString temp = ui->cottonsTempBox->currentText().left(2);
    QString spin = ui->spinBox->currentText();
    QString rinse = ui->cottonsRinseBox->currentText();
    QString ecoMode = ui->cottonsEcoModeBox->currentText();
    // calculate time


    // and the hours
    QString washHour = ui->cottonsHourLabel->text();
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer);
    switchToPageByName(ui->stackedWidget, "startProgramPage");

    // create the spinner QLabe;l as a child of spinnerHolderLabel

    // restore the page
    //on_cottonsBackButton_clicked();

}


// FOR COTTONS ECO --------------------------------------------------------------

void MainWindow::on_cottonsEcoTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;

    if(arg1 == "60°C")  tempTime = 10;
    else if(arg1 == "30°C")tempTime = -10;
    else if(arg1 == "20°C") tempTime = -15;
    else tempTime = 0;


    ui->cottonsEcoHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsEcoSpinBox_currentTextChanged(const QString &arg1)
{
    // the same as cottons
    endTime = 3.0;
    if(arg1 == "1400RPM")  spinTime = 5;
    else if(arg1 == "1000RPM") spinTime = -5;
    else if(arg1 == "800RPM") spinTime = -10;
    else if(arg1 == "600RPM") spinTime = -15;
    else spinTime = 0;


    ui->cottonsEcoHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsEcoRinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "1")rinseTime = -10;
    else rinseTime = 0;

    ui->cottonsEcoHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsEcoEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;
    if(arg1 == "Off")ecoTime = -40;
    else ecoTime = 0;


    ui->cottonsEcoHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_cottonsEcoStartButt_clicked()
{

}




// FOR SYNTHETYCS ----------------------------------------------------------

void MainWindow::on_synthTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 1.0;

    if(arg1 == "60°C")  tempTime = 10;
    else if(arg1 == "40°C")tempTime = 5;
    else if(arg1 == "20°C") tempTime = -10;
    else tempTime = 0;


    ui->syntheticsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_synthSpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 1.0;
    if(arg1 == "1200RPM")  spinTime = 10;
    else if(arg1 == "1000RPM") spinTime = 5;
    else if(arg1 == "600RPM") spinTime = -5;
    else spinTime = 0;


    ui->syntheticsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_synthRinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 1.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "1")rinseTime = -10;
    else rinseTime = 0;

    ui->syntheticsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_synthEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 1.0;
    if(arg1 == "On")ecoTime = 10;
    else ecoTime = 0;


    ui->syntheticsHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_synthStartButt_clicked()
{

}



// FOR WOOL/SILK PAGE --------------------------------------------------
void MainWindow::on_woolSilkTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.40;

    if(arg1 == "30°C")  tempTime = 5;
    else if(arg1 == "40°C")tempTime = 10;
    else tempTime = 0;


    ui->woolSilkHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_woolSilkSpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.40;
    if(arg1 == "600RPM") spinTime = 3;
    else if(arg1 == "800RPM") spinTime = 5;
    else spinTime = 0;


    ui->woolSilkHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_woolSilkRinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.40;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "2")rinseTime = 5;
    else if(arg1 == "0")rinseTime = -5;
    else rinseTime = 0;

    ui->woolSilkHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_woolSilkEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.40;
    if(arg1 == "On")ecoTime = -5;
    else ecoTime = 0;


    ui->woolSilkHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_woolSilkStartButt_clicked()
{

}



// FOR ANTI-ALLERGY PROGRAM

void MainWindow::on_antiAllergyTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "70°C")  tempTime = 10;
    else if(arg1 == "90°C")tempTime = 20;
    else tempTime = 0;


    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergySpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "1200RPM") spinTime = 5;
    else if(arg1 == "1400RPM") spinTime = 10;
    else spinTime = 0;


    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergyRinseBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "3")  rinseTime = 10;
    else if(arg1 == "4")rinseTime = 20;
    else rinseTime = 0;

    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergyEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")ecoTime = -20;
    else ecoTime = 0;


    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergySteamBox_currentTextChanged(const QString &arg1)
{
    endTime = 2.0;
    if(arg1 == "On")steamTime= -20;
    else steamTime = 0;

    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergyStartButt_clicked()
{

}



// NON STOP Program page

void MainWindow::on_nonStopTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;
    if(arg1 == "40°C")  tempTime = 5;
    else if(arg1 == "60°C")tempTime = 10;
    else tempTime = 0;


    ui->nonStopHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_nonStopSpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;
    if(arg1 == "1000RPM") spinTime = 5;
    else if(arg1 == "1200RPM") spinTime = 10;
    else spinTime = 0;


    ui->nonStopHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_nonStopDryBox_currentTextChanged(const QString &arg1)
{

    endTime = 3.0;
    if(arg1 == "CupBoard Dry")dryTime = 10;
    else if(arg1 == "Extra Dry")dryTime = 20;
    else dryTime = 0;


    ui->nonStopHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));

}


void MainWindow::on_nonStopEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 3.0;
    if(arg1 == "On")ecoTime = -15;
    else ecoTime = 0;


    ui->nonStopHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_nonStopStartButt_clicked()
{

}




// FOR ANTI-CREASE Page

void MainWindow::on_antiCreaseTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.50;

    if(arg1 == "30°C")  tempTime = 5;
    else if(arg1 == "40°C")tempTime = 10;
    else tempTime = 0;


    ui->antiCreaseHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiCreaseSpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.50;
    if(arg1 == "600RPM") spinTime = 5;
    else if(arg1 == "800RPM") spinTime = 10;
    else spinTime = 0;


    ui->antiCreaseHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiCreaseExtraBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.50;
    if(arg1 == "Enabled") anticreaseTime = 5;
    else anticreaseTime = 0;

    ui->antiCreaseHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiCreaseEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.50;
    if(arg1 == "On")ecoTime = -10;
    else ecoTime = 0;


    ui->antiCreaseHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiCreaseStartButt_clicked()
{

}



// FOR REFRESH Program

void MainWindow::on_refreshTempBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.20;

    if(arg1 == "30°C")  tempTime = 5;
    else if(arg1 == "40°C")  tempTime = 10;
    else tempTime = 0;


    ui->refreshHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_refreshSpinBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.20;
    if(arg1 == "200RPM") spinTime = 2;
    else if(arg1 == "400RPM") spinTime = 5;
    else spinTime = 0;


    ui->refreshHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_refreshSteamBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.20;
    if(arg1 == "Enabled")steamTime= 10;
    else steamTime = 0;

    ui->refreshHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_refreshEcoModeBox_currentTextChanged(const QString &arg1)
{
    endTime = 0.20;
    if(arg1 == "On")ecoTime = -5;
    else ecoTime = 0;


    ui->refreshHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_refreshStartButt_clicked()
{

}

// back buttons






void MainWindow::on_cottonsEcoBackButton_clicked()
{
    // now here i need to reset all the comboboxes related to that page and alos the hour label
    ui->cottonsEcoHourLabel->setText("3");
    ui->cottonsEcoEcoModeBox->setCurrentIndex(0);
    ui->cottonsEcoRinseBox->setCurrentIndex(0);
    ui->cottonsEcoSpinBox->setCurrentIndex(0);
    ui->cottonsEcoTempBox->setCurrentIndex(0);

}


void MainWindow::on_cottonsBackButton_clicked()
{
    ui->cottonsHourLabel->setText("2");
    ui->cottonsEcoModeBox->setCurrentIndex(0);
    ui->cottonsRinseBox->setCurrentIndex(0);
    ui->cottonsTempBox->setCurrentIndex(0);
    ui->spinBox->setCurrentIndex(0);
}


void MainWindow::on_syntheticsBackButton_clicked()
{
    ui->syntheticsHourLabel->setText("1");
    ui->synthEcoModeBox->setCurrentIndex(0);
    ui->synthRinseBox->setCurrentIndex(0);
    ui->synthTempBox->setCurrentIndex(0);
    ui->synthSpinBox->setCurrentIndex(0);
}


void MainWindow::on_woolSilkBackButton_clicked()
{
    ui->woolSilkEcoModeBox->setCurrentIndex(0);
    ui->woolSilkHourLabel->setText("0.40");
    ui->woolSilkRinseBox->setCurrentIndex(0);
    ui->woolSilkSpinBox->setCurrentIndex(0);
    ui->woolSilkTempBox->setCurrentIndex(0);
}


void MainWindow::on_antiAllergyBackButton_clicked()
{
    ui->antiAllergyEcoModeBox->setCurrentIndex(0);
    ui->antiAllergyHourLabel->setText("2");
    ui->antiAllergyRinseBox->setCurrentIndex(0);
    ui->antiAllergySpinBox->setCurrentIndex(0);
    ui->antiAllergySteamBox->setCurrentIndex(0);
    ui->antiAllergyTempBox->setCurrentIndex(0);

}


void MainWindow::on_nonStopBackButton_clicked()
{
    ui->nonStopDryBox->setCurrentIndex(0);
    ui->nonStopHourLabel->setText("3");
    ui->nonStopEcoModeBox->setCurrentIndex(0);
    ui->nonStopSpinBox->setCurrentIndex(0);
    ui->nonStopTempBox->setCurrentIndex(0);
}


void MainWindow::on_antiCreaseBackButton_clicked()
{
    ui->antiCreaseEcoModeBox->setCurrentIndex(0);
    ui->antiCreaseExtraBox->setCurrentIndex(0);
    ui->antiCreaseHourLabel->setText("0.50");
    ui->antiCreaseSpinBox->setCurrentIndex(0);
    ui->antiCreaseTempBox->setCurrentIndex(0);
}


void MainWindow::on_refreshBackButt_clicked()
{
    ui->refreshEcoModeBox->setCurrentIndex(0);
    ui->refreshSpinBox->setCurrentIndex(0);
    ui->refreshHourLabel->setText("0.20");
    ui->refreshSteamBox->setCurrentIndex(0);
    ui->refreshTempBox->setCurrentIndex(0);
}

