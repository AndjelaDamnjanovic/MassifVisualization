#include "parser.h"

#include <QFile>
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <cstring>

Parser::Parser(std::string filename)
    :m_filename(filename)
{

}

Parser::~Parser()
{
    for(auto snap : m_snapshots){
        delete snap;
    }
}

QVector<Snapshot *> Parser::getSnapshots() const
{
    return m_snapshots;
}

QVector<quint64> Parser::getTimes() const
{
    return m_timesI;
}

QVector<quint64> Parser::getTotalB() const
{
    return m_totalB;
}

QVector<std::string> Parser::getMassifArgs() const
{
    return m_massifArgs;
}

std::string Parser::getCommand() const
{
    return m_command;
}

std::string Parser::getTimeUnit() const
{
    return m_timeUnit;
}

std::string Parser::getFilename() const
{
    return m_filename;
}

bool Parser::isValidMassifFile() const
{
    return m_isMassifFile;
}

void Parser::parseFile()
{
    m_snapshots.clear();
    std::ifstream inputFile(m_filename);

    parsePreamble(inputFile);

    if (!m_isMassifFile) {
        inputFile.close();
        return;
    }

    parseSnapshots(inputFile);
    if (!m_isMassifFile) {
        inputFile.close();
        return;
    }
}

QVector<quint64> Parser::getTotalBytes() const
{
    return m_totalB;
}

QVector<quint64> Parser::getTimesI() const
{
    return m_timesI;
}

void Parser::parsePreamble(std::ifstream &file)
{
    std::string line;

    std::getline(file, line);
    parseDescription(line);

    if (!m_isMassifFile) {
        file.close();
        return;
    }

    std::getline(file, line);
    parseCommand(line);

    if (!m_isMassifFile) {
        file.close();
        return;
    }

    std::getline(file, line);
    parseTimeUnit(line);

    if (!m_isMassifFile) {
        file.close();
        return;
    }
}

