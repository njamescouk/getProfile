#pragma warning (disable: 4786)

#include "split_string.h"
#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable: 4996)

splitstring::splitstring()
{
    ;
}

splitstring::~splitstring()
{
    ;
}

/*
    split s on whitespace, results in words
    repeatedly remove whitespace from beginning of string
    find end of word and put it in words

    note std::string &s - why?
*/
void splitstring::split (std::string &s, std::vector<std::string> &words)
{
    std::string trimmed;
    size_t start = s.find_first_not_of(" \t\n");
    if (start != std::string::npos)
    {
        trimmed = s.substr(start);
        std::string head = lop(trimmed);
        if (head.length() > 0)
            words.push_back(head);
        split(trimmed, words);
    }
}

/*
    split s on any char in fieldSeps, results in words
    repeatedly remove whitespace from beginning of string
    find end of word and put it in words

    note std::string &s - why?
*/
void splitstring::split (std::string &s, std::vector<std::string> &words, std::string fieldSeps)
{
    std::string trimmed;
    size_t start = s.find_first_not_of(fieldSeps);
    if (start != std::string::npos)
    {
        trimmed = s.substr(start);
        std::string head = lop(trimmed, fieldSeps);
        if (head.length() > 0)
            words.push_back(head);
        split(trimmed, words, fieldSeps);
    }
}

// s has no leading space return first word
// chop first word off s, leaving space
std::string splitstring::lop (std::string &s)
{
    std::string res = "";
    size_t start_of_space = s.find_first_of(" \t\n");
    // this is the first space after the first word
    if (start_of_space != std::string::npos)
    {
        // put first word in res
        res = s.substr(0, start_of_space);
        // lop first word off s
        s = s.substr(start_of_space+1);
    }
    else
    {
        // no space at end, return the word
        res = s;
        s = "";
    }

    return res;
}

// s has no leading fieldSep
// return first word
// chop first word off s, leaving space
// another way of putting it, return head, assign tail to s
std::string splitstring::lop (std::string &s, std::string fieldSeps)
{
    std::string res = "";
    size_t start_of_sep = s.find_first_of(fieldSeps);
    // this is the first space after the first word
    if (start_of_sep != std::string::npos)
    {
        // put first word in res
        res = s.substr(0, start_of_sep);
        // lop first word off s
        s = s.substr(start_of_sep+1);
    }
    else
    {
        // no space at end, return the word
        res = s;
        s = "";
    }

    return res;
}

/*
    split s on newlines, results in lines
    find next newline or end of string, add head to lines
*/
void splitstring::split_on_lines (std::string s, std::vector<std::string> &lines)
{
    lines.resize(0);

    std::string tail;
    size_t start = s.find_first_of("\n");

    while (start != std::string::npos)
    {
        if (start + 1 < s.length())
        {
            tail = s.substr(start+1);

            std::string head = s.substr(0, start+1);
            if (head.length() > 0)
                lines.push_back(head);
        
            s = tail;
        }
        else
        {
            lines.push_back(s);
            s = "";
            break;
        }

        start = s.find_first_of("\n");
    }

    if (s.length() > 0)
        lines.push_back(s);
}

// remove c from beginning and end of s
void splitstring::trim(std::string &s, char c)
{
    std::string res = s;
    if (res[0] == c)
        s = res.substr(1);

    res = s;
    size_t len = res.length();

    if (res[len - 1] == c)
        s = res.substr(0, len - 1);
}

// remove c from beginning and end of s if it exists at both ends
void splitstring::fussy_trim(std::string &s, char c)
{
    size_t len = s.length();
    if (len < 1)
        return;

    if (s[0] == c && s[len - 1] == c)
        s = s.substr(1, len - 2);
}

// remove whitespace from beginning and end of s
void splitstring::trimws(std::string &s)
{
    if (s.empty())
        return;

    const char *wsChars = " \t\r\n";
    size_t beg = s.find_first_not_of(wsChars),
           end = s.find_last_not_of(wsChars);

    if (beg == std::string::npos)
        s = "";
    else
        s = s.substr(beg, end - beg + 1);
}

//  return string containing secs rendered as hh:mm:ss
std::string splitstring::timeStr(int secs)
{
    int m = secs/60;
    int s = secs%60;
    int h = m/60;
    m = m%60;
    
    char buf[BUFSIZ];
    #ifdef WIN32
    _snprintf(buf, BUFSIZ-1, "%.2d:%.2d:%.2d", h, m, s);
    #else
    snprintf(buf, BUFSIZ-1, "%.2d:%.2d:%.2d", h, m, s);
    #endif
    std::string str = buf;

    return str;
}

//  return string containing secs rendered as hh:mm
std::string splitstring::timeStrHM(int secs)
{
    int m = secs/60;
    int h = m/60;
    m = m%60;
    
    char buf[BUFSIZ];
    #ifdef WIN32
    _snprintf(buf, BUFSIZ-1, "%.2d:%.2d", h, m);
    #else
    snprintf(buf, BUFSIZ-1, "%.2d:%.2d", h, m);
    #endif
    std::string str = buf;

    return str;
}

// how many seconds is t, which is in hh:mm:ss format?
int splitstring::secs (std::string t)
{
    std::vector<std::string> fields;

    splitstring::split(t, fields, ":");
    if (fields.size() != 3)
        return -1;
        
    int h = atoi(fields[0].c_str());
    int m = atoi(fields[1].c_str());
    int s = atoi(fields[2].c_str());
    int res = (((h * 60) + m) * 60) + s;
        
    return res;
}

// the problem here is to not do recursive replacement
void splitstring::subst(std::string &s, std::string sOld, std::string sNew)
{
    if (sOld.empty())
        return;  // nothing to do

    size_t soughtpos = 0;
    while(1)
    {
        soughtpos = s.find(sOld, soughtpos);
        if (soughtpos != std::string::npos)
        {
            s.replace(soughtpos, sOld.length(), sNew);
            soughtpos += sNew.size();
        }
        else
        {
            break;
        }
    }
}

std::string splitstring::reverse(std::string s)
{
    size_t len = s.length();

    if (len == 0)
        return "";

    std::string res(len, ' ');

    size_t i;
    for (i = 0; i < len; i++)
    {
        res[i] = s[len - i - 1];
    }

    return res;
}

std::string splitstring::toLower(std::string s)
{
    std::string res = "";

    size_t i;
    for (i = 0; i < s.length(); i++)
        res += (char)tolower(s[i]);

    return res;
}

bool splitstring::longToStr(long l, std::string &s)
{
    char buf[BUFSIZ];
#ifdef WIN32
    int ccount = _snprintf(buf, BUFSIZ-1, "%ld", l);
#else
    int ccount = snprintf(buf, BUFSIZ-1, "%ld", l);
#endif

    if (ccount == BUFSIZ-1)
        return false;

    buf[BUFSIZ-1] = 0;

    s = buf;

    return true;
}
