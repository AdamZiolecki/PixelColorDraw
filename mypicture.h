#ifndef MYPICTURE_H
#define MYPICTURE_H

#include <QList>
#include <QTreeWidgetItem>
#include <QImage>
#include <QColor>

class MyPicture
{
public:
    MyPicture();
    QImage generatePicture(QImage grayscaleImage, QList<QTreeWidgetItem *> itemList);
};

#endif // MYPICTURE_H
