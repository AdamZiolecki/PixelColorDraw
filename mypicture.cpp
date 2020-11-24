#include "mypicture.h"
#include <QDebug>

MyPicture::MyPicture()
{

}

QImage MyPicture::generatePicture(QImage grayscaleImage, QList<QTreeWidgetItem *> itemList)
{
    QVector<uint> colorLayer;
    QImage outImage = grayscaleImage;
    QColor color;

    // *** Read layers ***

    // First layer (white background)
    colorLayer.append(255);
    colorLayer.append(qRgb(255, 255, 255));

    // Read other layers
    for (int i = 0; i < itemList.size(); ++i)
    {
        colorLayer.append(itemList.at(i)->text(1).toUInt());
        color = itemList.at(i)->backgroundColor(2);
        colorLayer.append(qRgb(color.red(), color.green(), color.blue()));
    }

    // Create out image
    for (int k = 0; k < colorLayer.size(); k += 2)
    {
        for (int i = 0; i < outImage.width(); i++)
        {
            for (int j = 0; j < outImage.height(); j++)
            {
                if (qGray(grayscaleImage.pixel(i, j)) <= colorLayer[k])
                {
                    outImage.setPixelColor(i, j, colorLayer[k+1]);
                }

            }

        }
    }

    return outImage;
}
