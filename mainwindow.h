#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QQuaternion>
#include <QVector3D>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <qpushbutton.h>
#include <QLabel>
#include <QSlider>
#include <functional>
#include <QComboBox>

#include "myhashtable.h"
class SceneModifier;
struct ProgramFill {
    float waterTank;      // 0.0–1.0 fraction of full capacity
    float lintTrap;       // 0.0–1.0
    float detergentTray;  // 0.0–1.0
    int   maxTankRuns;
    int   maxLintRuns;
    int   maxDetRuns;
};

extern int waterRuns;
    extern int lintRuns;
    extern int detRuns;

// Then in your .cpp where you initialize:
extern const QMap<QString,ProgramFill> programFills;
extern float currentWaterTarget;
extern QString currentProgram;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
namespace Qt3DCore {
class QTransform;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void addFunc();

    void insertPowder_button();

    void on_cottonsButton_clicked();

    void on_cottonsEcoButton_clicked();

    void on_syntheticsButton_clicked();

    void on_woolSilkButton_clicked();

    void on_antiAllergyButton_clicked();

    void on_nonStopButton_clicked();

    void on_antiCreaseButton_clicked();

    void on_refreshButton_clicked();

    void on_cottonsTempBox_currentTextChanged(const QString &arg1);

    void on_spinBox_currentTextChanged(const QString &arg1);

    void on_cottonsRinseBox_currentTextChanged(const QString &arg1);

    void on_cottonsEcoModeBox_currentTextChanged(const QString &arg1);
    float calculateEndtime();

    void on_cottonsEcoTempBox_currentTextChanged(const QString &arg1);

    void on_cottonsEcoSpinBox_currentTextChanged(const QString &arg1);

    void on_cottonsEcoRinseBox_currentTextChanged(const QString &arg1);

    void on_cottonsEcoEcoModeBox_currentTextChanged(const QString &arg1);

    void on_cottonsEcoStartButt_clicked();


    void on_synthTempBox_currentTextChanged(const QString &arg1);

    void on_synthSpinBox_currentTextChanged(const QString &arg1);

    void on_synthRinseBox_currentTextChanged(const QString &arg1);

    void on_synthEcoModeBox_currentTextChanged(const QString &arg1);

    void on_synthStartButt_clicked();


    void on_woolSilkTempBox_currentTextChanged(const QString &arg1);

    void on_woolSilkSpinBox_currentTextChanged(const QString &arg1);

    void on_woolSilkRinseBox_currentTextChanged(const QString &arg1);

    void on_woolSilkEcoModeBox_currentTextChanged(const QString &arg1);

    void on_woolSilkStartButt_clicked();


    void on_antiAllergyTempBox_currentTextChanged(const QString &arg1);

    void on_antiAllergySpinBox_currentTextChanged(const QString &arg1);

    void on_antiAllergyRinseBox_currentTextChanged(const QString &arg1);

    void on_antiAllergyEcoModeBox_currentTextChanged(const QString &arg1);

    void on_antiAllergySteamBox_currentTextChanged(const QString &arg1);

    void on_antiAllergyStartButt_clicked();


    void on_nonStopTempBox_currentTextChanged(const QString &arg1);

    void on_nonStopSpinBox_currentTextChanged(const QString &arg1);

    void on_nonStopDryBox_currentTextChanged(const QString &arg1);

    void on_nonStopEcoModeBox_currentTextChanged(const QString &arg1);

    void on_nonStopStartButt_clicked();


    void on_antiCreaseTempBox_currentTextChanged(const QString &arg1);

    void on_antiCreaseSpinBox_currentTextChanged(const QString &arg1);

    void on_antiCreaseExtraBox_currentTextChanged(const QString &arg1);

    void on_antiCreaseEcoModeBox_currentTextChanged(const QString &arg1);

    void on_antiCreaseStartButt_clicked();


    void on_refreshTempBox_currentTextChanged(const QString &arg1);

    void on_refreshSpinBox_currentTextChanged(const QString &arg1);

