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
        //wyrównanie histogramu
        HistogramEqualization * he = new HistogramEqualization(image);
        image = he->transform();

        //kolorowy

        QHash<int, int>* hist_czer = image->getHistogram()->get(image->getHistogram()->RChannel);
        QHash<int, int>* hist_ziel = image->getHistogram()->get(image->getHistogram()->GChannel);
        QHash<int, int>* hist_nieb = image->getHistogram()->get(image->getHistogram()->BChannel);

        QHash<int, int>::const_iterator c = hist_czer->constBegin();
        QHash<int, int>::const_iterator z = hist_ziel->constBegin();
        QHash<int, int>::const_iterator n = hist_nieb->constBegin();

        long long mi0_licznik = 0;
        long long mi0_mianownik = 0;
        long long mi1_licznik = 0;
        long long mi1_mianownik = 0;

        int wszystkie_pikesel[256];
        for (int i=0; i<=255; i++){
            wszystkie_pikesel[i]=0;
        }

        // dla czerwonego
        for (c = hist_czer->constBegin(); c != hist_czer->constEnd(); ++c){

            wszystkie_pikesel[c.key()] = 1;

            if (c.key() != z.key()){
                z = hist_ziel->constBegin();
                while (c.key() != z.key() && z != hist_ziel->constEnd()) {
                    ++z;
                }
            }

            if (c.key() != n.key()){
                n = hist_nieb->constBegin();
                while (c.key() != n.key() && n != hist_nieb->constEnd()) {
                    ++n;
                }
            }

            int srednia_z_trzech;

            if (c.key() == z.key() && c.key() == n.key()){
                srednia_z_trzech = ( c.value() + z.value() + n.value() ) / 3;
            } else if (c.key() == z.key()) {
                srednia_z_trzech = ( c.value() + z.value() ) / 2;
            } else if (c.key() == n.key()) {
                srednia_z_trzech = ( c.value() + n.value() ) / 2;
            } else {
                srednia_z_trzech = c.value();
            }

            if (c.key() < threshold){
                mi0_licznik += srednia_z_trzech * c.key();
                mi0_mianownik += srednia_z_trzech;
            }

            if (c.key() >= threshold){
                mi1_licznik += srednia_z_trzech * c.key();
                mi1_mianownik += srednia_z_trzech;
            }

            //qDebug() << Q_FUNC_INFO << "Srednia " << c.key() << z.key() << " " << n.key();

        }

//        for (int i=0; i<=255; i++){
//            if (wszystkie_pikesel[i] == 0)
//                qDebug() << Q_FUNC_INFO << "Czerwony kanał " << i << " " << wszystkie_pikesel[i];
//        }



        for (int i=0; i<=255; i++){
            if (wszystkie_pikesel[i] == 0){

                z = hist_ziel->constBegin();
                while (i != z.key() && z != hist_ziel->constEnd()) {
                    ++z;
                }

                n = hist_nieb->constBegin();
                while (i != n.key() && n != hist_nieb->constEnd()) {
                    ++n;
                }

                int srednia_z_dwoch;
                if (i == z.key() && i == n.key()){
                    srednia_z_dwoch = ( z.value() + n.value() ) / 2;
                } else if (i == z.key()) {
                    srednia_z_dwoch = z.value();
                } else {
                    srednia_z_dwoch = n.value();
                }

                wszystkie_pikesel[z.key()] = 1;

                if (i < threshold){
                    mi0_licznik += srednia_z_dwoch * i;
                    mi0_mianownik += srednia_z_dwoch;
                }

                if (i >= threshold){
                    mi1_licznik += srednia_z_dwoch * i;
                    mi1_mianownik += srednia_z_dwoch;
                }

            }
        }

//        for (int i=0; i<=255; i++){
//            if (wszystkie_pikesel[i] == 0)
//                qDebug() << Q_FUNC_INFO << "Pozostale " << i << " " << wszystkie_pikesel[i];
//        }


        int mi0 = mi0_licznik / mi0_mianownik;
        int mi1 = mi1_licznik / mi1_mianownik;

        int threshold_new = (mi0 + mi1) / 2;

        while (threshold_new != threshold) {

            threshold = threshold_new;

            mi0_licznik = 0;
            mi0_mianownik = 0;
            mi1_licznik = 0;
            mi1_mianownik = 0;

            for (c = hist_czer->constBegin(); c != hist_czer->constEnd(); ++c){

                if (c.key() != z.key()){
                    z = hist_ziel->constBegin();
                    while (c.key() != z.key() && z != hist_ziel->constEnd()) {
                        ++z;
                    }
                }

                if (c.key() != n.key()){
                    n = hist_nieb->constBegin();
                    while (c.key() != n.key() && n != hist_nieb->constEnd()) {
                        ++n;
                    }
                }

                int srednia_z_trzech;

                if (c.key() == z.key() && c.key() == n.key()){
                    srednia_z_trzech = ( c.value() + z.value() + n.value() ) / 3;
                } else if (c.key() == z.key()) {
                    srednia_z_trzech = ( c.value() + z.value() ) / 2;
                } else if (c.key() == n.key()) {
                    srednia_z_trzech = ( c.value() + n.value() ) / 2;
                } else if (n.key() == z.key()) {
                    srednia_z_trzech = ( n.value() + z.value() ) / 2;
                } else {
                    srednia_z_trzech = c.value();
                }

                if (c.key() < threshold){
                    mi0_licznik += srednia_z_trzech * c.key();
                    mi0_mianownik += srednia_z_trzech;
                }

                if (c.key() >= threshold){
                    mi1_licznik += srednia_z_trzech * c.key();
                    mi1_mianownik += srednia_z_trzech;
                }

            }





            for (int i=0; i<=255; i++){
                if (wszystkie_pikesel[i] == 0){

                    z = hist_ziel->constBegin();
                    while (i != z.key() && z != hist_ziel->constEnd()) {
                        ++z;
                    }

                    n = hist_nieb->constBegin();
                    while (i != n.key() && n != hist_nieb->constEnd()) {
                        ++n;
                    }

                    int srednia_z_dwoch;
                    if (i == z.key() && i == n.key()){
                        srednia_z_dwoch = ( z.value() + n.value() ) / 2;
                    } else if (i == z.key()) {
                        srednia_z_dwoch = z.value();
                    } else {
                        srednia_z_dwoch = n.value();
                    }

                    if (i < threshold){
                        mi0_licznik += srednia_z_dwoch * i;
                        mi0_mianownik += srednia_z_dwoch;
                    }

                    if (i >= threshold){
                        mi1_licznik += srednia_z_dwoch * i;
                        mi1_mianownik += srednia_z_dwoch;
                    }

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

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                int srednia = ( r + g + b ) / 3;

                if (srednia < threshold){
                    newImage->setPixel(x,y, Qt::color0);
                } else {
                    newImage->setPixel(x,y, Qt::color1);
                }

            }


    }


    return newImage;
}



