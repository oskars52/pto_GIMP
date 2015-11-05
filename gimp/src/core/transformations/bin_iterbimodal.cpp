#include "bin_iterbimodal.h"

#include "conversion_grayscale.h"
#include "histogram_equalization.h"
#include "../histogram.h"

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img) :
    Transformation(img)
{
}

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationIterBimodal::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);


    int threshold = 128;



    if (image->format() == QImage::Format_Indexed8)
    {
        //szary

        QHash<int, int>* hist_szar = image->getHistogram()->get(image->getHistogram()->LChannel);

        QHash<int, int>::const_iterator i = hist_szar->constBegin();

        long long mi0_licznik = 0;
        long long mi0_mianownik = 0;
        long long mi1_licznik = 0;
        long long mi1_mianownik = 0;


        for (i = hist_szar->constBegin(); i != hist_szar->constEnd(); ++i){

            if (i.key() < threshold){
                mi0_licznik += i.value() * i.key();
                mi0_mianownik += i.value();
            }

            if (i.key() >= threshold){
                mi1_licznik += i.value() * i.key();
                mi1_mianownik += i.value();
            }


        }

        int mi0 = mi0_licznik / mi0_mianownik;
        int mi1 = mi1_licznik / mi1_mianownik;

        int threshold_new = (mi0 + mi1) / 2;

        while (threshold_new != threshold) {

            threshold = threshold_new;


            mi0_licznik = 0;
            mi0_mianownik = 0;
            mi1_licznik = 0;
            mi1_mianownik = 0;

            for (i = hist_szar->constBegin(); i != hist_szar->constEnd(); ++i){

                if (i.value() < threshold){
                    mi0_licznik += i.value() * i.key();
                    mi0_mianownik += i.value();
                }

                if (i.value() > threshold){
                    mi1_licznik += i.value() * i.key();
                    mi1_mianownik += i.value();
                }

            }

            mi0 = mi0_licznik / mi0_mianownik;
            mi1 = mi1_licznik / mi1_mianownik;

            threshold_new = (mi0 + mi1) / 2;

        }




        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = qGray(pixel);    // Get the 0-255 value of the L channel

                if (v < threshold){
                    newImage->setPixel(x,y, Qt::color0);
                } else {
                    newImage->setPixel(x,y, Qt::color1);
                }

            }

    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        //kolorowy

        QHash<int, int>* hist_czer = image->getHistogram()->get(image->getHistogram()->RChannel);
        QHash<int, int>* hist_ziel = image->getHistogram()->get(image->getHistogram()->GChannel);
        QHash<int, int>* hist_nieb = image->getHistogram()->get(image->getHistogram()->BChannel);



    }












    return newImage;
}



