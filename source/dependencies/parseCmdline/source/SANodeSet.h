// SANodeSet.h
#ifndef have_SANodeSet
#define have_SANodeSet

#include <set>
#include "SANode.h"
#include "NPArray.h"

class SANodeSet : public std::set<SANode>
{
public:
    SANodeSet()
    {
    }

    virtual ~SANodeSet()
    {
    }

    void insertEnds(NPArray newChildren)
    {
        NPArray::iterator itCh;
        for (itCh = newChildren.begin(); itCh != newChildren.end(); itCh++)
        {
            insert(itCh->getEnd());
        }
    }

    void insertNode(SANode n)
    {
        SANodeSet::iterator it = find(n);
        if (it != end())
        {
            bool accept = n.isAccepting() || it->isAccepting();
            n.setAccepting(accept);
            erase(it);
        }

        insert(n);
    }
};



#endif // have_SANodeSet

