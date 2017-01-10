#include "form_createNewSwarmType1.h"
#include "Core.h"
#include "SwarmType1FactoryMethods.h"

void form_createNewSwarmType1::swarmLabelInputTextChanged(QString /*text*/) {
    updateCmdCreate();
}

void form_createNewSwarmType1::updateCmdCreate() {
    QString text = canonicalizedSwarmLabel();
    bool empty = text.isEmpty() || text.isNull();
    cmd_create->setEnabled(!empty);
}

QString  form_createNewSwarmType1::canonicalizedSwarmLabel() {
    QString text = inputField_localSwarmLabel->text();
    return text.trimmed();
}

form_createNewSwarmType1::form_createNewSwarmType1(CCore&Core)
    : mCore(Core)
{
    setupUi(this);
    updateCmdCreate();
    
    connect(cmd_create,SIGNAL(pressed()),this,
            SLOT(slot_cmdCreate()));
    
    connect(cmd_cancel,SIGNAL(clicked()),this,
            SLOT(close()));

    connect(inputField_localSwarmLabel, SIGNAL(textChanged(QString)), this, SLOT(swarmLabelInputTextChanged(QString)));
}

form_createNewSwarmType1::~form_createNewSwarmType1() {
}

void form_createNewSwarmType1::slot_cmdCreate() {
    cmd_create->setEnabled(false);
    inputField_localSwarmLabel->setEnabled(false);

    bool successBool;
    QString errorMsg;
    QString swarmIdFolderFullPathStr;
    SwarmType1LocalIdType failedSwarmId;

    SwarmType1 *swarm = SwarmType1FactoryMethods::createSwarmType1(
                canonicalizedSwarmLabel(), mCore, successBool, errorMsg, swarmIdFolderFullPathStr, failedSwarmId, new SwarmType1WhiteList(), 0, false);
    if(swarm && successBool){ mCore.getRosterModel().createNewSwarmType1(*swarm); close(); }
    else {
        QMessageBox::critical(this,tr("Error creating swarm"), successBool?tr("Error: swarm create returned nullptr"):errorMsg);
        updateCmdCreate();
        inputField_localSwarmLabel->setEnabled(true);
    }
}

void form_createNewSwarmType1::closeEvent(QCloseEvent* e) {
    e->ignore();
    emit signClosingWindow();
}

void form_createNewSwarmType1::requestFocus() {
    this->activateWindow();
    this->setWindowState((windowState() & (~Qt::WindowMinimized)) | Qt::WindowActive);
    this->raise();
}

void form_createNewSwarmType1::keyPressEvent(QKeyEvent* event) {
    if (event->key() != Qt::Key_Escape){
        QDialog::keyPressEvent(event);
    }
    else {
        event->accept();
        close();
    }
}
