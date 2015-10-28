#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    QHash<int, int>* czer = image->getHistogram()->get(image->getHistogram()->RChannel);
    QHash<int, int>* ziel = image->getHistogram()->get(image->getHistogram()->GChannel);
    QHash<int, int>* nieb = image->getHistogram()->get(image->getHistogram()->BChannel);
    QHash<int, int>* szar = image->getHistogram()->get(image->getHistogram()->LChannel);

    QHash<int, int>::const_iterator i;



    if (image->format() == QImage::Format_Indexed8)
    {

        // kolor szary

        double dystrybuanta_szar[PIXEL_VAL_MAX+1];

        long long suma_szar = 0;
        for (i = szar->constBegin(); i != szar->constEnd(); ++i){
            suma_szar += i.value();
        }

        long long suma2_szar;
        for (int i = 0; i < PIXEL_VAL_MAX; ++i) {
            suma2_szar = 0;

            for (int j=0; j<=i; j++){
                suma2_szar += szar->value(j);
            }

            dystrybuanta_szar[i] = (double)suma2_szar / (double)suma_szar;

        }

        int LUT_SZAR[PIXEL_VAL_MAX+1];

        double d0_szar;
        for (int i=0; i < PIXEL_VAL_MAX; i++){

            if (dystrybuanta_szar[i] > 0){
                d0_szar = dystrybuanta_szar[i];
                break;
            }
        }

        for (int i=0; i < PIXEL_VAL_MAX; i++){

            LUT_SZAR[i] = ( ( dystrybuanta_szar[i] - d0_szar ) / ( 1 - d0_szar ) ) * ( PIXEL_VAL_MAX - 1);
        }


        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {

                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = qGray(pixel);    // Get the 0-255 value of the L channel
                v = LUT_SZAR[v];

                newImage->setPixel(x,y, v);
            }



    }
    else //if (image->format() == QImage::Format_RGB32)
    {


        // kolor czerwony

        double dystrybuanta_czer[PIXEL_VAL_MAX+1];

        long long suma_czer = 0;
        for (i = czer->constBegin(); i != czer->constEnd(); ++i){
            suma_czer += i.value();
        }

        long long suma2_czer;
        for (int i = 0; i < PIXEL_VAL_MAX; ++i) {
            suma2_czer = 0;

            for (int j=0; j<=i; j++){
                suma2_czer += czer->value(j);
            }

            dystrybuanta_czer[i] = (double)suma2_czer / (double)suma_czer;

        }

        int LUT_CZER[PIXEL_VAL_MAX+1];

        double d0_czer;
        for (int i=0; i < PIXEL_VAL_MAX; i++){

            if (dystrybuanta_czer[i] > 0){
                d0_czer = dystrybuanta_czer[i];
                break;
            }
        }

        for (int i=0; i < PIXEL_VAL_MAX; i++){

            LUT_CZER[i] = ( ( dystrybuanta_czer[i] - d0_czer ) / ( 1 - d0_czer ) ) * ( PIXEL_VAL_MAX - 1);
        }



        // kolor zielony

        double dystrybuanta_ziel[PIXEL_VAL_MAX+1];

        long long suma_ziel = 0;
        for (i = ziel->constBegin(); i != ziel->constEnd(); ++i){
            suma_ziel += i.value();
        }

        long long suma2_ziel;
        for (int i = 0; i < PIXEL_VAL_MAX; ++i) {
            suma2_ziel = 0;

            for (int j=0; j<=i; j++){
                suma2_ziel += ziel->value(j);
            }

            dystrybuanta_ziel[i] = (double)suma2_ziel / (double)suma_ziel;

        }

        int LUT_ZIEL[PIXEL_VAL_MAX+1];

        double d0_ziel;
        for (int i=0; i < PIXEL_VAL_MAX; i++){

            if (dystrybuanta_ziel[i] > 0){
                d0_ziel = dystrybuanta_ziel[i];
                break;
            }
        }

        for (int i=0; i < PIXEL_VAL_MAX; i++){

            LUT_ZIEL[i] = ( ( dystrybuanta_ziel[i] - d0_ziel ) / ( 1 - d0_ziel ) ) * ( PIXEL_VAL_MAX - 1);
        }




        // kolor niebieski

        double dystrybuanta_nieb[PIXEL_VAL_MAX+1];

        long long suma_nieb = 0;
        for (i = nieb->constBegin(); i != nieb->constEnd(); ++i){
            suma_nieb += i.value();
        }

        long long suma2_nieb;
        for (int i = 0; i < PIXEL_VAL_MAX; ++i) {
            suma2_nieb = 0;

            for (int j=0; j<=i; j++){
                suma2_nieb += nieb->value(j);
            }

            dystrybuanta_nieb[i] = (double)suma2_nieb / (double)suma_nieb;

        }

        int LUT_NIEB[PIXEL_VAL_MAX+1];

        double d0_nieb;
        for (int i=0; i < PIXEL_VAL_MAX; i++){

            if (dystrybuanta_nieb[i] > 0){
                d0_nieb = dystrybuanta_nieb[i];
                break;
            }
        }

        for (int i=0; i < PIXEL_VAL_MAX; i++){

            LUT_NIEB[i] = ( ( dystrybuanta_nieb[i] - d0_nieb ) / ( 1 - d0_nieb ) ) * ( PIXEL_VAL_MAX - 1);
        }


        // nowy obrazek

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                r = LUT_CZER[r];
                g = LUT_ZIEL[g];
                b = LUT_NIEB[b];
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }


    }








    return newImage;
}

