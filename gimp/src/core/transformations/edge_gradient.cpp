#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    PNM* newImage = new PNM(image->width(), image->height(), image->format());

    PNM* image_x = horizontalDetection();
    PNM* image_y = verticalDetection();

    int width  = image->width();
    int height = image->height();

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {
            QRgb pixel = newImage->pixel(x,y);
            QRgb pixel_x = image_x->pixel(x,y);
            QRgb pixel_y = image_y->pixel(x,y);

            float r = qRed(pixel);
            float g = qGreen(pixel);
            float b = qBlue(pixel);

            float r_x = qRed(pixel_x);
            float g_x = qGreen(pixel_x);
            float b_x = qBlue(pixel_x);

            float r_y = qRed(pixel_y);
            float g_y = qGreen(pixel_y);
            float b_y = qBlue(pixel_y);


            r = sqrt(r_x*r_x + r_y*r_y);
            g = sqrt(g_x*g_x + g_y*g_y);
            b = sqrt(b_x*b_x + b_y*b_y);

            if (r > 255){
                r = 255;
            }
            if (g > 255){
                g = 255;
            }
            if (b > 255){
                b = 255;
            }

            //qDebug() << Q_FUNC_INFO << r_x << g_x << b_x << r_y << g_y << b_y;
            QColor newPixel = QColor(r,g,b);

            newImage->setPixel(x,y, newPixel.rgb());
        }

    return newImage;
}

