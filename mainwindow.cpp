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
#include <QElapsedTimer>

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


const QMap<QString,ProgramFill> programFills {
    { "Cottons",     { 1.00f, 0.50f, 0.20f, 10, 20, 50 } },
    { "CottonsEco",  { 0.90f, 0.40f, 0.25f, 10, 25, 40 } },
    { "Synthetics",  { 0.80f, 0.35f, 0.15f, 12, 30, 60 } },
    { "Wool/Silk",   { 0.60f, 0.25f, 0.10f, 15, 40, 80 } },
    { "AntiAllergy", { 1.00f, 0.60f, 0.40f, 10, 15, 25 } },
    { "NonStop",     { 0.70f, 0.30f, 0.20f, 14, 28, 56 } },
    { "AntiCrease",  { 0.50f, 0.20f, 0.10f, 20, 50, 100 } },
    { "Refresh",     { 0.40f, 0.15f, 0.05f, 25, 60, 200 } }
};

int waterRuns = 0;
int lintRuns  = 0;
int detRuns   = 0;

QString currentProgram = "";

#include <functional>



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
    camera->setPosition(QVector3D(5.0f, 3.0f, 10.0f));  // Diagonal view
    camera->setViewCenter(QVector3D(0, -1.0f, 0));


    // Load 3D Model
    modelTransform = new Qt3DCore::QTransform();
    view->setTargetTransform(modelTransform);
    Qt3DRender::QSceneLoader *loader = new Qt3DRender::QSceneLoader(rootEntity);
    loader->setSource(QUrl::fromLocalFile("C:\\Users\\User\\washingMachineSimulator\\assets\\scene.gltf"));

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

    ui->fillingWaterSlider->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->heatingWaterSlider->setAttribute(Qt::WA_TransparentForMouseEvents);


    // lets place drawer label
     modifier = new SceneModifier(rootEntity);
    // Add progress bars to the scene
    modifier->addProgressBars();

    // Initialize levels to 0
    modifier->updateProgressBars(0, 0, 0);
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

    connect(ui->antiCreaseBackButton, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_antiCreaseBackButton_clicked();});
    });

    connect(ui->refreshBackButt, &QPushButton::clicked,this,[=]() {
        backToPrograms([=]() {on_refreshBackButt_clicked();});
    });

    connect(ui->cleanWaterButton,     &QPushButton::clicked, this, &MainWindow::cleanWaterTank);
    connect(ui->cleanLintButton,      &QPushButton::clicked, this, &MainWindow::cleanLintTrap);
    connect(ui->cleanDetergentButton, &QPushButton::clicked, this, &MainWindow::cleanDetergentTray);


    // timers connecting
    waterFillingSliderTimer = new QTimer(this);
    waterFillingElapsedTimer = new QElapsedTimer();
    connect(waterFillingSliderTimer, &QTimer::timeout, this, &MainWindow::moveWaterFillingSlider);

    heatingWaterSliderTimer = new QTimer(this);
    heatingWaterElapsedTimer =  new QElapsedTimer();
    connect(heatingWaterSliderTimer , &QTimer::timeout, this, &MainWindow::moveHeatingWaterSlider);

    washWaterSliderTimer = new QTimer(this);
    washWaterElapsedTimer =  new QElapsedTimer();
    connect(washWaterSliderTimer , &QTimer::timeout, this, &MainWindow::moveWashCycleSlider);

    drainWaterSliderTimer = new QTimer(this);
    drainWaterElapsedTimer =  new QElapsedTimer();
    connect(drainWaterSliderTimer, &QTimer::timeout , this, &MainWindow::moveDrainWaterSlider);

    rinseWaterSliderTimer = new QTimer(this);
    rinseWaterElapsedTimer =  new QElapsedTimer();
    connect(rinseWaterSliderTimer, &QTimer::timeout, this,&MainWindow::moveRinseWaterSlider);


    spinWaterSliderTimer = new QTimer(this);
    spinWaterElapsedTimer =  new QElapsedTimer();
    connect(spinWaterSliderTimer, &QTimer::timeout, this, &MainWindow::moveSpinWaterSlider);

    steamWaterSliderTimer = new QTimer(this);
    steamWaterElapsedTimer = new QElapsedTimer();
    connect(steamWaterSliderTimer, &QTimer::timeout, this, &MainWindow::moveSteamWaterSlider);

    dryWaterSliderTimer = new QTimer(this);
    dryWaterElapsedTimer = new QElapsedTimer();
    connect(dryWaterSliderTimer, &QTimer::timeout, this, &MainWindow::movedryWaterSlider);

    antiCreaseWaterSliderTimer = new QTimer(this);
    antiCreaseWaterElapsedTimer = new QElapsedTimer();
    connect(antiCreaseWaterSliderTimer, &QTimer::timeout, this, &MainWindow::moveAntiCreaseWaterSlider);


    // when we start the program we need to collect first the program name and all the comboboxes
    // then reset everything


    modifier->addProgressBars();
    modifier->updateProgressBars(0.0f, 0.0f, 0.0f);



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


double addHourFloats(double time1, double time2){
    int hours1 = static_cast<int>(time1);
    int minutes1 = static_cast<int>((time1 - hours1) * 100);

    int hours2 = static_cast<int>(time2);
    int minutes2 = static_cast<int>((time2 - hours2) * 100);

    int totalMinutes = (hours1 * 60 + minutes1) + (hours2 * 60 + minutes2);


    int finalHours = totalMinutes / 60;
    int finalMinutes = totalMinutes / 60;
    return finalHours + (finalMinutes / 100.0);
}

double subtractHourFloats(double time1, double time2){
    int hours1 = static_cast<int>(time1);
    int minutes1 = static_cast<int>((time1 - hours1) * 100);

    int hours2 = static_cast<int>(time2);
    int minutes2 = static_cast<int>((time2 - hours2) * 100);

    int totalMinutes = (hours1 * 60 + minutes1) - (hours2 * 60 + minutes2);


    int finalHours = totalMinutes / 60;
    int finalMinutes = totalMinutes / 60;
    return finalHours + (finalMinutes / 100.0);
}


