#include "PBitMap.h"
#include <cstdio>
#include <map>
#include "CmdLineParser.h" 

char *progname = 0;
char *szVersion = "1.0";
 
extern std::map<std::string, OptionSpec> cmdlineLexingSpecs;
void usage(FILE *fp);
void version(FILE *fp);

#pragma warning( disable : 4996 )

enum GET_PROFILE_STATE
{
    GET_PROFILE_BEGIN,
    GET_PROFILE_PROFILING,
    GET_PROFILE_STOP
};

struct RH
{
    RH(int r, int h)
        : radius(r)
        , height(h)
    {
    }

    int radius;
    int height;

    void write(FILE *fp)
    {
        fprintf (fp, "%d,%d\n", radius, height);
    }
};

struct RHArray : public std::vector<RH>
{
    void write(FILE *fp)
    {
        fprintf (fp, "\"radius\",\"height\"\n");
        size_t i;
        for (i = 0; i < size(); i++)
        {
            at(i).write(fp);
        }
    }
};

RHArray getRadiusAndHeight(PBitMap pbm);

int main (int argc, char * argv[])
{
    progname = argv[0];
    std::map<std::string, OptionSpec> specs;

    std::string blurb = progname;
    blurb += " profile_pbm_file\ncwrites a table of values\n"
             "of radius and height for the profile_pbm_file\n";

    specs["h"] = OptionSpec(false, "this help");
    specs["v"] = OptionSpec(false, "version");
    CmdLineSpec cls(specs);
    cmdlineLexingSpecs = specs;

    pcldebug = 0;
    CmdLineParser clp(argc, argv, cls);

    Cmdline cmdline = clp.parse();
    if (!cmdline.isValid())
    {
        cls.help(blurb, stdout);
        return 1;
    }

    if (cmdline.hasOption("v"))
    {
        version(stdout);
        return 0;
    }

    if (cmdline.hasOption("h"))
    {
        cls.help(blurb, stdout);
        return 0;
    }

    FILE *fp = stdin;
    std::string inputFile;
    if (cmdline.numArguments() > 0)
    {
        inputFile = cmdline.getArgument(0);
        fp = fopen(inputFile.c_str(), "rt");
        if (fp == 0)
        {
            perror(progname);
            return 2;
        }
    }

    PBitMap pbm;
    bool res = pbm.read(fp);
    
    if (fp != stdin)
    {
        fclose(fp);
    }

    if (!res)
    {
        return 3;
    }

    RHArray rh = getRadiusAndHeight(pbm);
    rh.write(stdout);

    return 0;
}

/*
    3 states:
       1. seeing all white rows                  = GET_PROFILE_BEGIN
       2. encounter first row with black pixel   = GET_PROFILE_PROFILING
       3. encounter all white row                = GET_PROFILE_STOP
          encounter black pixel in column 0      = GET_PROFILE_STOP

    stay in GET_PROFILE_BEGIN until we get a black pixel
        move to GET_PROFILE_PROFILING, stay there until black in column 0
            move to GET_PROFILE_STOP
*/
RHArray getRadiusAndHeight(PBitMap pbm)
{
    GET_PROFILE_STATE state = GET_PROFILE_BEGIN;
    RHArray res;
    int lastRow = pbm.getHeight() - 1;

    int row;
    for (row = 0; row < pbm.getHeight() && state != GET_PROFILE_STOP; row++)
    {
        bool rowIsEmpty = true;

        PBitMapRow pRow = pbm.getRow(row);
        int blackPos = pRow.hasColour(PBIT_BLACK);
        bool haveBlack = (blackPos >= 0);
        bool haveBlackInColZero = (blackPos == 0);

        if (haveBlack)
        {
            if (state == GET_PROFILE_BEGIN)
            {
                state = GET_PROFILE_PROFILING;
            }
        }
        else // row is all white
        {
            if (state == GET_PROFILE_PROFILING)
            {
                state = GET_PROFILE_STOP;
            }
        }

        if (state == GET_PROFILE_PROFILING)
        {
            int rowFromBottom = lastRow - row;
            res.push_back(RH(blackPos, rowFromBottom));

            if (haveBlackInColZero)
            {
                state = GET_PROFILE_STOP;
            }
        }
    }

    return res;
}

void usage(FILE *fp)
{
    fprintf (fp, "usage %s [-d]  profile_pbm computes a table of values\n", progname);
    fprintf (fp, "of radius and path length for the pbm profile\n");
    fprintf (fp, "-d  dump AHRN\n");
}

void version(FILE *fp)
{
    fprintf (fp, "%s version %s\n", progname, szVersion);
}
