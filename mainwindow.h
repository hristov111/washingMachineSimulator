#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QQuaternion>
#include <QVector3D>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>

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

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    Qt3DCore::QTransform *modelTransform = nullptr;
    QWidget *overlay = nullptr;

    // this will hold labels transforms, root Transofrom, and their first coordinates , so we can go back and revert coordinates
    MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *colorTable= nullptr;

    QWidget *container = nullptr;

protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