// TIMER FUNCTIONS

void MainWindow::calculateIncrementationForSliders(QSlider* slider, double time, double& current,int& max,int& steps, double&increment,int targetValue){
    current = slider->value();
    max = targetValue == 0? slider->maximum():targetValue;
    steps = time * 10;
    increment = static_cast<double>(max - current )/ steps;
}

void MainWindow::calculateDecrementationForSliders(QSlider* slider, double time){
    currentValueSliderForDecrement = slider->value();        // starting from current value
    minValueSlider = slider->minimum();          // target is min (usually 0)
    stepsForSlider = time *10;        // number of timer ticks
    decrementStepSlider = static_cast<double>(currentValueSliderForDecrement - minValueSlider) / stepsForSlider;
}

int hourFloatToMinutes(double hourFloat){
    int hours = static_cast<int>(hourFloat);
    int minutes = static_cast<int>((hourFloat - hours) * 100);
    return (hours * 60) + minutes;
}

void MainWindow::setProgramLabelTime(double time){
    int programTimeMins = hourFloatToMinutes(ui->programStartedHourLabel->text().toFloat());
    int reductionMinutes = hourFloatToMinutes(convertToNormalTime(time,60.0).toFloat());
    int resultMins = programTimeMins - reductionMinutes;
    int resultHours = resultMins / 60;
    int resultMinutes =resultMins % 60;
    QString formattedResult = QString("%1.%2").arg(resultHours).arg(resultMinutes,2,10,QChar('0'));
    ui->programStartedHourLabel->setText(formattedResult);

}

void MainWindow::setSliderAttributesSuccess(QTimer*& timer, QMovie*& movie, QLabel*& label,QLabel*& holderLabel){
    timer->stop();
    movie->stop();
    label->hide();
    holderLabel->setText("✔");
}

void MainWindow::clearSlider(QTimer*& timer, QMovie*& movie, QLabel*& label,QLabel*& holderLabel){
    if(timer != nullptr)timer->stop();
    if(movie != nullptr)movie->stop();
    if(label != nullptr)label->hide();
    holderLabel->setText("");
}


void MainWindow::moveWaterFillingSlider(){
    //fillingWaterCurrentValueForIncrement,maxfillingWaterValueSlider,fillingWaterStepsForSlider,fillingWaterCurrentValueForIncrement
    int percentage ;
    int stop = false;
    if(fillingWaterisIncrementing){
        fillingWaterCurrentValueForIncrement += fillingWaterincrementStepSlider;
        percentage = (fillingWaterCurrentValueForIncrement * 100) / maxfillingWaterValueSlider;
        if(fillingWaterCurrentValueForIncrement < maxfillingWaterValueSlider){
            ui->fillingWaterSlider->setValue(static_cast<int>(fillingWaterCurrentValueForIncrement));
        }else stop = true;
    }else
    {
        currentValueSliderForDecrement -= decrementStepSlider;
        percentage = (currentValueSliderForDecrement * 100) / maxValueSlider;
        if(currentValueSliderForDecrement> minValueSlider){
            ui->fillingWaterSlider->setValue(static_cast<int>(currentValueSliderForDecrement));
        }else stop = true;
    }
    ui->fillingWaterLabel->setText("Filling Water " + QString::number(percentage) + " %");


    if(stop){
        if(fillingWaterisIncrementing){
            ui->fillingWaterSlider->setValue(100);
            ui->fillingWaterLabel->setText("Filling Water 100%");
            // increment only on the first step

        }
        else {
            ui->fillingWaterSlider->setValue(0);
            ui->fillingWaterLabel->setText("Filling Water 0%");

        }
        setSliderAttributesSuccess(waterFillingSliderTimer,waterFillingSpinnerMovie,waterFillingLabel,ui->fillingWaterSpinnerHolderLabel);
        // starting heating water
        if(cottonsStep == 1){
            cottonsStep++;
            setProgramLabelTime(fillWaterTimeSlider);
            startSlider(heatingWaterLabel,ui->heatingWaterSpinningHolderLabel,heatingWaterSpinnerMovie,heatingWaterSliderTimer,heatingWaterElapsedTimer,std::bind(&MainWindow::prepareHeatingFunction,this));
        }
    }
}


void MainWindow::startDetergentFilling() {
    auto fill = programFills.value(currentProgram);
    int target = int(100 * fill.detergentTray);
    detergentValue = 0;
    detergentTimer = new QTimer(this);
    connect(detergentTimer, &QTimer::timeout, [=]() mutable {
        if (detergentValue < target) {
            detergentValue++;
        } else {
            detergentTimer->stop();
            startSoftenerFilling();
        }
    });
    detergentTimer->start(50);
}


void MainWindow::startSoftenerFilling() {
    softenerValue = 0;
    softenerTimer = new QTimer(this);
    connect(softenerTimer, &QTimer::timeout, [this]() {
        if(softenerValue < 100) {
            softenerValue++;
        } else {
            softenerTimer->stop();
            waterFillingSpinnerMovie->stop();
            waterFillingLabel->hide();
            ui->fillingWaterSpinnerHolderLabel->setText("✔");
            startSlider(heatingWaterLabel,ui->heatingWaterSpinningHolderLabel,
                        heatingWaterSpinnerMovie,heatingWaterSliderTimer);
            onCycleFinished();
        }
    });
    softenerTimer->start(50);
}

