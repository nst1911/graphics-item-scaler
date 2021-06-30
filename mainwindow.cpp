#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsRectItem *item = new QGraphicsRectItem(QRectF(0, 0, 100, 100));
    item->setPen(QColor(102, 102, 102));
    item->setBrush(QColor(158, 204, 255));

    GraphicsItemScaler *scaler = new GraphicsItemScaler(item);
    scaler->setPos(10, 10);
    scaler->setHandleItemBrush(QColor(64, 64, 64));
    scaler->setHandlersIgnoreTransformations(true);
    scaler->setBoundingRectAreaVisible(true);
    scene->addItem(scaler);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mTestItem = item;
    mScaler = scaler;

    connect(ui->plusButton, &QPushButton::clicked, this, [&]() {
        ui->graphicsView->scale(1.2, 1.2);
    });
    connect(ui->minusButton, &QPushButton::clicked, this, [&]() {
        ui->graphicsView->scale((1.0 / 1.2), (1.0 / 1.2));
    });
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]() {
        mTestItem->setRotation(ui->spinBox->value());
        mScaler->recalculate(); // You have to call it every time your item's rotation is changed
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
