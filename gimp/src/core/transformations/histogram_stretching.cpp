#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());


    QHash<int, int>* czer = image->getHistogram()->get(image->getHistogram()->RChannel);
    QHash<int, int>* ziel = image->getHistogram()->get(image->getHistogram()->GChannel);
    QHash<int, int>* nieb = image->getHistogram()->get(image->getHistogram()->BChannel);

    QHash<int, int>::const_iterator i;




    // kolor czerwony
    int czer_max = -1;
    int czer_min = 260;
    for (i = czer->constBegin(); i != czer->constEnd(); ++i){
//        qDebug() << Q_FUNC_INFO << i.value() << " " << i.key() << endl;
        if(i.value() > 0){
            if (i.key() < czer_min){
                czer_min = i.key();

            }
        }
    }
    for (i = czer->constBegin(); i != czer->constEnd(); ++i){
        if(i.value() > 0){
            if (i.key() > czer_max){
                czer_max = i.key();

            }

        }
    }

    // kolor zielony
    int ziel_max = -1;
    int ziel_min = 260;
    for (i = ziel->constBegin(); i != ziel->constEnd(); ++i){
        if(i.value() > 0){
            if (i.key() < ziel_min){
                ziel_min = i.key();

            }
        }
    }
    for (i = ziel->constBegin(); i != ziel->constEnd(); ++i){
        if(i.value() > 0){
            if (i.key() > ziel_max){
                ziel_max = i.key();

            }
        }
    }

    // kolor niebieski
    int nieb_max = -1;
    int nieb_min = 260;
    for (i = nieb->constBegin(); i != nieb->constEnd(); ++i){
        if(i.value() > 0){
            if (i.key() < nieb_min){
                nieb_min = i.key();

            }
        }
    }
    for (i = nieb->constBegin(); i != nieb->constEnd(); ++i){
        if(i.value() > 0){
            if (i.key() > nieb_max){
                nieb_max = i.key();

            }
        }
    }


    qDebug() << Q_FUNC_INFO << czer_max << " " << czer_min << endl;
    qDebug() << Q_FUNC_INFO << ziel_max << " " << ziel_min << endl;
    qDebug() << Q_FUNC_INFO << nieb_max << " " << nieb_min << endl;

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            int r = qRed(pixel);    // Get the 0-255 value of the R channel
            int g = qGreen(pixel);  // Get the 0-255 value of the G channel
            int b = qBlue(pixel);   // Get the 0-255 value of the B channel

            r = ( (double)PIXEL_VAL_MAX / (double)((double)czer_max - (double)czer_min) ) * (double)(r - czer_min);
            g = ( (double)PIXEL_VAL_MAX / (double)((double)ziel_max - (double)ziel_min) ) * (double)(g - ziel_min);
            b = ( (double)PIXEL_VAL_MAX / (double)((double)nieb_max - (double)nieb_min) ) * (double)(b - nieb_min);
            QColor newPixel = QColor(r,g,b);
            newImage->setPixel(x,y, newPixel.rgb());
        }


    return newImage;
}


