#include "interactive3dwindow.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <Qt3DRender/QCamera>
#include <Qt3DInput/QInputAspect>




Interactive3DWindow::Interactive3DWindow(QScreen *screen, MyHashtable<QString,Qt3DExtras::QPhongMaterial*> *myTable): Qt3DExtras::Qt3DWindow(screen),table(myTable){
    this->setMouseGrabEnabled(true);

    registerAspect(new Qt3DInput::QInputAspect());
}

void Interactive3DWindow::setTargetTransform(Qt3DCore::QTransform *transform){
    targetTransform = transform;
}

void makeButtonRed(std::optional<Qt3DExtras::QPhongMaterial*> res){
    if(res.has_value()){
        res.value()->setDiffuse(QColor(Qt::red));
    }
}

void Interactive3DWindow::mousePressEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        lastMousePos = event->pos();

        // insert label click QPoint(120,43)
        QPointF insertPowderLabel = QPointF(120,43);
        float insertPowderLabeldistance = QLineF(lastMousePos, insertPowderLabel).length();
        // select program  click    125,86
        QPointF selectProgramLabel = QPointF(125,86);
        float selectProgramdistance = QLineF(lastMousePos, selectProgramLabel).length();

        // start program click QPoint(115,130)
        QPointF startProgram = QPointF(115,130);
        float startProgramDistance = QLineF(lastMousePos, startProgram).length();
        // emergency stop click QPoint(127,171)
        QPointF emergencyStop = QPointF(127,171);
        float emergencyStopDistance = QLineF(lastMousePos, emergencyStop).length();


        //const float radius = 40.0f;
        //if(insertPowderLabeldistance <= radius){
            //qDebug() << "Insert powder clicked";
            // here i need to change
            //auto res = table->get("1. Insert Powder");
           // makeButtonRed(res);
        //}
        //else if(selectProgramdistance <= radius){
            //qDebug() << "Select program clicked";
            //auto res = table->get("2. Select Program");
            //makeButtonRed(res);

        //}
        //else if(startProgramDistance <= radius){
            //qDebug() << "Start program clicked";
            //auto res =  table->get("3. Start Program");
            //makeButtonRed(res);

        //}
       // else if(emergencyStopDistance <= radius){
            //qDebug() << "Emergency stop clicked";
            //auto res =  table->get("4. Emergency stop");
            //makeButtonRed(res);

        //}
        //qDebug() << "Mouse clicked at " << event->pos();


    }
    Qt3DWindow::mousePressEvent(event);
}

void Interactive3DWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton && targetTransform){

        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();


        rotationX +=delta.y() * 0.5f;
        rotationY += delta.x() * 0.5f;

        QQuaternion newRotation =QQuaternion::fromEulerAngles(rotationX, rotationY, 0.0f);
        targetTransform->setRotation(newRotation);


        if(modifier){
            modifier->applyRotationToLabels(newRotation);
        }
    }

    Qt3DWindow::mouseMoveEvent(event);
}

void Interactive3DWindow::setModifier(SceneModifier *mod){
    modifier = mod;
}

void Interactive3DWindow::wheelEvent(QWheelEvent *event){
    Qt3DRender::QCamera *camera = this->camera();
    if(!camera){
        return;
    }

    float zoomFactor = 0.2f;
    float delta = event->angleDelta().y();

    QVector3D camPos = camera->position();
    QVector3D center = camera->viewCenter();
    QVector3D direction = (center -camPos).normalized();

    if(delta > 0){
        // zoom in
        camPos += direction * zoomFactor;
    }else if (delta < 0){
        camPos -= direction * zoomFactor;
    }

    camera->setPosition(camPos);

    event->accept();
}
