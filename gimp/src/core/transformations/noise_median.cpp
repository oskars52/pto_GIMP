#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    Channel RR = RChannel; //0
    Channel GG = GChannel; //1
    Channel BB = BChannel; //2

    if (image->format() == QImage::Format_Indexed8)
    {

    }
    else if (image->format() == QImage::Format_RGB32)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                // wywołujemy okna sąsiedztwa dla każdego kanału
                int r = getMedian(x,y,RR);
                int g = getMedian(x,y,GG);
                int b = getMedian(x,y,BB);

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());

            }

    }


    return newImage;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();
    radius = radius * 2 + 1;

    int zlicz = 0;

    float tab[radius*radius + 1];

    math::matrix<float> okno = Transformation::getWindow(x,y,radius,channel,RepeatEdge);

    for(int i=0; i<radius; ++i){
        for(int q=0; q<radius; ++q){

            tab[zlicz++] = okno(i,q);
        }
    }


    int mediana;


    //sortujemy tablicę
    int pom;
    int n = zlicz;
    for(int i=0;i<n;i++)
        for(int j=0;j<n-i-1;j++)
            if(tab[j]>tab[j+1])
            {
                pom = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = pom;
            }


    if ( zlicz % 2 == 0 ){ // wartość parzysta
        mediana = (tab[zlicz/2] + tab[zlicz/2+1]) / 2;
    } else {
        mediana = tab[zlicz/2];
    }




    return mediana;
}