void MainWindow::moveHeatingWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    ui->heatingWaterLabel->setText("Heating Water " + QString::number((int)currentValueSliderForIncrement) + "°C");
    if(currentValueSliderForIncrement< maxValueSlider){
        ui->heatingWaterSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->heatingWaterLabel->setText("Heating Water " + QString::number(currentBox->currentText().toInt()));
        ui->heatingWaterSlider->setValue(100);
        setProgramLabelTime(heatWaterTimeSlider);
        setSliderAttributesSuccess(heatingWaterSliderTimer,heatingWaterSpinnerMovie ,heatingWaterLabel,ui->heatingWaterSpinningHolderLabel);
        // starting wash cycle
        if(cottonsStep == 2){
            cottonsStep++;
            startSlider(washWaterLabel,ui->washCycleSpinningHolderLabel,washWaterSpinnerMovie,washWaterSliderTimer,washWaterElapsedTimer,std::bind(&MainWindow::prepareWashWaterFunction,this));
        }
        // this is for the refresh program
        else if (isRefresh){
            // need to check if we have directly spin or before that steam
            if(ui->refreshSteamBox->currentText() == "Enabled"){
                // start steaming
                startSlider(steamWaterLabel,ui->steamSpinningHolderLabel,steamWaterSpinnerMovie,steamWaterSliderTimer,steamWaterElapsedTimer,std::bind(&MainWindow::prepareSteamWaterFunction,this));

            }else {
                // start spinning directly
                startSlider(spinWaterLabel,ui->spinCycleSpinningHolderLabel,spinWaterSpinnerMovie,spinWaterSliderTimer,spinWaterElapsedTimer,std::bind(&MainWindow::prepareSpinFunction,this));

            }
        }
    }
}
void MainWindow::moveWashCycleSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->washCycleLabel->setText("Washing " + QString::number(percentage) + " %");
    if(currentValueSliderForIncrement < maxValueSlider){
        ui->washCycleSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->washCycleSlider->setValue(100);
        ui->washCycleLabel->setText("Washing 100%");
        setProgramLabelTime(washCycleTimeSlider);
        setSliderAttributesSuccess(washWaterSliderTimer,washWaterSpinnerMovie,washWaterLabel, ui->washCycleSpinningHolderLabel);
        if(cottonsStep == 3){
            cottonsStep++;
            fillingWaterisIncrementing = false;
            startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
            startSlider(drainWaterLabel,ui->drainWaterSpinningHolderLabel,drainWaterSpinnerMovie,drainWaterSliderTimer,drainWaterElapsedTimer,std::bind(&MainWindow::prepareDrainingFunction,this));
        }
    }
}

// draining - filling water going down -> so we need to drain until the slider in the filling is down

void MainWindow::moveDrainWaterSlider(){
    // while there is draining we need also to remove the fill water
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->drainWaterLabel->setText("Draining " + QString::number(percentage) + " %");
    if(currentValueSliderForIncrement < maxValueSlider){
        ui->drainWaterSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->drainWaterSlider->setValue(100);
        ui->drainWaterLabel->setText("Draining 100%");
        setSliderAttributesSuccess(drainWaterSliderTimer,drainWaterSpinnerMovie, drainWaterLabel,ui->drainWaterSpinningHolderLabel);
        if(cottonsStep == 4){
            cottonsStep++;
            setProgramLabelTime(drainWaterTimeSlider);
            fillingWaterisIncrementing = true;
            startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
            startSlider(rinseWaterLabel,ui->rinseCycleSpinningHolderLabel,rinseWaterSpinnerMovie, rinseWaterSliderTimer,rinseWaterElapsedTimer, std::bind(&MainWindow::prepareRinseWaterFunction,this));
        }else if (isAntiAllergy){
            startSlider(steamWaterLabel, ui->steamSpinningHolderLabel, steamWaterSpinnerMovie,steamWaterSliderTimer,steamWaterElapsedTimer, std::bind(&MainWindow::prepareSteamWaterFunction,this));
        }
        else if (cottonsStep == 6){
            // run spin Cycle
            startSlider(spinWaterLabel, ui->spinCycleSpinningHolderLabel,spinWaterSpinnerMovie,spinWaterSliderTimer,spinWaterElapsedTimer, std::bind(&MainWindow::prepareSpinFunction, this));
        }
    }

}

void MainWindow::moveSteamWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->steamLabel->setText("Steam Treatment " + QString::number(percentage) + "%");
    if(currentValueSliderForIncrement< maxValueSlider){
        ui->steamSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->steamLabel->setText("Steam Treatment 100%");
        ui->steamSlider->setValue(100);
        setProgramLabelTime(steamTimeSlider);
        setSliderAttributesSuccess(steamWaterSliderTimer,steamWaterSpinnerMovie,steamWaterLabel,ui->steamSpinningHolderLabel);
        // starting wash cycle
        startSlider(spinWaterLabel, ui->spinCycleSpinningHolderLabel,spinWaterSpinnerMovie,spinWaterSliderTimer,spinWaterElapsedTimer, std::bind(&MainWindow::prepareSpinFunction, this));

    }
}

void MainWindow::moveRinseWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->rinseCycleLabel->setText("Rinsing " + QString::number(percentage) + " %");
    if(currentValueSliderForIncrement< maxValueSlider){
        ui->rinseCycleSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->rinseCycleSlider->setValue(100);
        ui->rinseCycleLabel->setText("Rinsing 100%");
        setProgramLabelTime(rinseCycleTimeSlider);
        setSliderAttributesSuccess(rinseWaterSliderTimer,rinseWaterSpinnerMovie,rinseWaterLabel, ui->rinseCycleSpinningHolderLabel);

        // stop aslo the filling water
        cottonsStep++;
        // drain water again (before that we need to reset the drain water slider)
        fillingWaterisIncrementing = false;
        startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
        startSlider(drainWaterLabel,ui->drainWaterSpinningHolderLabel,drainWaterSpinnerMovie,drainWaterSliderTimer,drainWaterElapsedTimer,std::bind(&MainWindow::prepareDrainingFunction,this));

    }
}

void MainWindow::movedryWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->dryLabel->setText("Drying " + QString::number(percentage) + " %");
    if(currentValueSliderForIncrement< maxValueSlider){
        ui->dryslider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->dryslider->setValue(100);
        ui->dryLabel->setText("Rinsing 100%");
        setProgramLabelTime(dryWaterTimeSlider);
        setSliderAttributesSuccess(dryWaterSliderTimer,dryWaterSpinnerMovie,dryWaterLabel, ui->dryCycleSpinningHolderLabel);

    }
}

