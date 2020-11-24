#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QColor>
#include "treewidgetitem.h"
#include "mypicture.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addLayerButton_clicked();

    void on_actionOpen_triggered();

    void on_colorButton_clicked();

    void itemEdit();

    void verifyChange(QTreeWidgetItem *item, int column);

    void treeWidgetItemMenu(const QPoint &pos);

    void deleteItem();

    void on_actionSave_image_triggered();

    void on_actionSave_project_triggered();

    void on_actionOpen_project_triggered();

    void on_generateImageButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *originalImageScene;
    QGraphicsScene *grayImageScene;
    QGraphicsScene *finalImageScene;
    QImage originalImage;
    QImage grayscaleImage;
    QImage finalImage;
    QColor currentColor;
    MyPicture *picture;

    QString generateLayerName();
    void updatePicture();
};

#endif // MAINWINDOW_H
