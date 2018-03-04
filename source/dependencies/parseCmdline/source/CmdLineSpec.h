// CmdLineSpec.h
#ifndef have_CmdLineSpec
#define have_CmdLineSpec

#include <map>
#include <string>
#include <cstdio>
#include "OptionSpec.h"
#include "split_string.h"

class CmdLineSpec
{
    std::map<std::string, OptionSpec> m_specs;

public:
    CmdLineSpec()
    {
    }

    CmdLineSpec(std::map<std::string, OptionSpec> specs)
    {
        m_specs.insert(specs.begin(), specs.end());
    }

    virtual ~CmdLineSpec()
    {
    }

    bool hasFlag(char *flag)
    {
        if(m_specs.find(flag) != m_specs.end())
        {
            return true;
        }

        return false;
    }

    std::map<std::string, OptionSpec> getSpecs()
    {       
        return m_specs;
    }
    OptionSpec getSpec(std::string name)
    {
        std::map<std::string, OptionSpec>::iterator it = m_specs.find(name);
        if(it != m_specs.end())
        {
            return it->second;
        }

        return OptionSpec();
    }

    void help(std::string blurb, FILE* fp)
    {
        fprintf(fp, "usage: %s\noptions are:\n", blurb.c_str());
        std::map<std::string, OptionSpec>::iterator it;
        for (it = m_specs.begin(); it != m_specs.end(); it++)
        {
            std::string opt = it->first;
            OptionSpec osCur = it->second;
            std::string hText = osCur.getHelpText();
            splitstring::subst(hText, "\n", "\n                  ");
            if (!hText.empty())
                fprintf(fp, "    -%-10s : %s\n", opt.c_str(), hText.c_str());
        }
    }

};



#endif // have_CmdLineSpec

