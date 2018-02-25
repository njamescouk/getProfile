#include "PBitMap.h" 
#include "PbmPixel.h"
#include <queue>
#include <set>

bool PBitMap::read(FILE *fp)
{
    pbm_readpbm(fp, this);

    return true;
}

bool PBitMap::write(FILE *fp)
{
    int cols = 0;
    int rows = 0;
    pbm_writepbm (fp, this);

    return rows == getHeight() && cols == getWidth(); // duh
}

// write (x,y) of pixels of colour
void PBitMap::writeCoords(FILE *fp, PbmBW colour)
{
    size_t row;
    for (row = 0; row < m_rows.size(); row++)
    {
        bool rowIsEmpty = true;

        PBitMapRow pRow = m_rows[row];
        size_t col;
        for (col = 0; col < (size_t)pRow.getWidth(); col++)
        {
            PbmColour pix = pRow.getBW(col);
            if (pix.getBW() == colour)
            {
                fprintf(fp, "(%ld, %ld) ", col, row);
                rowIsEmpty = false;
            }
        }

        if (!rowIsEmpty)
        {
            fprintf(fp, "\n");
        }
    }
}

PbmPixel PBitMap::getPixel(PbmPoint p)
{
    PbmPixel res;

    if (contains(p))
    {
        res = p;
        res.setColour(getColour(p));
    }

    return res;
}

void PBitMap::setPixel(PbmPixel px)
{
    if (contains((PbmPoint)px))
    {
        PBitMapRow& r = getRowRef(px.getY());
        r.setBW(px.getX(), px.getColour());
    }
}


PbmColour PBitMap::getColour(PbmPoint p)
{
    PbmColour res;

    if (contains(p))
    {
        PBitMapRow r = getRow(p.getY());
        res = r[p.getX()];
    }

    return res;
}

int PBitMap::floodFillR(PbmPoint p, PbmBW target)
{
    if (getColour(p).getBW() != target)
        return 0;

    if (!contains(p))
        return 0;

    fprintf(stderr, "%d, %d\n", p.getX(), p.getY());

    PbmPixel px;
    (PbmPoint)px = p;
    px.setColour(target == PBIT_WHITE ? PBIT_BLACK : PBIT_WHITE);
    setPixel(px);

    int res = 0;
    res += floodFillR(p.pointNorth(), target);
    res += floodFillR(p.pointSouth(), target);
    res += floodFillR(p.pointEast(), target);
    res += floodFillR(p.pointWest(), target);

    return res;
}

int PBitMap::floodFill(PbmPoint p, PbmBW target)
{

    if (!contains(p))
        return 0;

    // fprintf(stderr, "%d, %d\n", p.getX(), p.getY());
    int res = 0;

    // 1
    std::queue<PbmPoint> qPoint;
    // 2
    if (getColour(p).getBW() != target)
        return 0;
    // 3
    qPoint.push(p);
    // 4
    while(!qPoint.empty())
    {
        // 5
        PbmPoint cur = qPoint.front();
        // fprintf(stderr, "%d, %d          \r", cur.getX(), cur.getY());
        // 6
        if (getColour(p).getBW() == target)
        {
            toggleColour(cur);
            res++;
        }
        // 7
        qPoint.pop();

        PbmPoint pNew;
        // 8
        pNew = cur.pointWest();
        if (getColour(pNew).getBW() == target)
        {
            // 9
            toggleColour(pNew);
            res++;
            // 9
            qPoint.push(pNew);
        }

        pNew = cur.pointEast();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            qPoint.push(pNew);
        }

        pNew = cur.pointNorth();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            qPoint.push(pNew);
        }

        pNew = cur.pointSouth();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            qPoint.push(pNew);
        }
    }

    return res;
}

int PBitMap::floodFillS(PbmPoint p, PbmBW target)
{

    if (!contains(p))
        return 0;

    // fprintf(stderr, "%d, %d\n", p.getX(), p.getY());
    int res = 0;

    // 1
    std::set<PbmPoint, PbmPoint::PythagCompare> sPoint;
    // 2
    if (getColour(p).getBW() != target)
        return 0;
    // 3
    sPoint.insert(p);
    // 4
    while(!sPoint.empty())
    {
        // 5
        std::set<PbmPoint, PbmPoint::PythagCompare>::iterator it = sPoint.begin();
        PbmPoint cur = *it;
        // fprintf(stderr, "%d, %d          \r", cur.getX(), cur.getY());
        // 6
        if (getColour(p).getBW() == target)
        {
            toggleColour(cur);
            res++;
        }
        // 7
        sPoint.erase(it);

        PbmPoint pNew;
        // 8
        pNew = cur.pointWest();
        if (getColour(pNew).getBW() == target)
        {
            // 9
            toggleColour(pNew);
            res++;
            // 9
            sPoint.insert(pNew);
        }

        pNew = cur.pointEast();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            sPoint.insert(pNew);
        }

        pNew = cur.pointNorth();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            sPoint.insert(pNew);
        }

        pNew = cur.pointSouth();
        if (getColour(pNew).getBW() == target)
        {
            toggleColour(pNew);
            res++;
            sPoint.insert(pNew);
        }
    }

    return res;
}

void PBitMap::toggleColour(PbmPoint p)
{
    PbmColour extgCol = getColour(p);
    PbmPixel px;
    (PbmPoint)px = p;
    px.setColour(extgCol == PBIT_WHITE ? PBIT_BLACK : PBIT_WHITE);
    setPixel(px);
}

