#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);

    float suma = 0;

    for(int i=0; i<maskSize*maskSize; ++i){
        suma += tmpMask.at(i).toDouble();
    }

    for(int i=0; i<maskSize; ++i){
        for(int q=0; q<maskSize; ++q){
            if(normalize){
                    mask(i,q)= (tmpMask.at(i*maskSize+q).toDouble()/suma);

            } else {
                    mask(i,q)= (tmpMask.at(i*maskSize+q).toDouble());

            }
        }

    }




    return convolute(mask, RepeatEdge);
}
