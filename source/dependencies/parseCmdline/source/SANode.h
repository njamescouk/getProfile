// SANode.h
#ifndef have_SANode
#define have_SANode

#include <set>
#include <string>
#include "NodePair.h"

class SANode
{
    std::string m_name;
    std::vector<SANode *> m_pChildren;
    bool m_isAccepting;

public:
    SANode() 
        : m_isAccepting(false)
    {}

    SANode(std::string s)
        : m_isAccepting(false)
    {
        if (s.empty())
            return;

        size_t lastPos = s.length();

        if (s[s.length() - 1] == '*')
        {
            lastPos--;
            m_isAccepting = true;
        }

        m_name = s.substr(0, lastPos);
    }

    virtual ~SANode(){}

    std::string getName()
    {
        return m_name;
    }

    std::vector<SANode *> getChildren()
    {
        return m_pChildren;
    }

    SANode *getChild (std::string s)
    {
        size_t i;
        for (i = 0; i < m_pChildren.size(); i++)
        {
            if (m_pChildren[i]->getName() == s)
                return m_pChildren[i];
        }

        return 0;
    }

    void addChild(SANode *n)
    {
        m_pChildren.push_back(n);
    }

    SANode *makePair(NodePair np)
    {
        SANode *res = new SANode(np.getStart());
        if (!np.getEnd().empty())
        {
            SANode *pKid = new SANode(np.getEnd());
            res->addChild(pKid);
        }

        return res;
    }

    void insertNode(NodePair np)
    {
        std::vector<SANode *> possNodes = findNodes(np.getStart());
        if (possNodes.size() == 0)
        {
            SANode *sanPair = makePair(np);
            m_pChildren.push_back(sanPair);
        }
        else
        {
            size_t i;
            for (i = 0; i < possNodes.size(); i++)
            {
                SANode *pPoss = possNodes[i];
                if (pPoss->getChild(np.getEnd()) == 0)
                {
                    // ie. we haven't already got this NodePair
                    SANode *pLeaf = new SANode(np.getEnd());
                    pPoss->addChild(pLeaf);
                }
            }
        }
    }

    bool isAccepting() const
    {
        return m_isAccepting;
    }

    void setAccepting(bool b)
    {
        m_isAccepting = b;
    }

    bool operator<(const SANode n) const
    {
        return m_name < n.m_name;
    }

    bool operator==(const SANode n) const
    {
        return m_name == n.m_name;
    }

    std::vector<SANode *> findNodes(std::string s)
    {
        std::vector<SANode *> res;

        std::vector<SANode *>::iterator it;
        for (it = m_pChildren.begin(); it != m_pChildren.end(); it++)
        {
            SANode *pCur = *it;
            if (pCur != 0)
            {
                if (pCur->getName() == s)
                {
                    res.push_back(pCur);
                }

                std::vector<SANode *> leafNodes = pCur->findNodes(s);
                res.insert(res.end(), leafNodes.begin(), leafNodes.end());
            }
        }

        return res;
    }

    std::string toString()
    {
        std::string res;

        if (m_isAccepting)
            res += "*";

        if (m_pChildren.size() > 0)
        {
            res += "\n(";

            std::vector<SANode *>::iterator itCh;
            for (itCh = m_pChildren.begin(); itCh != m_pChildren.end(); itCh++)
            {
                SANode *cur = *itCh;
                if (itCh != m_pChildren.begin())
                {
                    res += ", ";
                }

                res += cur->getName();
                res += cur->toString();
            }

            res += ")\n";
        }

        return res;
    }

    std::string getChildrenString()
    {
        std::string res;
        std::vector<SANode *>::iterator itCh;
        for (itCh = m_pChildren.begin(); itCh != m_pChildren.end(); itCh++)
        {
            if (itCh != m_pChildren.begin())
            {
                res += ", ";
            }

            SANode *cur = *itCh;
            res += cur->getName();
        }

        return res;
    }

    std::string helpString(std::string indent)
    {
        std::string res;
        res += indent + m_name;
        indent += "    ";

        std::vector<SANode *>::iterator itCh;
        for (itCh = m_pChildren.begin(); itCh != m_pChildren.end(); itCh++)
        {
            res += "\n";
            SANode *nextSan = *itCh;
            if (nextSan != 0)
                res += nextSan->helpString(indent);
        }

        return res;
    }

    // DO NOT CHANGE
    std::string writeChildrenAsDLs(std::string uri, std::string indent, std::vector<std::string> trace)
    {
        //print(stderr, "writeChildrenAsDLs");
        std::string res;
        indent = indent + "  ";

        res += indent + "<dl> <!-- [" + getChildrenString() + "] -->\n";

        // DO NOT CHANGE
        size_t i;
        for (i = 0; i < m_pChildren.size(); i++)
        {
            if (i > 0)
                res += "\n";
            SANode *pCur = m_pChildren[i];
            res += pCur->makeDLContents(uri, indent, trace);
        }

        res += indent + "</dl>\n";

        return res;
    }

    std::string makeDL(std::string uri, std::string indent, std::vector<std::string> trace)
    {
        // print(stderr, "makeDL");
        std::string res;
        indent = indent + "  ";

        res += indent + "<dl> <!-- " + m_name + ":[" + getChildrenString() + "] -->\n";
        res += makeDLContents(uri, indent, trace);
        res += indent + "</dl>\n";

        return res;
    }

    // need this as an entry point
    std::string makeDLContents(std::string uri, std::string indent, std::vector<std::string> trace)
    {
        // print(stderr, "makeDLContents");
        std::string res;
        indent += "  ";

        trace.push_back(m_name);

        res = indent + "<dt>" + m_name + "</dt>\n";
        res += makeDDs(uri, indent, trace);

        return res;
    }

    // write children contained in <dd>s
    std::string makeDDs(std::string uri, std::string indent, std::vector<std::string> trace)
    {
        // print(stderr, "makeDDs");
        std::string res;

        size_t i;
        for (i = 0; i < m_pChildren.size(); i++)
        {
            SANode *pCur = m_pChildren[i];
            if (pCur->m_pChildren.size() == 0)
            {
                res += indent + "<dd>" + pCur->makeAnchor(uri, trace) + "</dd>\n";
            }
            else
            {
                res += indent + "<dd>\n";
                res += pCur->makeDL(uri, indent, trace);
                res += indent + "</dd>\n";
            }
        }

        return res;
    }

    // eg "        <dd><a href=\"meterbase?arg01=show&arg02=reading&arg03=elec\">elec</a></dd>\n"
    // BUG only good for trace.size in (0, 999)
    std::string makeAnchor(std::string uri, std::vector<std::string> trace)
    {
        std::string res = "<a href=\"" + uri + "?";
        trace.push_back(m_name);

        size_t i;
        for (i = 0; i < trace.size(); i++)
        {
            if (i > 0)
                res += "&";
            char buf[BUFSIZ];
            itoa((int)i+1, buf, 10);
            if (i < 10)
                res += "arg0";
            else
                res += "arg";
            res += buf;
            res += "=" + trace[i];
        }

        res += "\">";
        res += m_name + "</a>";

        return res;
    }

    void print(FILE *fp, std::string func)
    {
        fprintf(fp, "%s(%s:[", func.c_str(), m_name.c_str());
        size_t i;
        for (i = 0; i < m_pChildren.size(); i++)
        {
            if (i > 0)
                fprintf(fp, ", ");
            fprintf(fp, "%s", m_pChildren[i]->getName().c_str());
        }
        fprintf(fp, "])\n");
    }

};



#endif // have_SANode

