// FuncTable.h
#ifndef have_FuncTable
#define have_FuncTable

#include <map>
#include <string>
#include <vector>

/*
    with 
    FuncTable<int (Meterbase::*)(FILE *)> sft;

    call func like so:
    void Meterbase::someFunc()
    {
        ...
        FILE *ofp = fopen ...
        std::string s = ...
        res = (this->*sft[s])(ofp); // hack as sft[s] needs "this" ptr
        ...
    }
*/

template <class _FuncSpec> class FuncTable 
    : public std::map<std::string, _FuncSpec>
{
public:
    FuncTable(){}

    std::vector<std::string> strings()
    {
        std::vector<std::string>  res;

        std::map<std::string, _FuncSpec>::iterator it;
        for (it = begin(); it != end(); it++)
        {
            res.push_back(it->first);
        }

        return res;
    }

    /* works but pointless
    std::vector<_FuncSpec> funcs()
    {
        std::vector<_FuncSpec>  res;

        std::map<std::string, _FuncSpec>::iterator it;
        for (it = begin(); it != end(); it++)
        {
            res.push_back(it->second);
        }

        return res;
    }
    */
};

#endif // have_FuncTable

