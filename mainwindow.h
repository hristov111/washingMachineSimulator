#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QQuaternion>
#include <QVector3D>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <qpushbutton.h>

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

    void on_temperatureBox_currentTextChanged(const QString &arg1);

    void on_spinBox_currentTextChanged(const QString &arg1);

    void on_rinseBox_currentTextChanged(const QString &arg1);

    void on_ecomodeBox_currentTextChanged(const QString &arg1);
    float calculateEndtime();

private:
    Ui::MainWindow *ui;

    Qt3DCore::QTransform *modelTransform = nullptr;
    QWidget *overlay = nullptr;

    // this will hold labels transforms, root Transofrom, and their first coordinates , so we can go back and revert coordinates
    MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *colorTable= nullptr;

    QWidget *container = nullptr;


    QString currentMode = "";
    int tempTime = 0;
    int spinTime = 0;
    int ecoTime = 0;
    int rinseTime = 0;

    float endTime = 2.0f;

    // buttons

protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
