#include "PbmPoint.h" 
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "PBitMap.h"
#include "PbmPixel.h"

PbmPoint::PbmPoint() 
{
    m_x = -1;
    m_y = -1;
}

PbmPoint::PbmPoint(int x, int y)
    : m_x(x),
      m_y(y)
{
}

PbmPoint::~PbmPoint() 
{
}

/*
we visit (0, 0) 
         (1, 0) (0, 1)                   decreasing x, increasing y, sum = 1
         (0, 2) (1, 1) (2, 0)            increasing x, decreasing y, sum = 2
         (3, 0) (2, 1) (1, 2) (0, 3)     decreasing x, increasing y, sum = 3
         (0, 4) (1, 3)...                increasing x, decreasing y, sum = 4

         (x + y) % 2 == 0 => increasing x

stack overflow at or about (276, 45)!

alternatively, cd = (x + y)(x + y + 1)/2 
               minus number of steps to the axis
               plus steps on axes.

*/
size_t PbmPoint::cantorDiagNumber() const
{
    size_t sn = (m_x + m_y) * (m_x + m_y + 1) / 2;
    int stToA = stepsToAxis();

    // xSteps + ySteps = x + y
    size_t res = sn + m_x + m_y - stToA;

    return res;
}

int PbmPoint::stepsToAxis() const
{
    int res = 0;

    if (m_x == 0)
    {
        if (yIsEven())
        {
            res = m_y;
        }
        else
        {
            res = 0;
        }
    }
    else if (m_y == 0)
    {
        if (xIsEven())
        {
            res = 0;
        }
        else
        {
            res = m_x;
        }
    }
    else // *
    {
        if (xIncreasing())
        {
            // moving to x axis, ie y = 0
            res = m_y;
        }
        else if (yIncreasing())
        {
            // moving to y axis, ie x = 0
            res = m_x;
        }
        else
        {
            assert(false); // * x and y both non zero here, contrary to hypothesis
        }
    }

    return res;
}

bool PbmPoint::lessXY(PbmPoint pp) const
{
    if (m_x < pp.m_x)
        return true;

    if (m_x == pp.m_x)
        // return m_y < pp.m_y;
        return false;

    return false;
}

bool PbmPoint::lessYX(PbmPoint pp) const
{
    if (m_y < pp.m_y)
        return true;

    if (m_y == pp.m_y)
        // return m_x < pp.m_x;
        return false;

    return false;
}

bool PbmPoint::lessX(PbmPoint pp) const
{
    if (m_x < pp.m_x)
        return true;

    return false;
}

bool PbmPoint::lessY(PbmPoint pp) const
{
    if (m_y < pp.m_y)
        return true;

    return false;
}

int PbmPoint::taxiModulus() const
{
    return abs(m_x) + abs(m_y);
}

int PbmPoint::taxiDistance(PbmPoint p) const
{
    int deltaX = m_x - p.m_x;
    int deltaY = m_y - p.m_y;

    return abs(deltaX) + abs(deltaY);
}

double PbmPoint::pythagModulus() const
{
    return sqrt((double)(m_x * m_x + m_y * m_y));
}

double PbmPoint::pythagDistance(PbmPoint p) const
{
    double deltaX = m_x - p.m_x;
    double deltaY = m_y - p.m_y;
    return sqrt((deltaX * deltaX + deltaY * deltaY));
}

int PbmPoint::XYMetric() const
{
    return abs(m_x) + abs(m_y);
}

