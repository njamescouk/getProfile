// PbmPoint.h
#ifndef have_PbmPoint
#define have_PbmPoint

#pragma warning( disable : 4996 )

#include <string>
#include <cstdio>

class PbmPoint
{
    int m_x;
    int m_y;

    int stepsToAxis() const;
    
    bool xIsEven() const
    {
        return ((m_x % 2) == 0);
    }

    bool yIsEven() const
    {
        return ((m_y % 2) == 0);
    }

    bool sumIsEven() const
    {
        return (((m_x + m_y) % 2) == 0);
    }

    bool xIncreasing() const
    {
        return sumIsEven();
    }

    bool yIncreasing() const
    {
        return !sumIsEven();
    }

public:
    class XCompare
    {
    public:
        bool operator()(PbmPoint p1, PbmPoint p2)
        {
            return p1.lessX(p2);
        }
    };

    class YCompare
    {
    public:
        bool operator()(PbmPoint p1, PbmPoint p2)
        {
            return p1.lessY(p2);
        }
    };

    class TaxiCompare
    {
    public:
        bool operator()(PbmPoint p1, PbmPoint p2)
        {
            if (p1.taxiModulus() == p2.taxiModulus())
            {
                return p1.getX() < p2.getX();
            }

            return p1.taxiModulus() < p2.taxiModulus();
        }
    };

    class PythagCompare
    {
    public:
        bool operator()(PbmPoint p1, PbmPoint p2)
        {
            if (p1.pythagModulus() == p2.pythagModulus())
            {
                return p1.getX() < p2.getX();
            }

            return p1.pythagModulus() < p2.pythagModulus();
        }
    };

    PbmPoint();
    PbmPoint(int x, int y);

    virtual ~PbmPoint();

    /*
    static const int eastLimit;
    static const int southLimit;
    */
    void setX(int i)
    {
        m_x = i;
    }

    void setY(int i)
    {
        m_y = i;
    }

    int getX() const
    {
        return m_x;
    }

    int getY() const
    {
        return m_y;
    }

    PbmPoint translate(int xOffset, int yOffset)
    {
        PbmPoint res = *this;

        res.setX(m_x + xOffset);
        res.setY(m_y + yOffset);

        return res;
    }

    size_t cantorDiagNumber() const;
    bool lessXY(PbmPoint pp) const;
    bool lessYX(PbmPoint pp) const;
    bool lessX(PbmPoint pp) const;
    bool lessY(PbmPoint pp) const;

    double pythagModulus() const;
    double pythagDistance(PbmPoint p) const;

    int taxiModulus() const;
    int taxiDistance(PbmPoint p) const;

    int XYMetric() const;

    PbmPoint pointNorth()
    {
        PbmPoint res;
        res.setX(m_x);
        res.setY(m_y - 1);

        return res; 
    }

    PbmPoint pointSouth()
    {
        PbmPoint res;
        res.setX(m_x);
        res.setY(m_y + 1);

        return res;
    }
    
    PbmPoint pointEast()
    {
        PbmPoint res;
        res.setX(m_x + 1);
        res.setY(m_y);

        return res;
    }
    
    PbmPoint pointWest()
    {
        PbmPoint res;
        res.setX(m_x - 1);
        res.setY(m_y);

        return res;
    }
    
    std::string toString() const
    {
        char s[BUFSIZ]; // going for largish buffer overflow here
        sprintf (s, "(%d, %d)", m_x, m_y);
        std::string res = s;

        return res;
    }
};



#endif // have_PbmPoint

