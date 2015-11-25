#include "morph_dilate.h"

MorphDilate::MorphDilate(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphDilate::MorphDilate(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphDilate::morph(math::matrix<float> window, math::matrix<bool> se)
{
    float min = PIXEL_VAL_MAX+1;

    int rozmiar = window.size();



    for (int x=0; x<rozmiar; x++)
        for (int y=0; y<rozmiar; y++)
        {
            qDebug() << Q_FUNC_INFO << "zzz " << window(x,y);
            if (se(x,y)){

                if (window(x,y) < min)
                    min = window(x,y);

            }

        }


    return (int)min;
}
