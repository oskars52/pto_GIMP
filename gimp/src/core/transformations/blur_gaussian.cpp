#include "blur_gaussian.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    for(int i=0; i<size; ++i){
        for(int q=0; q<size; ++q){

            mask(i,q)=BlurGaussian::getGauss(i-(size/2),q-(size/2),sigma);

        }

    }

    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    

    float a = (1/(2*M_PI*sigma*sigma))* exp( (-1)*((x*x + y*y)/(2*sigma*sigma)) );


    return a;
}

