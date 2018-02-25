// PbmPixel.h
#ifndef have_PbmPixel
#define have_PbmPixel

#include "PbmPoint.h"
#include "PbmColour.h"

class PbmPixel : public PbmPoint
{
    PbmColour m_colour;

public:
    PbmPixel();
    virtual ~PbmPixel();

    PbmPixel& operator=(PbmPoint p);

    void setColour(PbmColour c)
    {
        m_colour = c;
    }

    PbmColour getColour()
    {
        return m_colour;
    }
};



#endif // have_PbmPixel