    void on_refreshSteamBox_currentTextChanged(const QString &arg1);

    void on_refreshEcoModeBox_currentTextChanged(const QString &arg1);

    void on_refreshStartButt_clicked();


    void on_cottonsStartButt_clicked();


    void backToPrograms(std::function<void()> callback);

    void on_cottonsEcoBackButton_clicked();

    void on_cottonsBackButton_clicked();

    void on_syntheticsBackButton_clicked();

    void on_woolSilkBackButton_clicked();

    void on_antiAllergyBackButton_clicked();

    void on_nonStopBackButton_clicked();

    void on_antiCreaseBackButton_clicked();

    void on_refreshBackButt_clicked();

    void cleanLintTrap();

    void onCycleFinished();

    void cleanWaterTank();

    void cleanDetergentTray();

    // water filling things
    void startSlider(QLabel*& label, QLabel* holderLabel, QMovie *& spinnerMovie,QTimer* timer,QElapsedTimer* elapsedTimer,std::function<void()>callback = []() {});

    void moveWaterFillingSlider();
    void moveHeatingWaterSlider();
    void moveWashCycleSlider();
    void moveDrainWaterSlider();
    void moveRinseWaterSlider();
    void moveSpinWaterSlider();
    void moveSteamWaterSlider();
    void movedryWaterSlider();
    void moveAntiCreaseWaterSlider();

    void prepareHeatingFunction();
    void prepareDrainingFunction();
    void prepareSpinFunction();
    void prepareFillingWaterFunction();
    void prepareWashWaterFunction();
    void prepareRinseWaterFunction();
    void prepareSteamWaterFunction();
    void prepareDryWaterFunction();
    void prepareAntiCreaseWaterFunction();

    void calculateIncrementationForSliders(QSlider* slider,double time,double& current,int& max,int& steps, double&increment, int targetValue = 0);
    void calculateDecrementationForSliders(QSlider* slider, double time);

    void calculateTimedifference(int defaultTime, double& timeslider, int numberOfPrograms ,int time = 0);

    int convertMinutesToCompressedSeconds(int minutes, double compressionFactor);
    QString convertToNormalTime(int seconds, double compressionFactor);
    int convertAndCompressTime(double hourFloat, double compressionFactor);

    QString formatSecondsToHourMinute(int seconds);

    void setSliderAttributesSuccess(QTimer*& timer, QMovie*& movie, QLabel*& label,QLabel*& holderLabel);
    void clearSlider(QTimer*& timer, QMovie*& movie, QLabel*& label,QLabel*& holderLabel);


    void setProgramLabelTime(double time);

    void distributeSecondsForCottons(double& mainSlider);
    void distributeEcoTimeForCottons();
    void distributeEcoTimeForCottonsEco();
    void distributeEcoTimeForAntiAllergy();
    void distributeEcoTimeForNonStop();
    void distributeEcoTimeForAntiCrease();
    void distributeEcoTimeForRefresh();

    void setSlidersToZero();
    void stopTimersAndOthers();
    void setTempSliderMinMax(QComboBox*& box);

    void on_emergencyStopButton_clicked();

private:
    SceneModifier *modifier;  // Add this line

    // Add these declarations
    void startDetergentFilling();
    void startSoftenerFilling();


    Ui::MainWindow *ui;
    QTimer *detergentTimer = nullptr;
    QTimer *softenerTimer = nullptr;
    int detergentValue = 0;
    int softenerValue = 0;
    Qt3DCore::QTransform *modelTransform = nullptr;
    QWidget *overlay = nullptr;

    // this will hold labels transforms, root Transofrom, and their first coordinates , so we can go back and revert coordinates
    MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *colorTable= nullptr;

    QWidget *container = nullptr;

    QString emergencyProgramStop;


    // times for sliders fo how long to be filling depending on user options
    double fillWaterTimeSlider;
    double heatWaterTimeSlider;
    double washCycleTimeSlider;
    double drainWaterTimeSlider;
    double rinseCycleTimeSlider;
    double spinCycleTimeSlider;
    double steamTimeSlider;
    double preWashTimeSlider;
    double antiCreaseTimeSlider;
    double dryWaterTimeSlider;

