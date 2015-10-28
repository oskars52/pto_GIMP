#include "histogram.h"

#include <QDebug>
#include <QPainter>

#include <cmath>

Histogram::Histogram(QImage* image)
{
    R = new QHash<int, int>;
    G = new QHash<int, int>;
    B = new QHash<int, int>;
    L = new QHash<int, int>;
    generate(image);
}

Histogram::~Histogram()
{
    delete R;
    delete G;
    delete B;
    delete L;
}

void Histogram::generate(QImage* image)
{


    int width  = image->width();
    int height = image->height();



    if (image->format() == QImage::Format_Mono)
    {

       //

    }
    else if (image->format() == QImage::Format_Indexed8)
    {

        for (int x=0; x<256; x++)
        {

            L->value(x,0);

        }

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {

                QRgb pixel = image->pixel(x,y);

                int v = qGray(pixel);    // Get the 0-255 value of the L channel

                L->insert(v,L->value(v)+1);

            }


    }
    else //if (image->format() == QImage::Format_RGB32)
    {


        for (int x=0; x<256; x++)
        {

            R->value(x,0);
            G->value(x,0);
            B->value(x,0);
        }

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);

                R->insert(r,R->value(r)+1);
                G->insert(g,G->value(g)+1);
                B->insert(b,B->value(b)+1);

            }

    }


}

/** Returns the maximal value of the histogram in the given channel */
int Histogram::maximumValue(Channel selectedChannel = RGB)
{

    QHash<int, int>::const_iterator i;
    int maxxx = -10;


    if(selectedChannel == RChannel){

        for (i = R->constBegin(); i != R->constEnd(); ++i){
             if(maxxx < i.value()){
               maxxx = i.value();
             }
        }

    }

    if(selectedChannel == GChannel){

        for (i = G->constBegin(); i != G->constEnd(); ++i){
             if(maxxx < i.value()){
               maxxx = i.value();
             }
        }

    }

    if(selectedChannel == BChannel){

        for (i = B->constBegin(); i != B->constEnd(); ++i){
             if(maxxx < i.value()){
               maxxx = i.value();
             }
        }

    }

    if(selectedChannel == LChannel){

        for (i = L->constBegin(); i != L->constEnd(); ++i){
             if(maxxx < i.value()){
               maxxx = i.value();
             }
        }

    }

//    if(selectedChannel == LChannel){
//       int ll = Histogram::maximumValue(LChannel);
//       if(maxxx < ll){
//         maxxx = ll;
//       }


//    }

     if(selectedChannel == RGB){
        int rr = Histogram::maximumValue(RChannel);
        if(maxxx < rr){
          maxxx = rr;
        }
        int gg = Histogram::maximumValue(GChannel);
        if(maxxx < gg){
          maxxx = gg;
        }
        int bb = Histogram::maximumValue(BChannel);
        if(maxxx < bb){
          maxxx = bb;
        }

     }

    return maxxx;









}


/** Returns a pointer to the given channel QHash<int, int> */
QHash<int, int>* Histogram::get(Channel channel = LChannel)
{
    if (channel==LChannel) return L;
    if (channel==RChannel) return R;
    if (channel==GChannel) return G;
    if (channel==BChannel) return B;

    return 0;
}

/**
 *  Returns a 255 by 100 QImage containing a Histogram for the given channel.
 *  The background is transparent (Alpha 0, RGB=255) */
QImage Histogram::getImage(Channel channel = LChannel, QBrush pen = Qt::gray)
{
    // Create blank QImage and fill it with transparent background:
    QImage histImage(255, 100, QImage::Format_ARGB32);
    histImage.fill(0);
    QPainter painter(&histImage);
    painter.setBrush(Qt::transparent); 
    painter.setPen(Qt::transparent);
    painter.drawRect(0,0,255,100);

    // Calculate the aspect ratio using the maximal value of the color histograms
    int maximum = (channel == LChannel ? maximumValue(LChannel) :  maximumValue(RGB));
    float ratio = 100.0/float(maximum);

    // Preparing the painter:
    painter.setBrush(pen);
    painter.setPen(pen.color());

    int h;
    // Draw histogram
    QHash<int, int>* hist = get(channel);
    QHash<int, int>::const_iterator cit = hist->begin();

    while (cit != hist->end())
    {
        h = 100 - floor(ratio*cit.value());
        painter.drawLine(cit.key(), h, cit.key(), 100);
        ++cit;
    }

    return histImage;
}
