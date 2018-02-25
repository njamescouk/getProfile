#ifndef SPLIT_STRING_DEFINED
#define SPLIT_STRING_DEFINED


#include <vector>
#include <string>

class splitstring
{
public:
    splitstring();
    virtual ~splitstring();
    static void split (std::string &s, std::vector<std::string> &words);
    static void split (std::string &s, std::vector<std::string> &words, std::string fieldSeps);
    static void split_on_lines (std::string s, std::vector<std::string> &lines);
    static void trim(std::string &s, char c);
    static void fussy_trim(std::string &s, char c);
    static void trimws(std::string &s);
    static std::string timeStr(int secs);
    static std::string timeStrHM(int secs);
    static int secs (std::string t);
    static void subst(std::string &s, std::string sOld, std::string sNew);
    static std::string reverse(std::string s);
    static std::string toLower(std::string s);
    static bool longToStr(long l, std::string &s);

private:
    static std::string lop (std::string &s);
    static std::string lop (std::string &s, std::string fieldSeps);
};


#endif // SPLIT_STRING_DEFINED
