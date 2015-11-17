#include "noise_bilateral.h"

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    sigma_d = getParameter("sigma_d").toInt();
    sigma_r = getParameter("sigma_r").toInt();
    radius = sigma_d;



    if (image->format() == QImage::Format_Indexed8)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = qGray(pixel);    // Get the 0-255 value of the L channel
                v = calcVal(x,y,LChannel);

                newImage->setPixel(x,y, v);
            }

    }
    else if (image->format() == QImage::Format_RGB32)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                r = calcVal(x,y,RChannel);
                g = calcVal(x,y,GChannel);
                b = calcVal(x,y,BChannel);

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }

    }


    return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
    math::matrix<float> okno = Transformation::getWindow(x,y,radius,channel,RepeatEdge);


    QPoint p,point;
    p.setX(x);
    p.setY(y);


    float suma_licznik = 0;
    float suma_mianownik = 0;
    for (int i=0; i<radius; i++){
        for (int j=0; j<radius; j++){
            point.setX(x-radius/2 + i);
            point.setY(y-radius/2 + j);
            suma_licznik += okno(i,j) * colorCloseness(okno(i,j),okno(radius/2,radius/2)) * spatialCloseness(point, p);
            suma_mianownik += colorCloseness(okno(i,j),okno(radius/2,radius/2)) * spatialCloseness(point, p);
        }
    }

    int result = suma_licznik / suma_mianownik;

    if (result > PIXEL_VAL_MAX)
        result = PIXEL_VAL_MAX;
    else if (result < PIXEL_VAL_MIN)
        result = PIXEL_VAL_MIN;

    return result;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    float param, result;
    param = ( (float)( (val1 - val2) * (val1 - val2) )  /  (float)( 2 * sigma_r * sigma_r ) ) * (-1);
    result = exp(param);

    return result;
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{

    float param, result;
    param = ( (float)(  (point1.x() - point2.x())  *   (point1.x() - point2.x()) +  (point1.y() - point2.y()) *  (point1.y() - point2.y())   )  /  (float)(2*sigma_d*sigma_d) ) * (-1);
    result = exp(param);

    return result;
}
