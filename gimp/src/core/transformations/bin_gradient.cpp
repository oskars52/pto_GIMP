#include "bin_gradient.h"

BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);


    int g_x, g_y;
    int suma_licznika = 0;
    int suma_mianownika = 0;

    for (int x=1; x<width-1; x++)
        for (int y=1; y<height-1; y++)
        {

            QRgb pixel = image->pixel(x+1,y); // Getting the pixel(x,y) value

            int v = qGray(pixel);    // Get the 0-255 value of the L channel

            double suma1 = v;

            pixel = image->pixel(x-1,y); // Getting the pixel(x,y) value

            v = qGray(pixel);    // Get the 0-255 value of the L channel

            double suma2 = v;

            g_x = suma1 - suma2;



            pixel = image->pixel(x,y+1); // Getting the pixel(x,y) value

            v = qGray(pixel);    // Get the 0-255 value of the L channel

            suma1 = v;

            pixel = image->pixel(x,y-1); // Getting the pixel(x,y) value

            v = qGray(pixel);    // Get the 0-255 value of the L channel

            suma2 = v;

            g_y = suma1 - suma2;



            int g_max = g_x > g_y ? g_x : g_y;



            pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            v = qGray(pixel);    // Get the 0-255 value of the L channel

            int suma = v;


            suma_licznika += g_max * suma;

            suma_mianownika += g_max;

        }


    int threshold = suma_licznika / suma_mianownika;


    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            int v = qGray(pixel);    // Get the 0-255 value of the L channel

            double suma = v;


            if (suma < threshold){
                newImage->setPixel(x,y, Qt::color0);
            } else {
                newImage->setPixel(x,y, Qt::color1);
            }

        }



    return newImage;
}


