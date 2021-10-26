#include "mainwindow.h"
#include "./ui_mainwindow.h"

ZENO_NAMESPACE_BEGIN

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nodeScene(new QDMGraphicsScene)
{
    ui->setupUi(this);
    ui->nodeView->setScene(nodeScene.get());
}

MainWindow::~MainWindow() = default;

ZENO_NAMESPACE_END
