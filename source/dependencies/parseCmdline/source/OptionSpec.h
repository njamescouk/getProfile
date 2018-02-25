// OptionSpec.h
#ifndef have_OptionSpec
#define have_OptionSpec

#include <string>

class OptionSpec
{
    bool m_hasArgument;
    bool m_isEmpty;
    std::string m_helpText;
    // bool m_isMandatory
    // std::string m_default

public:
    OptionSpec() 
    {
        m_hasArgument = false;
        m_isEmpty = true;
    }

    OptionSpec(bool hasArgument)
        : m_hasArgument(hasArgument)
    {
        m_isEmpty = false;
    }

    OptionSpec(bool hasArgument, std::string help)
        : m_hasArgument(hasArgument),
          m_helpText(help)
    {
        m_isEmpty = false;
    }

    virtual ~OptionSpec() 
    {
    }

    bool hasArgument()
    {
        return m_hasArgument;
    }

    bool empty()
    {
        return m_isEmpty;
    }

    std::string getHelpText()
    {
        return m_helpText;
    }
};



#endif // have_OptionSpec

