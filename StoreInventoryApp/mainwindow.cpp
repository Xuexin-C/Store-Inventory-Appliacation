#include "additemdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updateitemdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QStringList>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menuNewProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemNew);

    connect(ui->btnRemoveSelectedProduct, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct);

    connect(ui->lstProduct, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);

    connect(ui->menuEditSelectedProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);

    connect(ui->menuSaveProducts, &QAction::triggered,
            this, &MainWindow::handleSaveItems);

    connect(ui->menuLoadProducts, &QAction::triggered,
            this, &MainWindow::handleLoadItems);

}

MainWindow::~MainWindow()
{

    //free up product list memory
    for(Item* product : productList)
    {
        delete product;
    }
    productList.clear();
    delete ui;
}


void MainWindow::handleMenuItemNew()
{
    Item* newItem = nullptr;
    AddItemDialog addItemDialog(newItem, nullptr);

    addItemDialog.setModal(true);
    addItemDialog.exec();

    if(newItem != nullptr)
    {
        productList.push_back(newItem);
        ui->lstProduct->addItem(newItem->getName());
    }

}

void MainWindow::removeSelectedProduct()
{
    int index = ui->lstProduct->currentRow();

    if(index >= 0)
    {
        //remove from vector
        Item* theItem = productList.at(index);
        delete theItem;
        productList.removeAt(index);

        //remove from list widget in the UI
        delete ui->lstProduct->currentItem();
    }

    //set image to none.png, the default
    QPixmap pixmap("none.png");
    ui->lblImage->setPixmap(pixmap);
}//end remove

void MainWindow::handleItemClick(QListWidgetItem *item)
{
    int index = item->listWidget()->currentRow();

    if(index != -1)
    {
        Item* currentItem = productList.at(index);

        if(currentItem != nullptr)
        {
            ui->lblProductName->setText(currentItem->getName());
            ui->lblQuantity->setText(QString::number(currentItem->getQuantity()));

            QPixmap pixmap(currentItem->getImageFilePath());

            ui->lblImage->setPixmap(pixmap);
            ui->lblImage->setScaledContents(true);
        }//end inner if
    }//end if

}//end handleItemClick

void MainWindow::handleMenuItemEdit()
{
    int index = ui->lstProduct->currentRow();

    if(index != -1)
    {
        Item* currentItem = productList.at(index);

        if(currentItem != nullptr)
        {
            UpdateItemDialog updateItemDialog(currentItem, nullptr);
            updateItemDialog.exec();

            //update UI
            ui->lblProductName->setText(currentItem->getName());
            ui->lblQuantity->setText(QString::number(currentItem->getQuantity()));

            QPixmap pixmap(currentItem->getImageFilePath());
            ui->lblImage->setPixmap(pixmap);
            ui->lblImage->setScaledContents(true);
        }
    }
}//end handleMenuItemEdit

void MainWindow::handleSaveItems()
{
    QFile outputFile("products.txt");

    outputFile.open(QIODevice::WriteOnly |
                    QIODevice::Text);

    QTextStream out(&outputFile);

    for (Item* product : productList)
    {
        out<<product->getName()<<",";
        out<<product->getQuantity()<<",";
        out<<product->getImageFilePath()<<Qt::endl;
    }//end for

    out.flush();
    outputFile.close();
}//end handleSaveItems

void MainWindow::handleLoadItems()
{
    QFile inputFile("products.txt");

    inputFile.open(QIODevice::ReadOnly |
                   QIODevice::Text);

    QTextStream in(&inputFile);

    for(Item* temp : productList)
    {
        delete temp;
    }//end for

    productList.clear();
    ui->lstProduct->clear();

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList info = line.split(",");

        //handle list of products UI
        ui->lstProduct->addItem(info.at(0));

        //handle vector
        Item* product = new Item(info.at(0),
                                 info.at(1).toInt(),
                                 info.at(2));

        productList.push_back(product);

    }//end while


    in.flush();
    inputFile.close();
}
























