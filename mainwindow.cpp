#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "Original");
    ui->tabWidget->setTabText(1, "Gray");
    ui->tabWidget->setTabText(2, "Final");

    originalImageScene = new QGraphicsScene(this);
    grayImageScene = new QGraphicsScene(this);
    finalImageScene = new QGraphicsScene(this);

    QIcon colorButtonIcon("D:/crap/colordialogicon2.png");
    ui->colorButton->setIcon(colorButtonIcon);
    ui->colorButton->setIconSize(QSize(50, 50));

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnWidth(3, 50);

//    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(itemEdit()));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemEdit()));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(verifyChange(QTreeWidgetItem*, int)));
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::treeWidgetItemMenu);

    picture = new MyPicture;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addLayerButton_clicked()
{
    TreeWidgetItem *newItem = new TreeWidgetItem(ui->treeWidget);
    QString defaultLayerName = generateLayerName();

    newItem->setText(0, defaultLayerName);
    newItem->setText(1, QString::number(ui->thresholdSpinBox->value()));
    newItem->setBackgroundColor(2, currentColor);
    newItem->setCheckState(3, Qt::Checked);
    newItem->setTextAlignment(3, Qt::AlignCenter);  // Doesn't really work

    ui->treeWidget->addTopLevelItem(newItem);
    ui->treeWidget->sortItems(1, Qt::DescendingOrder);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.png *.jpg)"));
    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Error";
        }

        file.close();
    }

    // Orignal Image
    originalImage = QImage(fileName);
    originalImageScene->addPixmap(QPixmap::fromImage(originalImage));
    ui->originalImageView->setScene(originalImageScene);

    // Grayscale Image
    grayscaleImage = originalImage.convertToFormat(QImage::Format_Grayscale8);
    grayImageScene->addPixmap(QPixmap::fromImage(grayscaleImage));
    ui->grayImageView->setScene(grayImageScene);
}

void MainWindow::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(currentColor);
    if(color.isValid())
    {
        currentColor = color;
    }
}

QString MainWindow::generateLayerName()
{
    QList<QTreeWidgetItem *> treeWidgetItemsList;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QString text = "Layer_" + QString::number(i);
        treeWidgetItemsList = ui->treeWidget->findItems(text, Qt::MatchContains|Qt::MatchRecursive, 0);
        if (treeWidgetItemsList.empty())
        {
            return "Layer_" + QString::number(i);
        }
    }

    return "Layer_" + QString::number(ui->treeWidget->topLevelItemCount());
}

void MainWindow::updatePicture()
{
    QList<QTreeWidgetItem *> items = ui->treeWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive);
    // Remove items that have show status off
    for (int i = 0; i < items.size(); ++i)
    {
        if (items.at(i)->checkState(3) == 0)
        {
            qDebug() << items.size();
            items.removeAt(i);
            i = -1;
        }
    }
    finalImage = picture->generatePicture(originalImage, items);
    finalImageScene->addPixmap(QPixmap::fromImage(finalImage));
    ui->finalImageView->setScene(finalImageScene);
}

void MainWindow::itemEdit()
{
    if (ui->treeWidget->currentColumn() == 0 || ui->treeWidget->currentColumn() == 1)
    {
        ui->treeWidget->currentItem()->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else if (ui->treeWidget->currentColumn() == 2)
    {
        ui->treeWidget->currentItem()->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        QColor color = QColorDialog::getColor(ui->treeWidget->currentItem()->backgroundColor(2));
        if(color.isValid())
        {
            ui->treeWidget->currentItem()->setBackgroundColor(2, color);
        }
    }
    else if (ui->treeWidget->currentColumn() == 3)
    {
        //ui->treeWidget->currentItem()->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        if (ui->treeWidget->currentItem()->checkState(3) == 2)
        {
            ui->treeWidget->currentItem()->setCheckState(3, Qt::Unchecked);
        }
        else
        {
            ui->treeWidget->currentItem()->setCheckState(3, Qt::Checked);
        }
    }
}

void MainWindow::verifyChange(QTreeWidgetItem *item, int column)
{
    if (column == 1)
    {
        QString textToValidate = item->text(1);
        int pos = 0;
        QIntValidator v(0, 255, this);
        if (!v.validate(textToValidate, pos))
        {
            // TODO: show error communicate here
            item->setText(1, "0");
        }
    }
    if (ui->treeWidget->currentItem() != nullptr)
    {
        ui->treeWidget->currentItem()->setSelected(false);
    }
    ui->treeWidget->sortItems(1, Qt::DescendingOrder);
}

void MainWindow::treeWidgetItemMenu(const QPoint &pos)
{
    if (ui->treeWidget->itemAt(pos))
    {
        QAction newAct("Delete");
        newAct.setStatusTip(tr("Delete this layer"));
        connect(&newAct, SIGNAL(triggered()), this, SLOT(deleteItem()));

        QMenu menu(this);
        menu.addAction(&newAct);
        menu.exec(ui->treeWidget->mapToGlobal(pos));
    }

}

void MainWindow::deleteItem()
{
    delete ui->treeWidget->currentItem();
}

void MainWindow::on_actionSave_image_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Image files (*.png *.jpg)"));
    if (fileName != "")
    {
        finalImage.save(fileName);
    }
}

void MainWindow::on_actionSave_project_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text files (*.txt)"));
    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append | QIODevice::Truncate))
        {
            qDebug() << "Error 0";
        }
        QTextStream out(&file);
        int i = 0;
        while (i < ui->treeWidget->topLevelItemCount())
        {
            out << ui->treeWidget->topLevelItem(i)->text(0) << ";";
            out << ui->treeWidget->topLevelItem(i)->text(1) << ";";
            out << ui->treeWidget->topLevelItem(i)->backgroundColor(2).red() << ",";
            out << ui->treeWidget->topLevelItem(i)->backgroundColor(2).green() << ",";
            out << ui->treeWidget->topLevelItem(i)->backgroundColor(2).blue() << ";";
            out << ui->treeWidget->topLevelItem(i)->checkState(3) << "\n";
            ++i;
        }
        file.close();
    }
}

void MainWindow::on_actionOpen_project_triggered()
{
    // TODO: Ask for save current project if exists. Other way it will be lost
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));
    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Error";
        }

        ui->treeWidget->clear();

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList tempList = line.split(";");
            QStringList colorRgbList = tempList.at(2).split(",");
            TreeWidgetItem *newItem = new TreeWidgetItem(ui->treeWidget);
            newItem->setText(0, tempList.at(0));
            newItem->setText(1, tempList.at(1));
            newItem->setBackgroundColor(2, QColor(colorRgbList.at(0).toInt(), colorRgbList.at(1).toInt(), colorRgbList.at(2).toInt()));
            if (tempList.at(3) == "0")
            {
                newItem->setCheckState(3, Qt::Unchecked);
            }
            else if (tempList.at(3) == "2")
            {
                newItem->setCheckState(3, Qt::Checked);
            }
            else
            {
                qDebug() << "ERROR";
            }
        }
    }

}

void MainWindow::on_generateImageButton_clicked()
{
    updatePicture();
}
