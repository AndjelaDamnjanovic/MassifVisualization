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

bool Parser::isValidMassifFile() const
{
    return m_isMassifFile;
}

void Parser::parseFile()
{
    m_snapshots.clear();
    std::ifstream inputFile(m_filename);
    if(inputFile.is_open())
        std::cout<<"Open"<<std::endl;
    else
        std::cout<<"NOT open"<<std::endl;
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

void Parser::parsePreamble(std::ifstream &file)
{
    if(file.is_open())
        std::cout<<"Open"<<std::endl;
    else
        std::cout<<"NOT open"<<std::endl;

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

    // za slucaj da nije bilo dodatnih argumenata
    std::string s("desc: (none)");
    if(line.compare(s) == 0){
        std::cout<<"Ovde sam!"<<std::endl;
        m_massifArgs.append("(none)");
        return;
    }

    //deli se linija po -- karakterima koji odvajaju komande
    QVector<uint> positions;
    std::string delimiter = "--";
    uint position = line.find(delimiter, 0);
    //positions.append(position);

    while(position < line.size()){
        //std::cout<<position<<std::endl;
        positions.append(position);
        position = line.find(delimiter, position+1);
    }

    if(positions.size() == 0){
        m_isMassifFile = false;
        return;
    }

    int i;
    //std::cout<<positions.size();
    for (i = 0; i < positions.size() - 1; i++){
        m_massifArgs.append(line.substr(positions[i], positions[i+1]-positions[i]));
        //std::cout<<line.substr(positions[i], positions[i+1]-positions[i])<<std::endl;
    }
    m_massifArgs.append(line.substr(positions[i], line.size() - positions[i]));
    //std::cout<<line.substr(positions[i], line.size() - positions[i])<<std::endl;
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
    //std::cout<<m_command<<std::endl;
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
    //std::cout<<unit<<std::endl;
    if(unit.compare("B") == 0 || unit.compare("i") == 0 or unit.compare("ms") == 0){
        m_timeUnit = unit;
    }else{
        m_isMassifFile = false;
        return;
    }
    //std::cout<<m_timeUnit<<std::endl;
}

void Parser::parseSnapshots(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);

    if(line.rfind("#-----------", 0) != 0){
        m_isMassifFile = false;
        return;
    }

    for(int i =0; i<10;i++){
        std::getline(file, line);
    //while (std::getline(file, line)){
        Snapshot* snap = new Snapshot();
        uint num;
        parseSnapshotNumber(line, &num);

        if(num != INT_MAX){
            snap->setSnapshotIndex(&num);
            std::cout<<snap->getSnapshotIndex()<<std::endl;
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
            std::cout<<snap->getTime()<<std::endl;
        }else
            break;

        std::getline(file, line);
        quint64 usefulB;

        parseSnapshotUsefulB(line, &usefulB);
        if(usefulB != INT_MAX){
            snap->setUsefulHeapB(&usefulB);
            std::cout<<snap->getUsefulHeapB()<<std::endl;
        }else
            break;

        std::getline(file, line);
        quint64 extraB;

        parseSnapshotExtraB(line, &extraB);
        if(extraB != INT_MAX){
            snap->setExtraHeapB(&extraB);
            std::cout<<snap->getExtraHeapB()<<std::endl;
        }else
            break;

        quint64 totalB = extraB + usefulB;
        snap->setTotalHeapB(&totalB);
        std::cout<<snap->getTotalHeapB()<<std::endl;

        std::getline(file, line);
        quint64 stacks;

        parseSnapshotStacks(line, &stacks);
        if(stacks != INT_MAX){
            snap->setStacks(&stacks);
            std::cout<<snap->getStacks()<<std::endl;
        }else
            break;

        std::getline(file, line);
        std::string snapshotType="";

        parseSnapshotType(line, &snapshotType);
        if(snapshotType == ""){
            //snap->setSnapshotType(&snapshotType);
            break;
        }

        if(snapshotType.compare("empty") == 0){
            std::cout<<"Empty"<<std::endl;
            snap->setSnapshotType((SnapshotType::EMPTY));
            std::getline(file, line);
            if ((line.rfind("#-----------", 0) != 0) && !file.eof()) {
                m_isMassifFile = false;
                return;
            }
        }else if(snapshotType.compare("detailed") == 0){
            std::cout<<"Detailed"<<std::endl;
            snap->setSnapshotType((SnapshotType::DETAILED));
            parseDetailedShapshot(file);
        }else if(snapshotType.compare("peak") == 0){
            std::cout<<"Peak"<<std::endl;
            snap->setSnapshotType((SnapshotType::PEAK));

            HeapTreeNode* htNode = parseDetailedShapshot(file);
        }else{
            break;
        }
        //break;
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
    //std::cout<<index<<std::endl;
    QString res = QString::fromStdString(index);
    uint num = res.toUInt();
    *number = num;
    //std::cout<<num<<std::endl;
    //std::cout<<snap->getSnapshotIndex()<<std::endl;
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
    //std::cout<<num<<std::endl;
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
    //std::cout<<num<<std::endl;
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
    //std::cout<<num<<std::endl;
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
    //std::cout<<num<<std::endl;
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
    //std::cout<<res<<std::endl;
}

HeapTreeNode* Parser::parseDetailedShapshot(std::ifstream &file)
{
    std::cout<<"Sve ok!"<<std::endl;
    HeapTreeNode* ht = new HeapTreeNode;
    std::string line;

    std::getline(file, line);
    std::cout<<line<<std::endl;
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
