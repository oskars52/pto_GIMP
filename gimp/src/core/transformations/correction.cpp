#include "correction.h"
#include <QtCore/qmath.h>

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    float shift  = getParameter("shift").toFloat();
    float factor = getParameter("factor").toFloat();
    float gamma  = getParameter("gamma").toFloat();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    // wypełnienie tablicy LUT
    for (int x=0; x<PIXEL_VAL_MAX+1; x++)
    {
        LUT[x] = x;
    }

    //
    for (int x=0; x<PIXEL_VAL_MAX+1; x++)
    {
        LUT[x] = LUT[x] + shift;
        LUT[x] = LUT[x] * factor;
        LUT[x] = qPow(LUT[x],gamma);
        LUT[x] = (LUT[x] > PIXEL_VAL_MAX) ? PIXEL_VAL_MAX : LUT[x];
        LUT[x] = (LUT[x] < PIXEL_VAL_MIN) ? PIXEL_VAL_MIN : LUT[x];
    }


    if (image->format() == QImage::Format_Mono)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QColor color = QColor::fromRgb(image->pixel(x,y));

                newImage->setPixel(x,y, color == Qt::white ? LUT[PIXEL_VAL_MAX] : LUT[PIXEL_VAL_MIN]);
            }
    }
    else if (image->format() == QImage::Format_Indexed8)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int v = qGray(pixel);
                v = LUT[v];

                newImage->setPixel(x,y, v);
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);

                r = LUT[r];
                g = LUT[g];
                b = LUT[b];

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }

    return newImage;
}