void MainWindow::moveAntiCreaseWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    int percentage = (currentValueSliderForIncrement * 100) / maxValueSlider;
    ui->antiCreaseLabel->setText("AntiCrease " + QString::number(percentage) + " %");
    if(currentValueSliderForIncrement< maxValueSlider){
        ui->antiCreaseSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        ui->antiCreaseSlider->setValue(100);
        ui->antiCreaseLabel->setText("Rinsing 100%");
        setProgramLabelTime(dryWaterTimeSlider);
        setSliderAttributesSuccess(antiCreaseWaterSliderTimer,atniCreaseWaterSpinnerMovie,antiCreaseWaterLabel, ui->antiCreaseSpinningHolderLabel);
    }
}


// prepare spin

void MainWindow::moveSpinWaterSlider(){
    currentValueSliderForIncrement += incrementStepSlider;
    // here it needs to start spinning till it reaches full capacity of spin, ant then hold that for a couple of seconds, so basically we need to cut the seconds in half, one for maxing the spin and the other
    if(currentValueSliderForIncrement <maxValueSlider){
        ui->spinCycleSlider->setValue(static_cast<int>(currentValueSliderForIncrement));
    }else {
        QTimer::singleShot(1000, this, [this]() {});
        setProgramLabelTime(spinCycleTimeSlider);
        setSliderAttributesSuccess(spinWaterSliderTimer,spinWaterSpinnerMovie,spinWaterLabel, ui->spinCycleSpinningHolderLabel);

        if(isnonstopStep){
            startSlider(dryWaterLabel,ui->dryCycleSpinningHolderLabel,dryWaterSpinnerMovie,dryWaterSliderTimer,dryWaterElapsedTimer,std::bind(&MainWindow::prepareDryWaterFunction, this));

        }else if(isAntiCrease){
            startSlider(antiCreaseWaterLabel,ui->antiCreaseSpinningHolderLabel,atniCreaseWaterSpinnerMovie,antiCreaseWaterSliderTimer,antiCreaseWaterElapsedTimer,std::bind(&MainWindow::prepareAntiCreaseWaterFunction, this));

        }
    }
}

void MainWindow::setTempSliderMinMax(QComboBox*& box){
    QString temp = box->currentText();
    ui->heatingWaterSlider->setMinimum(0);
    if(temp == "Cold"){
        ui->heatingWaterSlider->setMaximum(0);
    }else ui->heatingWaterSlider->setMaximum(temp.left(2).toInt());
    currentBox = box;
}


// FUNCTIONS FOR DIFFERENT SLIDER -- PREPARING FUNCS
void MainWindow::prepareHeatingFunction(){
    calculateIncrementationForSliders(ui->heatingWaterSlider,heatWaterTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);
}
void MainWindow::prepareSteamWaterFunction(){
    calculateIncrementationForSliders(ui->steamSlider,steamTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);
}
void MainWindow::prepareFillingWaterFunction(){
    if(fillingWaterisIncrementing){
        calculateIncrementationForSliders(ui->fillingWaterSlider,fillWaterTimeSlider,fillingWaterCurrentValueForIncrement,maxfillingWaterValueSlider,fillingWaterStepsForSlider,fillingWaterincrementStepSlider);

    }else {
        calculateDecrementationForSliders(ui->fillingWaterSlider,fillWaterTimeSlider);
    }
}
void MainWindow::prepareDrainingFunction(){
    ui->drainWaterSlider->setValue(0);
    calculateIncrementationForSliders(ui->drainWaterSlider, drainWaterTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);
}
void MainWindow::prepareSpinFunction(){
    ui->drainWaterSlider->setValue(0);
    QString text = ui->spinBox->currentText();
    int num = text.left(text.length() - 3).toInt();
    ui->spinCycleSlider->setRange(100,1700);
    calculateIncrementationForSliders(ui->spinCycleSlider, spinCycleTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider,num);
}
void MainWindow::prepareRinseWaterFunction(){
    calculateIncrementationForSliders(ui->rinseCycleSlider,rinseCycleTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);
}
void MainWindow::prepareWashWaterFunction(){
    calculateIncrementationForSliders(ui->washCycleSlider, washCycleTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);
}
void MainWindow::prepareDryWaterFunction(){
    calculateIncrementationForSliders(ui->dryslider, dryWaterTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);

}


void MainWindow::prepareAntiCreaseWaterFunction(){
    calculateIncrementationForSliders(ui->antiCreaseSlider, antiCreaseTimeSlider,currentValueSliderForIncrement,maxValueSlider,stepsForSlider,incrementStepSlider);

}


// PENDING CIRCLE FUNCTION
void MainWindow::startSlider(QLabel*& label, QLabel* holderLabel, QMovie *& spinnerMovie,QTimer* timer,QElapsedTimer* elapsedTimer,std::function<void()>callback){
    if(!label){
        label = new QLabel(holderLabel);
        label->setGeometry(0,0,holderLabel->width(),holderLabel->height());
        label->setAlignment(Qt::AlignCenter);

        // Load the spinner movie
        spinnerMovie = new QMovie("C:\\Users\\vboxuser\\Documents\\managementsystem\\assets\\Spinner@1x-1.0s-60px-60px (1).gif");
        label->setMovie(spinnerMovie);
    }
    // show the label
    label->show();
    holderLabel->setText("");
    spinnerMovie->start();

    // start filling the slider
    callback();
    timer->start(100);
    elapsedTimer->start();
}

