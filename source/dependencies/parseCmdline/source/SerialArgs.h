// SerialArgs.h
#ifndef have_SerialArgs
#define have_SerialArgs

#pragma warning( disable : 4996 )

#include <vector>
#include <set>
#include <map>
#include "SANode.h"
#include "NPArray.h"
#include "split_string.h" 

extern char* progname;

class SerialArgs
{
    std::string m_err;
    bool m_valid;
    SANode m_root;
    std::string m_dotString;

    void makeDotString(NPArray userSpec)
    {
        m_dotString.clear();

        m_dotString += "digraph G\n";
        m_dotString += "{\n";

        NPArray::iterator it;
        for (it = userSpec.begin(); it != userSpec.end(); it++)
        {
            m_dotString += it->dotString() + "\n";
        }

        m_dotString += "}\n";
    }

    void populate(NPArray userSpec)
    {
        NPArray::iterator itNpa;
        for (itNpa = userSpec.begin(); itNpa != userSpec.end(); itNpa++)
        {
            m_root.insertNode(*itNpa);
        }
    }

public:
    SerialArgs()
        : m_root("SerialArgs")
    {}

    SerialArgs(char *nodeArray[], int numNodes)
        : m_root("SerialArgs")
    {
        NPArray userSpec(nodeArray, numNodes);

        populate(userSpec);

        makeDotString(userSpec);
    }

    SerialArgs(std::vector<std::string> traces)
        : m_root("SerialArgs")
    {
        NPArray userSpec;

        size_t traceNdx;
        for(traceNdx = 0; traceNdx < traces.size(); traceNdx++)
        {
            std::vector<std::string> nodes;
            splitstring::split(traces[traceNdx], nodes, "->");
            if (nodes.size() > 1)
            {
                size_t nodeNdx;
                for(nodeNdx = 0; nodeNdx < nodes.size() - 1; nodeNdx++)
                {
                    std::string start = nodes[nodeNdx];
                    std::string end = nodes[nodeNdx+1];
                    NodePair np(start, end);
                    userSpec.push_back(np);
                }
            }
            else
            {
                userSpec.push_back(NodePair(nodes[0], ""));
            }
        }

        // fprintf (stderr, "%s\n", userSpec.toString().c_str());
        populate(userSpec);
        // fprintf (stderr, "%s\n", toString().c_str());
    }

    virtual ~SerialArgs() {}

    std::string getError()
    {
        return m_err;
    }

    std::string getDotString()
    {
        return m_dotString;
    }

    SANode getRoot()
    {
        return m_root;
    }
    
    bool isValid()
    {
        return m_valid;
    }

    bool getArgs(std::vector<std::string> &al, SANode *curNode)
    {
        bool res = true;
        std::string prompt = curNode->getChildrenString() + " or q to quit - ";
        printf("%s", prompt.c_str());
        char buf[BUFSIZ];
        if (fgets(buf, BUFSIZ/2, stdin) == 0)
            return false;

        if (buf[0] == 'q')
            return false;

        std::string arg = buf;
        splitstring::trimws(arg);
        al.push_back(arg);
        curNode = curNode->getChild(arg);
        if (curNode->isAccepting())
        {
            if (curNode->getChildren().size() == 0)
            {
                // accepting node, nowhere to go
                return true;
            }
            else
            {
                // accepting node, more options
                printf("(a)ccept or (m)ore - ");
                switch (getc(stdin))
                {
                case 'a':
                    return true;
                case 'm':
                    break;
                default:
                    return false;
                }
            }
        }
        else
        {
            if (curNode->getChildren().size() == 0)
            {
                // not accepting node, but nowhere to go
                // accept
                return true;
            }
            // else keep on trucking
        }

        res = getArgs(al, curNode);

        return res;
    }

    void help(FILE *fp)
    {
        fprintf(fp, "%s arguments are like so:\n", progname);
        fprintf(fp, "%s\n", helpString().c_str());
    }

    std::string toString()
    {
        return m_root.toString();
    }

    std::string helpString()
    {
        std::string res;

        std::vector<SANode *> vn = m_root.getChildren();
        size_t i;
        for (i = 0; i < vn.size(); i++)
        {
            SANode san = *(vn[i]);
            res += san.helpString("") + "\n";
        }

        return res;
    }

    std::string makeNavString(std::string uri)
    {
        std::string indent;
        std::string res = "<nav>\n";

        // make content of root <dl>
        std::vector<std::string> trace;
        res += m_root.writeChildrenAsDLs(uri, indent, trace);

        res += indent + "</nav>\n";

        return res;
    }

    /*

  <dl> <!-- [server, show, write] -->
    <dt>server</dt>
    <dd><a href="meterbase?arg01=server&arg02=kill">kill</a></dd>

    <dt>show</dt>
    <dd>
      <dl> <!-- bill:[elec, gas] -->
        <dt>bill</dt>
        <dd><a href="meterbase?arg01=show&arg02=bill&arg03=elec">elec</a></dd>
        <dd><a href="meterbase?arg01=show&arg02=bill&arg03=gas">gas</a></dd>
      </dl>
    </dd>
    <dd><a href="meterbase?arg01=show&arg02=billstats">billstats</a></dd>
    <dd><a href="meterbase?arg01=show&arg02=projected">projected</a></dd>
    <dd>
      <dl> <!-- reading:[elec, gas] -->
        <dt>reading</dt>
        <dd><a href="meterbase?arg01=show&arg02=reading&arg03=elec">elec</a></dd>
        <dd><a href="meterbase?arg01=show&arg02=reading&arg03=gas">gas</a></dd>
      </dl>
    </dd>

    <dt>write</dt>
    <dd>
      <dl> <!-- bill:[elec, gas] -->
        <dt>bill</dt>
        <dd><a href="meterbase?arg01=write&arg02=bill&arg03=elec">elec</a></dd>
        <dd><a href="meterbase?arg01=write&arg02=bill&arg03=gas">gas</a></dd>
      </dl>
    </dd>
    <dd>
      <dl> <!-- reading:[elec, gas] -->
        <dt>reading</dt>
        <dd><a href="meterbase?arg01=write&arg02=reading&arg03=elec">elec</a></dd>
        <dd><a href="meterbase?arg01=write&arg02=reading&arg03=gas">gas</a></dd>
      </dl>
    </dd>
  </dl>

*/
    //     "        <dd><a href=\"meterbase?arg01=show&arg02=reading&arg03=elec\">elec</a></dd>\n"
};



#endif // have_SerialArgs