    double fillingWaterCurrentValueForIncrement;
    double currentValueSliderForIncrement;
    double currentValueSliderForDecrement;

    int maxfillingWaterValueSlider;
    int maxValueSlider;
    int minValueSlider;

    int fillingWaterStepsForSlider;
    int stepsForSlider;

    double incrementStepSlider;
    double fillingWaterincrementStepSlider;
    double decrementStepSlider;

    // for cottons steps
    /*FillWater → HeatWater → WashCycle → DrainWater → RinseCycle → DrainWater → SpinCycle → Complete*/
    int cottonsStep = 0;
    bool isAntiAllergy = false;
    bool isnonstopStep = false;
    bool isAntiCrease = false;
    bool isRefresh = false;


    // variables useful for calculations of the endtime
    int tempTime = 0;
    int spinTime = 0;
    int ecoTime = 0;
    int rinseTime = 0;
    int steamTime = 0;
    int dryTime = 0;
    int anticreaseTime = 0;

    float endTime = 2.0f;

    // timers for sliders, labels,movie
    bool fillingWaterisIncrementing = true;
    QTimer* waterFillingSliderTimer = nullptr;
    QElapsedTimer *waterFillingElapsedTimer = nullptr;
    QLabel * waterFillingLabel = nullptr;
    QMovie * waterFillingSpinnerMovie = nullptr;


    // for heating
    bool heatingWaterIsIncrementing = true;
    QTimer* heatingWaterSliderTimer = nullptr;
    QElapsedTimer *heatingWaterElapsedTimer = nullptr;
    QLabel* heatingWaterLabel = nullptr;
    QMovie* heatingWaterSpinnerMovie = nullptr;
    QComboBox* currentBox = nullptr;

    // for washCycle
    bool washCycleIsIncrementing = true;
    QTimer* washWaterSliderTimer = nullptr;
    QElapsedTimer *washWaterElapsedTimer = nullptr;
    QLabel* washWaterLabel = nullptr;
    QMovie* washWaterSpinnerMovie = nullptr;

    // for draining water cycle
    bool drainingIsIncrementing = true;
    QTimer* drainWaterSliderTimer = nullptr;
    QElapsedTimer *drainWaterElapsedTimer = nullptr;
    QLabel* drainWaterLabel = nullptr;
    QMovie* drainWaterSpinnerMovie = nullptr;

    // for rinse water cycle
    bool rinseIsIncrementing = true;
    QTimer* rinseWaterSliderTimer = nullptr;
    QElapsedTimer *rinseWaterElapsedTimer = nullptr;
    QLabel* rinseWaterLabel = nullptr;
    QMovie* rinseWaterSpinnerMovie = nullptr;

    // for spin water cycle
    bool spinIsIncrementing = true;
    QTimer* spinWaterSliderTimer = nullptr;
    QElapsedTimer *spinWaterElapsedTimer = nullptr;
    QLabel* spinWaterLabel = nullptr;
    QMovie* spinWaterSpinnerMovie = nullptr;


    // for steam
    QTimer* steamWaterSliderTimer = nullptr;
    QElapsedTimer * steamWaterElapsedTimer = nullptr;
    QLabel * steamWaterLabel = nullptr;
    QMovie* steamWaterSpinnerMovie = nullptr;

    // for drying
    QTimer* dryWaterSliderTimer = nullptr;
    QElapsedTimer * dryWaterElapsedTimer = nullptr;
    QLabel * dryWaterLabel = nullptr;
    QMovie* dryWaterSpinnerMovie = nullptr;

    // for antiCrease
    QTimer* antiCreaseWaterSliderTimer = nullptr;
    QElapsedTimer * antiCreaseWaterElapsedTimer = nullptr;
    QLabel * antiCreaseWaterLabel = nullptr;
    QMovie* atniCreaseWaterSpinnerMovie = nullptr;


protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