// CALCULATE TIME DIFFERENCE
void MainWindow::calculateTimedifference(int defaultTime, double& timeslider, int numberOfPrograms, int tmp1){
    double defaulttimeForProgram =(double)defaultTime /(double)numberOfPrograms;
    int res ;
    if(tmp1!= 0){
        res =convertMinutesToCompressedSeconds(tmp1,60.0);
        timeslider = defaulttimeForProgram+ res;
    }else timeslider = defaulttimeForProgram;
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

void MainWindow::setSlidersToZero(){
    ui->fillingWaterSlider->setValue(0);
    ui->drainWaterSlider->setValue(0);
    ui->heatingWaterSlider->setValue(0);
    ui->antiCreaseSlider->setValue(0);
    ui->steamSlider->setValue(0);
    ui->dryslider->setValue(0);
    ui->spinCycleSlider->setValue(0);
    ui->rinseCycleSlider->setValue(0);
    ui->antiCreaseSlider->setValue(0);
}

void MainWindow::stopTimersAndOthers(){


    clearSlider(waterFillingSliderTimer,waterFillingSpinnerMovie,waterFillingLabel, ui->fillingWaterSpinnerHolderLabel);
    clearSlider(heatingWaterSliderTimer,heatingWaterSpinnerMovie,heatingWaterLabel,ui->heatingWaterSpinningHolderLabel);
    clearSlider(washWaterSliderTimer,washWaterSpinnerMovie,washWaterLabel, ui->washCycleSpinningHolderLabel);
    clearSlider(drainWaterSliderTimer,drainWaterSpinnerMovie,drainWaterLabel, ui->drainWaterSpinningHolderLabel);
    clearSlider(rinseWaterSliderTimer,rinseWaterSpinnerMovie,rinseWaterLabel, ui->rinseCycleSpinningHolderLabel);
    clearSlider(spinWaterSliderTimer,rinseWaterSpinnerMovie,rinseWaterLabel, ui->rinseCycleSpinningHolderLabel);
    clearSlider(steamWaterSliderTimer,steamWaterSpinnerMovie,steamWaterLabel, ui->steamSpinningHolderLabel);
    clearSlider(dryWaterSliderTimer,dryWaterSpinnerMovie,dryWaterLabel, ui->dryCycleSpinningHolderLabel);
    clearSlider(antiCreaseWaterSliderTimer,atniCreaseWaterSpinnerMovie,antiCreaseWaterLabel, ui->antiCreaseSpinningHolderLabel);



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
    // 2.8
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

void MainWindow::distributeSecondsForCottons(double& mainSlider){
    int seconds = 0;
    while(mainSlider < 0){
        if(fillWaterTimeSlider > 0 && drainWaterTimeSlider > 0)
        {
            drainWaterTimeSlider--;
            fillWaterTimeSlider--;
            if(mainSlider < 0)mainSlider++;
            else break;
        }
        if(washCycleTimeSlider > 0) {
            washCycleTimeSlider--;
            if(mainSlider < 0)mainSlider++;
            else break;
        }

    }
}


void MainWindow::distributeEcoTimeForCottons(){
    while(ecoTime > 0){
        if(ecoTime > 0){
            ecoTime --;
            fillWaterTimeSlider++;
        }
        else if(ecoTime > 0){
            ecoTime --;
            heatWaterTimeSlider++;
        }
        else if(ecoTime > 0){
            ecoTime --;
            washCycleTimeSlider++;
        }
        else if(ecoTime > 0){
            ecoTime --;
            drainWaterTimeSlider++;
        }
        else if(ecoTime > 0){
            ecoTime --;
            rinseCycleTimeSlider++;
        }
        else if(ecoTime > 0){
            ecoTime --;
            spinCycleTimeSlider++;
        }

    }
}
void MainWindow::distributeEcoTimeForCottonsEco(){
    bool isIn =false;
    while(ecoTime < 0){
        if(ecoTime < 0 && fillWaterTimeSlider> 0){
            ecoTime++;
            fillWaterTimeSlider--;
            isIn  = true;
        }
        else if(ecoTime < 0 && heatWaterTimeSlider > 0){
            ecoTime ++;
            heatWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && washCycleTimeSlider> 0){
            ecoTime ++;
            washCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && drainWaterTimeSlider>0){
            ecoTime ++;
            drainWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&rinseCycleTimeSlider > 0 ){
            ecoTime ++;
            rinseCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime << 0 &&spinCycleTimeSlider > 0 ){
            ecoTime ++;
            spinCycleTimeSlider--;
            isIn  = true;

        }
        if(!isIn){
            break;
        }
        isIn = false;

    }
}

// the idea is to simulate one min real time = 1 hour simulated
/*2.30 → 2h 30m → 9000 real seconds.

Compressed: 9000 / 60 = 150 simulated seconds.*/
int MainWindow::convertAndCompressTime(double hourFloat, double compressionFactor){
    int hours = static_cast<int>(hourFloat);
    double fractionalPart = hourFloat - hours;
    int minutes = static_cast<int>(fractionalPart * 100);

    int totalSeconds = (hours * 3600) + (minutes * 60);

    // compress
    int simulateSeconds = static_cast<int>(totalSeconds / compressionFactor);
    return simulateSeconds;
}

QString MainWindow::formatSecondsToHourMinute(int seconds){
    int hours = seconds /3600;
    int minutes = (seconds % 3600)/60;
    return QString("%1.%2").arg(hours).arg(minutes,2,10,QChar('0'));
}

QString MainWindow::convertToNormalTime(int seconds, double compressionFactor){
    int totalSeconds = seconds * compressionFactor;
    return formatSecondsToHourMinute(totalSeconds);
}

int MainWindow::convertMinutesToCompressedSeconds(int minutes, double compressionFactor){
    int realSeconds = minutes * 60;
    int simulatedSeconds = static_cast<int>(realSeconds / compressionFactor);
    return simulatedSeconds;
}

void MainWindow::on_cottonsStartButt_clicked()
{
    makeButtonGreen(colorTable->get("3. Start Program"));
    // collect the name of the program
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->cottonsTitle->text();
    QString temp = ui->cottonsTempBox->currentText().left(2);
    QString spin = ui->spinBox->currentText();
    QString rinse = ui->cottonsRinseBox->currentText();
    QString ecoMode = ui->cottonsEcoModeBox->currentText();
    QString overAllTime = ui->cottonsHourLabel->text();
    // calculate time

    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(2.0, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/

    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,7);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,7,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,7);
    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,7);
    // for rinseCycle
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,7,rinseTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,7,spinTime);
    QString normalTime = convertToNormalTime(fillWaterTimeSlider, 60.0);
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider + drainWaterTimeSlider*2 + rinseCycleTimeSlider + spinCycleTimeSlider +ecoTime );
    distributeEcoTimeForCottons();
    // lets fix negative values
    if(heatWaterTimeSlider < 0)distributeSecondsForCottons(heatWaterTimeSlider);
    if(rinseCycleTimeSlider < 0)distributeSecondsForCottons(rinseCycleTimeSlider);
    if(spinCycleTimeSlider < 0)distributeSecondsForCottons(spinCycleTimeSlider);

    setTempSliderMinMax(ui->cottonsTempBox);
    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    ui->minRinseCycle->setText("1");
    ui->maxRinseCycle->setText(rinse);
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // signal that we are dealing with cottons
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");


    ProgramFill pf = programFills.value("Cottons");
    // 2) Drive the 3D bars



    emergencyProgramStop = "cottonsDescPage";
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
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->cottonsEcoTitle->text();
    QString temp = ui->cottonsEcoTempBox->currentText().left(2);
    QString spin = ui->cottonsEcoSpinBox->currentText();
    QString rinse = ui->cottonsEcoRinseBox->currentText();
    QString ecoMode = ui->cottonsEcoEcoModeBox->currentText();
    QString overAllTime = ui->cottonsEcoHourLabel->text();
    // calculate time
    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(3.0, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,7);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,7,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,7);
    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,7);
    // for rinseCycle
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,7,rinseTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,7,spinTime);
    QString normalTime = convertToNormalTime(fillWaterTimeSlider, 60.0);
    setTempSliderMinMax(ui->cottonsEcoTempBox);
    distributeEcoTimeForCottonsEco();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider + drainWaterTimeSlider*2 + rinseCycleTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values
    if(heatWaterTimeSlider < 0)distributeSecondsForCottons(heatWaterTimeSlider);
    if(rinseCycleTimeSlider < 0)distributeSecondsForCottons(rinseCycleTimeSlider);
    if(spinCycleTimeSlider < 0)distributeSecondsForCottons(spinCycleTimeSlider);

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    ui->minRinseCycle->setText("1");
    ui->maxRinseCycle->setText(rinse);
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");

    // create the spinner QLabe;l as a child of spinnerHolderLabel

    // restore the page
    //on_cottonsBackButton_clicked();
    emergencyProgramStop = "cottonsEcoPage";

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
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->synthTitle->text();
    QString temp = ui->synthTempBox->currentText().left(2);
    QString spin = ui->synthSpinBox->currentText();
    QString rinse = ui->synthRinseBox->currentText();
    QString ecoMode = ui->synthEcoModeBox->currentText();
    QString overAllTime = ui->syntheticsHourLabel->text();
    // calculate time
    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(1.0, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,7);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,7,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,7);
    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,7);
    // for rinseCycle
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,7,rinseTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,7,spinTime);
    QString normalTime = convertToNormalTime(fillWaterTimeSlider, 60.0);
    setTempSliderMinMax(ui->synthTempBox);
    distributeEcoTimeForCottons();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider + drainWaterTimeSlider*2 + rinseCycleTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values
    if(heatWaterTimeSlider < 0)distributeSecondsForCottons(heatWaterTimeSlider);
    if(rinseCycleTimeSlider < 0)distributeSecondsForCottons(rinseCycleTimeSlider);
    if(spinCycleTimeSlider < 0)distributeSecondsForCottons(spinCycleTimeSlider);

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    ui->minRinseCycle->setText("1");
    ui->maxRinseCycle->setText(rinse);
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // synth is the same= as cottons
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "syntheticsPage";

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
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->woolsilkTitle->text();
    QString temp = ui->woolSilkTempBox->currentText().left(2);
    QString spin = ui->woolSilkSpinBox->currentText();
    QString rinse = ui->woolSilkRinseBox->currentText();
    QString ecoMode = ui->woolSilkEcoModeBox->currentText();
    QString overAllTime = ui->woolSilkHourLabel->text();
    // calculate time
    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(0.40, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,7);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,7,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,7);
    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,7);
    // for rinseCycle
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,7,rinseTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,7,spinTime);
    QString normalTime = convertToNormalTime(fillWaterTimeSlider, 60.0);
    setTempSliderMinMax(ui->synthTempBox);
    distributeEcoTimeForCottonsEco();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider + drainWaterTimeSlider*2 + rinseCycleTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values
    if(rinseCycleTimeSlider < 0)distributeSecondsForCottons(rinseCycleTimeSlider);

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    ui->minRinseCycle->setText("1");
    ui->maxRinseCycle->setText(rinse);
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // cottons is much the same as this program
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "woolSilkPage";

}