void Parser::parseDescription(const std::string &line)
{
    //primer izgleda linije koju treba parsirati:
    //desc: --peak-inaccuracy=2 --max-snapshots=30
    if(line.rfind("desc:", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    //deli se linija po -- karakterima koji odvajaju komande
    QVector<uint> positions;
    std::string delimiter = "--";
    uint position = line.find(delimiter, 0);

    while(position < line.size()){
        positions.append(position);
        position = line.find(delimiter, position+1);
    }

    if(positions.size() == 0){
        m_isMassifFile = false;
        return;
    }

    int i;

    for (i = 0; i < positions.size() - 1; i++){
        m_massifArgs.append(line.substr(positions[i], positions[i+1]-positions[i]));
    }
    m_massifArgs.append(line.substr(positions[i], line.size() - positions[i]));
}

void Parser::parseCommand(const std::string &line)
{
    if(line.rfind("cmd:", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = " ";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        return;
    }

    m_command = trim(line.substr(position, line.size() - position));
}

void Parser::parseTimeUnit(const std::string &line)
{
    if(line.rfind("time_unit:", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = " ";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        return;
    }

    std::string unit = trim(line.substr(position, line.size() - position));
    if(unit.compare("B") == 0 || unit.compare("i") == 0 or unit.compare("ms") == 0){
        m_timeUnit = unit;
    }else{
        m_isMassifFile = false;
        return;
    }
}

void Parser::parseSnapshots(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);

    if(line.rfind("#-----------", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    while (std::getline(file, line)){
        Snapshot* snap = new Snapshot();
        uint num;
        parseSnapshotNumber(line, &num);

        if(num != INT_MAX){
            snap->setSnapshotIndex(&num);
        }else
            break;

        std::getline(file, line);

        if(line.rfind("#-----------", 0) != 0){
            m_isMassifFile = false;
            break;
        }

        std::getline(file, line);
        quint64 time;

        parseSnapshotTime(line, &time);
        if(time != INT_MAX){
            snap->setTime(&time);
        }else
            break;

        std::getline(file, line);
        quint64 usefulB;

        parseSnapshotUsefulB(line, &usefulB);
        if(usefulB != INT_MAX){
            snap->setUsefulHeapB(&usefulB);
        }else
            break;

        std::getline(file, line);
        quint64 extraB;

        parseSnapshotExtraB(line, &extraB);
        if(extraB != INT_MAX){
            snap->setExtraHeapB(&extraB);
        }else
            break;

        quint64 totalB = extraB + usefulB;
        snap->setTotalHeapB(&totalB);

        std::getline(file, line);
        quint64 stacks;

        parseSnapshotStacks(line, &stacks);
        if(stacks != INT_MAX){
            snap->setStacks(&stacks);
        }else
            break;
        m_totalB.append(totalB + stacks);
        m_timesI.append(time);
        std::getline(file, line);
        std::string snapshotType="";

        parseSnapshotType(line, &snapshotType);
        if(snapshotType == ""){
            break;
        }

        if(snapshotType.compare("empty") == 0){
            snap->setSnapshotType((SnapshotType::EMPTY));
            std::getline(file, line);
            if ((line.rfind("#-----------", 0) != 0) && !file.eof()) {
                m_isMassifFile = false;
                return;
            }
        }else if(snapshotType.compare("detailed") == 0){
            snap->setSnapshotType((SnapshotType::DETAILED));
            HeapTreeNode* htNode = parseDetailedShapshot(file);
            snap->setCallTree(htNode);
        }else if(snapshotType.compare("peak") == 0){
            snap->setSnapshotType((SnapshotType::PEAK));

            HeapTreeNode* htNode = parseDetailedShapshot(file);
            snap->setCallTree(htNode);
        }else{
            break;
        }
        m_snapshots.append(snap);
    }
}

void Parser::parseSnapshotNumber(const std::string &line, uint *number){
    if(line.rfind("snapshot=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        *number = INT_MAX;
        return;
    }

    std::string index = trim(line.substr(position + 1, line.size() - position));
    QString res = QString::fromStdString(index);
    uint num = res.toUInt();
    *number = num;
}

void Parser::parseSnapshotTime(const std::string &line, quint64 *time)
{
    if(line.rfind("time=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        *time = INT_MAX;
        return;
    }

    std::string timeVal = trim(line.substr(position + 1, line.size() - position));
    QString res = QString::fromStdString(timeVal);
    quint64 num = res.toULongLong();
    *time = num;
}

void Parser::parseSnapshotUsefulB(const std::string &line, quint64 *useful)
{
    if(line.rfind("mem_heap_B=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        *useful = INT_MAX;
        return;
    }

    std::string memB = trim(line.substr(position + 1, line.size() - position));
    QString res = QString::fromStdString(memB);
    quint64 num = res.toULongLong();
    *useful = num;
}

void Parser::parseSnapshotExtraB(const std::string &line, quint64 *extra)
{
    if(line.rfind("mem_heap_extra_B=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        *extra = INT_MAX;
        return;
    }

    std::string memB = trim(line.substr(position + 1, line.size() - position));
    QString res = QString::fromStdString(memB);
    quint64 num = res.toULongLong();
    *extra = num;
}

void Parser::parseSnapshotStacks(const std::string &line, quint64 *stacks)
{
    if(line.rfind("mem_stacks_B=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        *stacks = INT_MAX;
        return;
    }

    std::string memB = trim(line.substr(position + 1, line.size() - position));
    QString res = QString::fromStdString(memB);
    quint64 num = res.toULongLong();
    *stacks = num;
}

void Parser::parseSnapshotType(const std::string &line, std::string *type)
{
    if(line.rfind("heap_tree=", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    std::string delimiter = "=";
    uint position = line.find(delimiter, 0);

    if(position == std::string::npos){
        m_isMassifFile = false;
        return;
    }

    std::string res = trim(line.substr(position + 1, line.size() - position));
    *type = res;
}

HeapTreeNode* Parser::parseDetailedShapshot(std::ifstream &file)
{
    HeapTreeNode* ht = new HeapTreeNode;
    std::string line;

    std::getline(file, line);
    uint posN = line.find("n");
    uint posDot = line.find(":");

    std::string childrenNum =line.substr(posN + 1, posDot - posN - 1);
    uint numChildren = QString::fromStdString(childrenNum).toUInt();
    ht->setNumOfChildern(&numChildren);

    std::regex regexp(" [1-9][0-9]+ ");
    std::smatch m;

    std::regex_search(line, m, regexp);
    std::string bytesAllocated = trim(m.str(0));
    quint64 resBytes = QString::fromStdString(bytesAllocated).toULongLong();
    ht->setAllocatedBytes(&resBytes);

    HeapTreeNode* parent = ht;
    std::getline(file, line);

    while(line.find("threshold") == std::string::npos && line.find("#-----------") == std::string::npos){

        HeapTreeNode *currNode = new HeapTreeNode;
        posN = line.find("n");
        posDot = line.find(":");

        childrenNum =line.substr(posN + 1, posDot - posN - 1);
        numChildren = QString::fromStdString(childrenNum).toUInt();
        currNode->setNumOfChildern(&numChildren);
        currNode->setParent(parent);

        std::regex_search(line, m, regexp);
        bytesAllocated = trim(m.str(0));
        resBytes = QString::fromStdString(bytesAllocated).toULongLong();
        currNode->setAllocatedBytes(&resBytes);

        std::regex regexp("0x[0-9A-F]{6}");
        std::regex_search(line, m, regexp);
        std::string hexadecimal = trim(m.str(0));
        currNode->setAddress(&hexadecimal);

        std::regex regexp2(": [a-zA-Z_]+ ");
        std::regex_search(line, m, regexp2);
        std::string fion = trim(m.str(0).erase(0, 1));
        currNode->setFunctionName(&fion);

        std::regex regexp3("\\([a-zA-Z0-9\.:]+\\)");
        std::regex_search(line, m, regexp3);
        std::string res = trim(m.str(0).erase(0, 1));
        std::string final = trim(res.erase(res.size() - 1, 1));


        std::string delimiter = ":";
        uint position = final.find(delimiter, 0);
        std::string filename = final.substr(0, position);
        currNode->setFileName(&filename);

        std::string lineNum = final.substr(position + 1, final.size() - position);
        uint numLine = QString::fromStdString(lineNum).toUInt();
        currNode->setLineNum(&numLine);

        std::getline(file, line);
        if(posN ==1)
            ht->addChild(currNode);
    }

    if(line.find("threshold") != std::string::npos && !file.eof()){
        std::getline(file, line);
    }

    return ht;
}

// posto STL nema svoju trim fju, koristila sam implementaciju koju sam nasla na github-u: https://gist.github.com/letiantian/a51003ae3f9896ee68d8c51f26c9312f
// (posto je fja preuzeta, bar sam ulozila trud da proverim radi li)
std::string Parser::trim(const std::string &src)
{
    std::string s(src);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\t\r")+1);
    return s;
}
