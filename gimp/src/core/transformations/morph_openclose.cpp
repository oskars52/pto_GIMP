#include "morph_openclose.h"

#include "morph_erode.h"
#include "morph_dilate.h"

MorphOpenClose::MorphOpenClose(PNM* img) :
    MorphologicalOperator(img), m_type(Open)
{
}

MorphOpenClose::MorphOpenClose(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv), m_type(Open)
{
}

PNM* MorphOpenClose::transform()
{
    int size = getParameter("size").toInt();;
    SE shape = (SE) getParameter("shape").toInt();
    m_type = (Type) getParameter("type").toInt();

     PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);

    if(m_type == Open){

        newImage = dilate(image,size,shape);
        newImage = erode(newImage,size,shape);


    } else {

        newImage = erode(image,size,shape);
        newImage = dilate(newImage,size,shape);

    }


    return newImage;
}

PNM* MorphOpenClose::erode(PNM* image, int size, SE shape)
{
    MorphErode e(image, getSupervisor());
    e.setParameter("silent", true);
    e.setParameter("shape", shape);
    e.setParameter("size", size);

    return e.transform();
}

PNM* MorphOpenClose::dilate(PNM* image, int size, SE shape)
{
    MorphDilate e(image, getSupervisor());
    e.setParameter("silent", true);
    e.setParameter("shape", shape);
    e.setParameter("size", size);

    return e.transform();
}
