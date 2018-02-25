// NPArray.h
#ifndef have_NPArray
#define have_NPArray

#include <vector>
#include <algorithm>
#include "NodePair.h"

class NPArray : public std::vector<NodePair>
{
    std::string m_err;
    bool m_valid;

public:
    NPArray(){}

    NPArray(char *nodeArray[], int numNodes)
    {
        int i;
        for (i = 0; i < numNodes; i++)
        {
            NodePair np(nodeArray[i]);
            if (np.isValid())
            {
                push_back(np);
            }
            else
            {
                std::string npErr(nodeArray[i]);
                m_err = "error: \"" + npErr;
                m_err += "\" is invalid";
                m_valid = false;
            }
        }
    }

    virtual ~NPArray(){}

    std::string getError()
    {
        return m_err;
    }

    bool isValid()
    {
        return m_valid;
    }

    // return set of initial nodes
    std::set<NodePair> findStarters()
    {
        std::set<NodePair> res;
        size_t i;
        for (i = 0; i < size(); i++)
        {
            SANode curStart = at(i).getStart();
            if (!isEnd(curStart.getName()))
            {
                res.insert(at(i));
            }
            else if (at(i).getEnd().empty())
            {
                res.insert(at(i)); // think this redundant
            }
        }

        return res;
    }

    // it std::string s the name of an end node in *this ?
    bool isEnd(std::string s)
    {
        size_t i;
        for (i = 0; i < size(); i++)
        {
            if (at(i).isEnd(s))
                return true;
        }

        return false;
    }
    
    // look in *this for node pairs with start = sought.name
    NPArray findBegin(SANode sought)
    {
        NPArray res;
        size_t i;
        for (i = 0; i < size(); i++)
        {
            if (at(i).getStart() == sought.getName())
            {
                res.push_back(at(i));
            }
        }

        return res;
    }


    // look in *this for node pairs with end = sought.name
    void findEnd(SANode sought, NPArray &np)
    {
        NPArray res;
        size_t i;
        for (i = 0; i < size(); i++)
        {
            if (at(i).getEnd() == sought.getName())
            {
                res.push_back(at(i));
            }
        }

        np = res;
    }

    std::string toString()
    {
        std::string res;
        std::vector<NodePair>::iterator it;
        for(it = begin(); it != end(); it++)
        {
            res += "(" + it->toString() + ")\n"; 
        }

        return res;
    }

    std::string dotString()
    {
        std::string res;

        std::vector<NodePair>::iterator it;
        for(it = begin(); it != end(); it++)
        {
            res += it->dotString() + ";\n"; 
        }
        
        return res;
    }
};


#endif // have_NPArray


