#ifndef HEAPTREENODE_H
#define HEAPTREENODE_H

#include <qglobal.h>
#include <iostream>
#include <QVector>

class HeapTreeNode
{
public:
    HeapTreeNode();
    ~HeapTreeNode();

    quint64 getAllocatedBytes() const;
    void setAllocatedBytes(const quint64 *);

    std::string getAddress() const;
    void setAddress(const std::string *);

    std::string getFunctionName() const;
    void setFunctionName(const std::string *);

    std::string getFileName() const;
    void setFileName(const std::string *);

    uint getLineNum() const;
    void setLineNum(const uint *);

private:
    quint64 m_allocatedBytes;
    std::string m_address;
    std::string m_functionName;
    std::string m_fileName;
    uint m_lineNum;

    // rad sa drvetom poziva
    QVector<HeapTreeNode> m_children;

};

#endif // HEAPTREENODE_H
