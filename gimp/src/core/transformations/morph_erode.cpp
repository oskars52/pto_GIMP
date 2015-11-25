#include "morph_erode.h"

MorphErode::MorphErode(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphErode::MorphErode(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphErode::morph(math::matrix<float> window, math::matrix<bool> se)
{
    float max=0.0;


    int rozmiar = window.rowsize();

    for (int x=0; x<rozmiar; x++)
        for (int y=0; y<rozmiar; y++)
        {

            if (se(x,y)){

                if (window(x,y) > max)
                    max = window(x,y);

            }

        }


    return (int)max;
}
