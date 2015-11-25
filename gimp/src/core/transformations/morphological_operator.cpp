#include "morphological_operator.h"

MorphologicalOperator::MorphologicalOperator(PNM* img) :
    Transformation(img)
{
}

MorphologicalOperator::MorphologicalOperator(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

// abstract
const int MorphologicalOperator::morph(math::matrix<float>, math::matrix<bool>)
{
    return 0;
}

math::matrix<bool> MorphologicalOperator::getSE(int size, SE shape)
{
    switch (shape)
    {
    case Square:    return seSquare(size);
    case Cross:     return seCross(size);
    case XCross:    return seXCross(size);
    case VLine:     return seVLine(size);
    case HLine:     return seHLine(size);
    default:        return seSquare(size);
    }
}


math::matrix<bool> MorphologicalOperator::seSquare(int size)
{
    //size jest nieparzysty oraz >= 3
    math::matrix<bool> ret(size, size);


    // wypełnienie figury wartością true - pełny kwadrat
    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){

            ret(i,j) = true;

        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seCross(int size)
{
    math::matrix<bool> ret(size, size);


    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){

            ret(i,j) = false;

            if ((i == size/2) || (j == size/2))
                ret(i,j) = true;

        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seXCross(int size)
{
    math::matrix<bool> ret(size, size);


    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){

            ret(i,j) = false;

            if ((i == j) || (size-i-1 == j))
                ret(i,j) = true;

        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seVLine(int size)
{
    math::matrix<bool> ret(size, size);

    // linia pionowa
    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){

            ret(i,j) = false;

            if (j == size/2)
                ret(i,j) = true;

        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seHLine(int size)
{
    math::matrix<bool> ret(size, size);


    // linia pozioma
    for(int i=0; i<size; ++i){
        for(int j=0; j<size; ++j){

            ret(i,j) = false;

            if (i == size/2)
                ret(i,j) = true;

        }
    }


    return ret;
}

PNM* MorphologicalOperator::transform()
{  
    int size  = getParameter("size").toInt();
    SE  shape = (MorphologicalOperator::SE) getParameter("shape").toInt();

    PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);


    int width  = image->width();
    int height = image->height();


    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {

            math::matrix<float> oknoR = Transformation::getWindow(x,y,size,RChannel,RepeatEdge);
            math::matrix<float> oknoG = Transformation::getWindow(x,y,size,GChannel,RepeatEdge);
            math::matrix<float> oknoB = Transformation::getWindow(x,y,size,BChannel,RepeatEdge);

            QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            int r = morph(oknoR, getSE(size, shape));
            int g = morph(oknoG, getSE(size, shape));
            int b = morph(oknoB, getSE(size, shape));

            QColor newPixel = QColor(r,g,b);
            newImage->setPixel(x,y, newPixel.rgb());

        }



    return newImage;
}
