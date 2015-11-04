#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);

    for(int i=0; i<size; ++i){

        for(int q=0; q<size; ++q){
            mask(i,q)=0;
            if(i==(size/2) && q==(size/2)){
                mask(i,q)=1;
            }
        }

    }

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    int size = mask.rowno();

    float weight = Convolution::sum(mask);



    if (image->format() == QImage::Format_Indexed8)
    {

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {

                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int v = qGray(pixel);    // Get the 0-255 value of the L channel

                math::matrix<float> oknoL = Transformation::getWindow(x,y,size,LChannel,mode);

                math::matrix<float> akumulatorL =  Convolution::join(Convolution::reflection(mask),oknoL);

                float sumL = Convolution::sum(akumulatorL);

                if (weight != 0){
                    sumL = sumL/weight;

                }

                if (sumL > 255){
                    sumL = 255;
                }

                if (sumL < 0){
                    sumL = 0;
                }

                newImage->setPixel(x,y, sumL);
            }

    }
    else //if (image->format() == QImage::Format_RGB32)
    {


        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                math::matrix<float> oknoR = Transformation::getWindow(x,y,size,RChannel,mode);
                math::matrix<float> oknoG = Transformation::getWindow(x,y,size,GChannel,mode);
                math::matrix<float> oknoB = Transformation::getWindow(x,y,size,BChannel,mode);


                math::matrix<float> akumulatorR =  Convolution::join(Convolution::reflection(mask),oknoR);
                math::matrix<float> akumulatorB =  Convolution::join(Convolution::reflection(mask),oknoB);
                math::matrix<float> akumulatorG =  Convolution::join(Convolution::reflection(mask),oknoG);

                float  sumR = Convolution::sum(akumulatorR);
                float  sumG = Convolution::sum(akumulatorG);
                float  sumB = Convolution::sum(akumulatorB);

                if (weight != 0){
                    sumR = sumR/weight;
                    sumG = sumG/weight;
                    sumB = sumB/weight;
                }

                if (sumR > 255){
                    sumR = 255;
                }
                if (sumG > 255){
                    sumG = 255;
                }
                if (sumB > 255){
                    sumB = 255;
                }

                if (sumR < 0){
                    sumR = 0;
                }
                if (sumG < 0){
                    sumG = 0;
                }
                if (sumB < 0){
                    sumB = 0;
                }

                QColor newPixel = QColor(sumR,sumG,sumB);
                newImage->setPixel(x,y, newPixel.rgb());
            }

    }


    return newImage;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for(int i=0; i<size; ++i){

        for(int q=0; q<size; ++q){
           C(i,q)=A(i,q)*B(i,q);
        }

    }


    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;
int size = A.rowno();

    for(int i=0; i<size; ++i){

        for(int q=0; q<size; ++q){
           sum += A(i,q);
        }

    }

    return sum;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for(int i=0; i<size; ++i){

        for(int q=0; q<size; ++q){
           C(size-i-1, size-q-1) = A(i,q);
        }

    }



    return C;
}
