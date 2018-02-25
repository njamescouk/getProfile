// PBitMapRow.h
#ifndef have_PBitMapRow
#define have_PBitMapRow

#include <vector>
#include "PbmColour.h"

class PBitMapRow : public std::vector<PbmColour>
{

public:
    PBitMapRow()
    {
    }

    virtual ~PBitMapRow()
    {
    }

    PbmColour& operator[](size_t i)
    {
        return at(i);
    }

    // "allocator" for width pixels
    void setWidth(int width, PbmBW background = PBIT_VOID)
    {
        clear();
        PbmColour col;
        col.setBW(background);
        resize(width, col);
    }

    void setBWs(std::vector<PbmColour> pxx)
    {
        if (pxx.size() == size())
        {
            (std::vector<PbmColour>)*this = pxx;
        }
        else
        {
            clear();
        }
    }

    // write row to fp
    void write(FILE *fp)
    {
        size_t i;
        for (i = 0; i < size(); i++)
        {
            at(i).write(fp);
        }
        fprintf(fp, "\n");
    }

    int getWidth()
    {
        return size();
    }

    PbmColour getBW(int i)
    {
        return at(i);
    }

    void setBW(int col, PbmColour bw)
    {
        at(col) = bw;
    }

    // return index of first pixel of colour bw in row or -1
    int hasColour(PbmColour bw)
    {
        int res = -1;

        size_t i;
        for (i = 0; i < size(); i++)
        {
            PbmColour curCol = at(i).getBW();
            if (curCol == bw)
            {
                res = i;
                break;
            }
        }

        return res;
    }
};



#endif // have_PBitMapRow

