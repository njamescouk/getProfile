// PBitMap.h
#ifndef have_PBitMap
#define have_PBitMap

#include <vector>
#include "pbm.h" 
#include "PBitMapRow.h" 
#include "PbmColour.h"
#include "PbmPoint.h"

class PbmPixel;

/*
portable bitmap m_height x m_width pixels
*/
class PBitMap
{
    int m_width;
    std::vector<PBitMapRow> m_rows;

public:
    PBitMap()
    {
        m_width = 0;
    }

    virtual ~PBitMap()
    {
        m_width = 0;
    }

    int getHeight()
    {
        return m_rows.size();
    }

    std::vector<PBitMapRow> getRows()
    {
        return m_rows;
    }

    PBitMapRow& getRowRef(int i)
    {
        return m_rows[i];
    }

    PBitMapRow getRow(int i)
    {
        return m_rows[i];
    }

    void setSize(int width, int height, PbmBW background = PBIT_VOID)
    {
        m_width = width;
        m_rows.resize(height);

        int i;
        for (i = 0; i < height; i++)
        {
            m_rows[i].setWidth(width, background);
        }
    }

    int getWidth() 
    {
        if (m_rows.size() > 0)
            return m_rows[0].getWidth();

        return 0;
    }

    bool contains(PbmPoint p)
    {
        if (p.getX() < 0 || p.getX() >= getWidth())
            return false;

        if (p.getY() < 0 || p.getY() >= getHeight())
            return false;

        return true;
    }

    PbmPixel getPixel(PbmPoint p);
    void setPixel(PbmPixel px);

    PbmColour getColour(PbmPoint p);
    int floodFillR(PbmPoint p, PbmBW target);
    int floodFill(PbmPoint p, PbmBW target);
    int floodFillS(PbmPoint p, PbmBW target);

    bool read(FILE *fp);
    bool write(FILE *fp);
    void writeCoords(FILE *fp, PbmBW colour);
    void toggleColour(PbmPoint p);
};



#endif // have_PBitMap

