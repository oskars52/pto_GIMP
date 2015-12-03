#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();

    int    size  = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    math::matrix<float> laplasjan_gausowski_wejsciowego_obrazu(size, size);
    //laplasjan_gausowski_wejsciowego_obrazu = EdgeLaplaceOfGauss::getMask(size, sigma);
    float v_o = 128;

    convolute(laplasjan_gausowski_wejsciowego_obrazu, RepeatEdge);

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {

        }


    return newImage;
}