// FOR ANTI-ALLERGY PROGRAM

void MainWindow::distributeEcoTimeForAntiAllergy(){
    bool isIn =false;
    while(ecoTime < 0){
        if(ecoTime < 0 && fillWaterTimeSlider> 0){
            ecoTime++;
            fillWaterTimeSlider--;
            isIn  = true;
        }
        else if(ecoTime < 0 && heatWaterTimeSlider > 0){
            ecoTime ++;
            heatWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && washCycleTimeSlider> 0){
            ecoTime ++;
            washCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && drainWaterTimeSlider>0){
            ecoTime ++;
            drainWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&rinseCycleTimeSlider > 0 ){
            ecoTime ++;
            rinseCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&spinCycleTimeSlider > 0 ){
            ecoTime ++;
            spinCycleTimeSlider--;
            isIn  = true;

        }else if (ecoTime < 0 && steamTimeSlider > 0){
            ecoTime ++;
            steamTimeSlider--;
            isIn  = true;
        }
        if(!isIn){
            break;
        }
        isIn = false;

    }
}

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
    if(arg1 == "On")steamTime= 20;
    else steamTime = 0;

    ui->antiAllergyHourLabel->setText(QString::number( calculateEndtime(), 'f', 2));
}


void MainWindow::on_antiAllergyStartButt_clicked()
{
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->antiAllergyTitle->text();
    QString temp = ui->antiAllergyTempBox->currentText().left(2);
    QString spin = ui->antiAllergySpinBox->currentText();
    QString rinse = ui->antiAllergyRinseBox->currentText();
    QString ecoMode = ui->antiAllergyEcoModeBox->currentText();
    QString overAllTime = ui->antiAllergyHourLabel->text();
    QString steamOption = ui->antiAllergySteamBox->currentText();
    // calculate time
    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(2.0, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,8);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,8,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,8);
    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,8);
    // for rinseCycle
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,8,rinseTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,8,spinTime);
    calculateTimedifference(compressedTime,steamTimeSlider, 8, steamTime);
    setTempSliderMinMax(ui->antiAllergyTempBox);
    distributeEcoTimeForAntiAllergy();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider + drainWaterTimeSlider*2 + rinseCycleTimeSlider + spinCycleTimeSlider + ecoTime + steamTimeSlider);

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    ui->minRinseCycle->setText("1");
    ui->maxRinseCycle->setText(rinse);
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // cottons is much the same as this program
    if(steamOption == "On"){
        isAntiAllergy = true;
    }
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "antiAllergyPage";
}



