#include "conversion_grayscale.h"

ConversionGrayscale::ConversionGrayscale(PNM* img) :
    Transformation(img)
{
}

ConversionGrayscale::ConversionGrayscale(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* ConversionGrayscale::transform()
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int width = image->width();
    int height = image->height();

     PNM* newImage = new PNM(width, height, image->format());

     if (image->format() == QImage::Format_Mono)
     {
         // Iterate over image space
         for (int x=0; x<width; x++)
             for (int y=0; y<height; y++)
             {
                 QColor color = QColor::fromRgb(image->pixel(x,y)); // Getting the pixel(x,y) value

                 newImage->setPixel(x,y, color == Qt::white ? Qt::color1 : Qt::color0);
             }
     }
     else if (image->format() == QImage::Format_Indexed8)
     {
         // Iterate over image space
         for (int x=0; x<width; x++)
             for (int y=0; y<height; y++)
             {
                 QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                 int v = qGray(pixel);    // Get the 0-255 value of the L channel

                 newImage->setPixel(x,y, v);
             }
     }
     else //if (image->format() == QImage::Format_RGB32)
     {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                double srednia = r*0.3 + g*0.6 + b*0.1;

                r = srednia;
                g = srednia;
                b = srednia;
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }

    }






    return newImage;
}
