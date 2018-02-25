#include "PbmPixel.h" 

PbmPixel::PbmPixel() 
    : PbmPoint(-1, -1),
      m_colour(PBIT_BLACK)
{
}

PbmPixel::~PbmPixel() 
{
}

PbmPixel& PbmPixel::operator=(PbmPoint p)
{
    setX(p.getX());
    setY(p.getY());
    // m_colour = PBIT_VOID;

    return *this;
}

