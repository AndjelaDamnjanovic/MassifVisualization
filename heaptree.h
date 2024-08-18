#ifndef HEAPTREE_H
#define HEAPTREE_H

#include <qglobal.h>
#include <iostream>

class HeapTree
{
public:
    HeapTree();
    ~HeapTree();

    qreal getPercentage() const;
    void setPercentage(const qreal *);

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
    qreal m_percentage;
    quint64 m_allocatedBytes;
    std::string m_address;
    std::string m_functionName;
    std::string m_fileName;
    uint m_lineNum;


};

#endif // HEAPTREE_H
