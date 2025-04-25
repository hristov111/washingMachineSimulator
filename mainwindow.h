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

#include "myhashtable.h"

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

    // water filling things
    void startSlider(QLabel* label, QLabel* holderLabel, QMovie * spinnerMovie,QTimer* timer);

    void moveWaterFillingSlider();
    void moveHeatingWaterSlider();

private:
    Ui::MainWindow *ui;

    Qt3DCore::QTransform *modelTransform = nullptr;
    QWidget *overlay = nullptr;

    // this will hold labels transforms, root Transofrom, and their first coordinates , so we can go back and revert coordinates
    MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *colorTable= nullptr;

    QWidget *container = nullptr;



    int tempTime = 0;
    int spinTime = 0;
    int ecoTime = 0;
    int rinseTime = 0;
    int steamTime = 0;
    int dryTime = 0;
    int anticreaseTime = 0;

    float endTime = 2.0f;

    // timers for sliders, labels,movie
    QTimer* waterFillingSliderTimer = nullptr;
    QLabel * waterFillingLabel = nullptr;
    QMovie * waterFillingSpinnerMovie = nullptr;

    QTimer* heatingWaterSliderTimer = nullptr;
    QLabel* heatingWaterLabel = nullptr;
    QMovie* heatingWaterSpinnerMovie = nullptr;

    // buttons

protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
