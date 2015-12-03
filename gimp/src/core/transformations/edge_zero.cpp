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

    EdgeLaplaceOfGauss edgeLaplaceOfGauss(image);
    edgeLaplaceOfGauss.setParameter("size",size);
    edgeLaplaceOfGauss.setParameter("sigma",sigma);

    Transformation laplasian(edgeLaplaceOfGauss.transform());
    float v_o = 128;

    if (image->format() == QImage::Format_Indexed8)
        {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {


        math::matrix<float> oknoL = laplasian.getWindow(x,y,size,LChannel,RepeatEdge);

        int max_l = 0;

        int min_l = PIXEL_VAL_MAX;


        for(int w = 0; w<size; w++){
              for(int e = 0; e<size; e++){
                  if(oknoL(w,e) > max_l){
                      max_l = oknoL(w,e);
                  }


                  if(oknoL(w,e) < min_l){
                      min_l = oknoL(w,e);
                  }


              }

        }


        if (min_l < (v_o - t) && max_l > (v_o + t))
                        {
                            newImage->setPixel(x, y, oknoL(size / 2, size / 2));
                        }
                        else
                        {
                            newImage->setPixel(x, y, 0);
                        }

}
return newImage;

    }
    else{

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {

            math::matrix<float> oknoR = laplasian.getWindow(x,y,size,RChannel,RepeatEdge);
            math::matrix<float> oknoG = laplasian.getWindow(x,y,size,GChannel,RepeatEdge);
            math::matrix<float> oknoB = laplasian.getWindow(x,y,size,BChannel,RepeatEdge);

            int max_r = 0;
            int max_g = 0;
            int max_b = 0;

            int min_r = PIXEL_VAL_MAX;
            int min_g = PIXEL_VAL_MAX;
            int min_b = PIXEL_VAL_MAX;


            for(int w = 0; w<size; w++){
                  for(int e = 0; e<size; e++){
                      if(oknoR(w,e) > max_r){
                          max_r = oknoR(w,e);
                      }
                      if(oknoG(w,e) > max_g){
                          max_g = oknoG(w,e);
                      }
                      if(oknoB(w,e) > max_b){
                          max_b = oknoB(w,e);
                      }


                      if(oknoR(w,e) < min_r){
                          min_r = oknoR(w,e);
                      }
                      if(oknoG(w,e) < min_g){
                          min_g = oknoG(w,e);
                      }
                      if(oknoB(w,e) < min_b){
                          min_b = oknoB(w,e);
                      }

                  }

            }



            if (min_r < (v_o - t) && max_r > (v_o + t))
                            {
                                newImage->setPixel(x, y, oknoR(size / 2, size / 2));
                            }
                            else if (min_g < (v_o - t) && max_g > (v_o + t))
                            {
                                newImage->setPixel(x, y, oknoG(size / 2, size / 2));
                            }
                            else if (min_b < (v_o - t) && max_b > (v_o + t))
                            {
                                newImage->setPixel(x, y, oknoB(size / 2, size / 2));
                            }
                            else
                            {
                                newImage->setPixel(x, y, 0);
                            }
        }


    return newImage;
    }
}
