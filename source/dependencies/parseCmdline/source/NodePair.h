// NodePair.h
#ifndef have_NodePair
#define have_NodePair

#include <string>
#include <algorithm>

/* store (name, accepting) pairs */
class NodePair
{
    std::string m_start;
    std::string m_end;
    bool m_valid;

public:
    NodePair()
        : m_valid(false) {}


    NodePair(std::string start, std::string end)
        : m_valid(true)
    {
#if defined USING_GPP
        if (start.find_first_of(" \t\r\n\f") != std::string::npos
            || end.find_first_of(" \t\r\n\f") != std::string::npos)
#else
        if (std::any_of(start.begin(), start.end(), isspace)
            || std::any_of(end.begin(), end.end(), isspace))
#endif
        {
            m_valid = false;
            return;
        }

        m_start = start;
        m_end = end;
    }

    NodePair(std::string s)
        : m_valid(false)
    {
        m_valid = true;

#if defined USING_GPP
        if (s.find_first_of(" \t\r\n\f") != std::string::npos)
#else
        if (std::any_of(s.begin(), s.end(), isspace))
#endif
        {
            m_valid = false;
            return;
        }

        size_t arrowPos = s.find("->");
        bool singletonNode = (arrowPos == std::string::npos); // no arrow => s is final accepting pos

        std::string n1(s.substr(0, arrowPos));
        m_start = n1;

        if (!singletonNode)
        {
            std::string n2(s.substr(arrowPos + 2));
            m_end = n2;
        }
        else
        {
            m_valid = false; // no singletons
        }
    }

    virtual ~NodePair(){}

    bool operator<(const NodePair np) const
    {
        return m_start < np.m_start;
    }

    bool operator==(const NodePair np) const
    {
        return m_start == np.m_start && m_end == np.m_end;
    }

    bool isValid()
    {
        return m_valid;
    }

    std::string getStart()
    {
        return m_start;
    }

    std::string getEnd()
    {
        return m_end;
    }

    std::string& getStartRef()
    {
        return m_start;
    }

    std::string& getEndRef()
    {
        return m_end;
    }

    bool isEnd(std::string s)
    {
        if (m_end == s)
            return true;

        return false;
    }

    std::string toString()
    {
        std::string res = m_start + "," + m_end;

        return res;
    }

    std::string dotStartRecord()
    {
        return m_start + "[label=\"" + toString() + "\"];";
    }

    std::string dotEndRecord()
    {
        return m_end + "[label=\"" + toString() + "\"];";
    }

    std::string dotString()
    {
        std::string res = dotStartRecord()
                          + "\n"
                          + dotEndRecord() 
                          + "\n\"" 
                          + m_start 
                          + "\"->\"" 
                          + m_end + "\";";

        return res;
    }

};
#endif // have_NodePair