// NON STOP Program page

void MainWindow::distributeEcoTimeForNonStop(){
    bool isIn =false;
    while(ecoTime < 0){
        if(ecoTime < 0 && fillWaterTimeSlider> 0){
            ecoTime++;
            fillWaterTimeSlider--;
            isIn  = true;
        }
        else if(ecoTime < 0 && heatWaterTimeSlider > 0){
            ecoTime ++;
            heatWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && washCycleTimeSlider> 0){
            ecoTime ++;
            washCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && drainWaterTimeSlider>0){
            ecoTime ++;
            drainWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&rinseCycleTimeSlider > 0 ){
            ecoTime ++;
            rinseCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&spinCycleTimeSlider > 0 ){
            ecoTime ++;
            spinCycleTimeSlider--;
            isIn  = true;

        }else if (ecoTime < 0 && dryWaterTimeSlider > 0){
            ecoTime ++;
            dryWaterTimeSlider--;
            isIn  = true;
        }
        if(!isIn){
            break;
        }
        isIn = false;

    }
}

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
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->nonStopTitle->text();
    QString temp = ui->nonStopTempBox->currentText().left(2);
    QString spin = ui->nonStopSpinBox->currentText();
    QString dry = ui->nonStopDryBox->currentText();
    QString ecoMode = ui->nonStopEcoModeBox->currentText();
    QString overAllTime = ui->nonStopHourLabel->text();
    ui->dryLabelOptions->setText(dry);
    // calculate time
    //  default time FOR COTTONS
    int compressedTime = convertAndCompressTime(3.0, 60.0); // this is compressed time for all programs combined
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,8);
    // for dry
    calculateTimedifference(compressedTime,dryWaterTimeSlider,8,dryTime);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,8,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,8);
    //
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,8);

    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,8);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,8,spinTime);
    setTempSliderMinMax(ui->nonStopTempBox);
    distributeEcoTimeForNonStop();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider+rinseCycleTimeSlider + drainWaterTimeSlider*2 + dryWaterTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // cottons is much the same as this program
    isnonstopStep = true;
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "nonStopPage";
}




// FOR ANTI-CREASE Page

