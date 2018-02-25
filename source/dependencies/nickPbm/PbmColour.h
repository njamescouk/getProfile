// PbmColour.h
#ifndef have_PbmColour
#define have_PbmColour

#include <cstdio>
#include <cstdlib>

// PbmColours
enum PbmBW
{
    PBIT_WHITE = 0, // PBM_WHITE
    PBIT_BLACK = 1,  // PBM_BLACK,
    PBIT_VOID
};

class PbmColour
{
    PbmBW m_bit;

public:
    PbmColour() 
    {
        m_bit = PBIT_VOID;
    }

    PbmColour(PbmBW c) 
    : m_bit(c)
    {
    }

    ~PbmColour()
    {
    }

    bool operator== (PbmColour c)
    {
        return m_bit == c.m_bit;
    }

    bool operator!= (PbmColour c)
    {
        return m_bit != c.m_bit;
    }

    void setBW(PbmBW pb)
    {
        m_bit = pb;
    }

    PbmBW getBW()
    {
        return m_bit;
    }

    void write(FILE *fp)
    {
        if (m_bit == PBIT_VOID)
        {
            fprintf(stderr, "junk in pbm, aborting\n");
            fflush(fp);
            exit(100);
        }

        // #define PBM_WHITE 0 - see pbm.h
        putc(m_bit == PBIT_WHITE ? '0' : '1', fp);
        putc(' ', fp);
    }
};

#endif // have_PbmColour