void MainWindow::distributeEcoTimeForAntiCrease(){
    bool isIn =false;
    while(ecoTime < 0){
        if(ecoTime < 0 && fillWaterTimeSlider> 0){
            ecoTime++;
            fillWaterTimeSlider--;
            isIn  = true;
        }
        else if(ecoTime < 0 && heatWaterTimeSlider > 0){
            ecoTime ++;
            heatWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && washCycleTimeSlider> 0){
            ecoTime ++;
            washCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 && drainWaterTimeSlider>0){
            ecoTime ++;
            drainWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&rinseCycleTimeSlider > 0 ){
            ecoTime ++;
            rinseCycleTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&spinCycleTimeSlider > 0 ){
            ecoTime ++;
            spinCycleTimeSlider--;
            isIn  = true;

        }else if (ecoTime < 0 && antiCreaseTimeSlider > 0){
            ecoTime ++;
            antiCreaseTimeSlider--;
            isIn  = true;
        }
        if(!isIn){
            break;
        }
        isIn = false;

    }
}

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
    makeButtonGreen(colorTable->get("3. Start Program"));
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->antiCreaseTitle->text();
    QString temp = ui->antiCreaseTempBox->currentText().left(2);
    QString spin = ui->antiCreaseSpinBox->currentText();
    QString antiCreaseExtra = ui->antiCreaseExtraBox->currentText();
    QString ecoMode = ui->antiCreaseEcoModeBox->currentText();
    QString overAllTime = ui->antiCreaseHourLabel->text();

    // calculate time
    //  default time FOR COTTONS
    int currentdel = 7;
    int compressedTime = convertAndCompressTime(0.50, 60.0); // this is compressed time for all programs combined
    if(antiCreaseExtra == "Enabled"){
        isAntiCrease = true;
        calculateTimedifference(compressedTime,antiCreaseTimeSlider,++currentdel, anticreaseTime);


    }
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    calculateTimedifference(compressedTime,fillWaterTimeSlider,currentdel);
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,currentdel,tempTime);
    // for wash Cycle
    calculateTimedifference(compressedTime,washCycleTimeSlider,currentdel);
    //
    calculateTimedifference(compressedTime,rinseCycleTimeSlider,currentdel);

    // for drainCycle
    calculateTimedifference(compressedTime,drainWaterTimeSlider,currentdel);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,currentdel,spinTime);
    setTempSliderMinMax(ui->antiCreaseTempBox);
    distributeEcoTimeForAntiCrease();
    compressedTime = (fillWaterTimeSlider + heatWaterTimeSlider + washCycleTimeSlider+rinseCycleTimeSlider + drainWaterTimeSlider*2 + antiCreaseTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values


    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // cottons is much the same as this program
    cottonsStep++;
    startSlider(waterFillingLabel,ui->fillingWaterSpinnerHolderLabel,waterFillingSpinnerMovie,waterFillingSliderTimer,waterFillingElapsedTimer,std::bind(&MainWindow::prepareFillingWaterFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "antiCreasePage";
}



// FOR REFRESH Program

void MainWindow::distributeEcoTimeForRefresh(){
    bool isIn =false;
    while(ecoTime < 0){

        if(ecoTime < 0 && heatWaterTimeSlider > 0){
            ecoTime ++;
            heatWaterTimeSlider--;
            isIn  = true;

        }
        else if(ecoTime < 0 &&spinCycleTimeSlider > 0 ){
            ecoTime ++;
            spinCycleTimeSlider--;
            isIn  = true;

        }else if (ecoTime < 0 && steamTimeSlider > 0){
            ecoTime ++;
            steamTimeSlider--;
            isIn  = true;
        }
        if(!isIn){
            break;
        }
        isIn = false;

    }
}

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
    if(arg1 == "400RPM") spinTime = 5;
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
    QString programstertedHourLabel = ui->programStartedHourLabel->text();
    //ui->programStartedHourLabel->setText(ui->cottonsHourLabel->text());
    QString program = ui->antiCreaseTitle->text();
    QString temp = ui->refreshTempBox->currentText().left(2);
    QString spin = ui->refreshSpinBox->currentText();
    QString steam = ui->refreshSteamBox->currentText();
    QString ecoMode = ui->refreshEcoModeBox->currentText();
    QString overAllTime = ui->refreshHourLabel->text();

    // calculate time
    //  default time FOR COTTONS
    int currentdel = 3;
    int compressedTime = convertAndCompressTime(0.20, 60.0); // this is compressed time for all programs combined
    isRefresh= true;
    if(steam == "Enabled"){
        calculateTimedifference(compressedTime,steamTimeSlider,currentdel, steamTime);
    }
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle →*/
    // for fillingWater
    // for heatingWater
    calculateTimedifference(compressedTime,heatWaterTimeSlider,currentdel,tempTime);
    // for spinCycle
    calculateTimedifference(compressedTime,spinCycleTimeSlider,currentdel,spinTime);

    setTempSliderMinMax(ui->refreshTempBox);
    distributeEcoTimeForRefresh();
    compressedTime = (  heatWaterTimeSlider + steamTimeSlider + spinCycleTimeSlider + ecoTime);
    // lets fix negative values

    makeButtonGreen(colorTable->get("3. Start Program"));

    ui->programStartedHourLabel->setText(convertToNormalTime(compressedTime,60.0));
    ui->minTempSlider->setText("0°C");
    ui->maxTempSlider->setText(temp + "°C");
    // and the hours
    ui->startProgramLabel->setText(program + "Started");
    // switch to the page
    // cottons is much the same as this program
    startSlider(heatingWaterLabel,ui->heatingWaterSpinningHolderLabel,heatingWaterSpinnerMovie,heatingWaterSliderTimer,heatingWaterElapsedTimer,std::bind(&MainWindow::prepareHeatingFunction, this));
    switchToPageByName(ui->stackedWidget, "startProgramPage");
    emergencyProgramStop = "refreshPage";
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


void MainWindow::onCycleFinished()
{
    const ProgramFill &pf = programFills.value(currentProgram);

    // Increment run‐counts, clamped to their maxima
    waterRuns = qMin(waterRuns + 1, pf.maxTankRuns);
    lintRuns  = qMin(lintRuns  + 1, pf.maxLintRuns);
    detRuns   = qMin(detRuns   + 1, pf.maxDetRuns);

    // Compute fractional fill = runs / maxRuns
    float fracW = float(waterRuns) / pf.maxTankRuns;
    float fracL = float(lintRuns ) / pf.maxLintRuns;
    float fracD = float(detRuns  ) / pf.maxDetRuns;

    // Update the 3D bars
    modifier->updateProgressBars(fracW, fracL, fracD);
}


void MainWindow::cleanWaterTank()
{
    waterRuns = 0;
    const ProgramFill &pf = programFills.value(currentProgram);
    modifier->updateProgressBars(0.0f,
                                 float(lintRuns)  / pf.maxLintRuns,
                                 float(detRuns )   / pf.maxDetRuns);
}

void MainWindow::cleanLintTrap()
{
    lintRuns = 0;
    const ProgramFill &pf = programFills.value(currentProgram);
    modifier->updateProgressBars(float(waterRuns) / pf.maxTankRuns,
                                 0.0f,
                                 float(detRuns)    / pf.maxDetRuns);
}

void MainWindow::cleanDetergentTray()
{
    detRuns = 0;
    const ProgramFill &pf = programFills.value(currentProgram);
    modifier->updateProgressBars(float(waterRuns) / pf.maxTankRuns,
                                 float(lintRuns)  / pf.maxLintRuns,
                                 0.0f);
}


void MainWindow::on_emergencyStopButton_clicked()
{
    stopTimersAndOthers();
    setSlidersToZero();
    if(emergencyProgramStop == "refreshPage"){
        on_refreshBackButt_clicked();
        isRefresh = false;
        // need to stop the timer and switch to another page and fix the sliders
    }else if(emergencyProgramStop == "antiCreasePage"){
        on_antiCreaseBackButton_clicked();
    }else if(emergencyProgramStop == "nonStopPage"){
        on_nonStopBackButton_clicked();

    }else if(emergencyProgramStop == "antiAllergyPage"){
        on_antiAllergyBackButton_clicked();
    }else if (emergencyProgramStop == "woolSilkPage"){
        on_woolSilkBackButton_clicked();
    }else if (emergencyProgramStop == "syntheticsPage"){
        on_syntheticsBackButton_clicked();
    }else if (emergencyProgramStop == "cottonsDescPage"){
        on_cottonsBackButton_clicked();
    }else if (emergencyProgramStop == "cottonsEcoPage"){
        on_cottonsEcoBackButton_clicked();
    }
    switchToPageByName(ui->stackedWidget, emergencyProgramStop);
}


