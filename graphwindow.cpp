#include "graphwindow.h"
#include "ui_graphwindow.h"

#include "parser.h"
#include <QMainWindow>
#include <QFile>
#include <QScatterSeries>
#include <QGraphicsScene>

#include <QGraphicsView>
#include <fstream>
#include <string>
#include <QString>

#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <QShortcut>
#include <cmath>

#include <sstream>
#include <QSplineSeries>
#include <unistd.h>
#include <cstdlib>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>

QT_CHARTS_USE_NAMESPACE

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::warning(QString s) {
    QMessageBox::warning(this, "Error", "<FONT COLOR='#171717'>"+s+"</FONT>");
}

void GraphWindow::SaveAsPic(const QString& m_ext){
    QString dir = QDir::homePath();
    QString name = "Untilted." + m_ext;
    QString fileType = m_ext.toUpper() + "(*." + m_ext.toUpper() + ")";
    QString fileName= QFileDialog::getSaveFileName(this, "Save image", dir + "/" + name, fileType, nullptr, QFileDialog::DontUseNativeDialog);
    if (!fileName.isNull()) {
        QPixmap pixMap = this->ui->graphicsView->grab();
        pixMap.save(fileName);
    }

}

void GraphWindow::on_actionSaveAsPng_triggered() {
    GraphWindow::SaveAsPic("png");
}


void GraphWindow::on_actionSaveAsJpg_triggered() {
    GraphWindow::SaveAsPic("jpeg");
}

void GraphWindow::on_openOne_triggered(){

    m_parsers.clear();
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/pc/Desktop", "massif.out.*", nullptr, QFileDialog::DontUseNativeDialog);

    QFile open(file);
    if(open.size() == 0){
        warning("This file is empty!");
        return;
    }

    std::string filename = file.toStdString();
    std::ifstream openFile;
    m_path=filename;
    openFile.open(filename);

    if (openFile.fail()){
        warning("Could not open a file!");
        return;
    }else{
        drawGraph(filename);
    }
}

void GraphWindow::on_openMultiple_triggered()
{
    m_path = "";
    m_parsers.clear();
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/home/pc/Desktop", "massif.out.*");

    if(files.size() == 0){
        return;
    }

    for(auto file : files){
        std::cout<<file.toStdString()<<std::endl;
        QFile open(file);
        if(open.size() == 0){
            warning("File " + file + " is empty!");
            return;
        }
    }
    m_files = new QStringList(files);
    QVector<Parser *> parsers;

    for(auto file : *m_files){
        Parser *parser = new Parser(file.toStdString());
        parser->parseFile();

        if(parser->isValidMassifFile()){
            parsers.append(parser);
        }else{
            warning("File " + file + " is not a valid massif file!");
            return;
        }
    }

    m_parsers = parsers;
    drawMultipleGraphChart();
}

void GraphWindow::on_actionSaveAsPng2_triggered() {
    GraphWindow::SaveAsPic("png");
}


void GraphWindow::on_actionSaveAsJpg2_triggered() {
    GraphWindow::SaveAsPic("jpg");
}

void GraphWindow::on_actionClose2_triggered()
{
    on_actionClose_triggered();
}


void GraphWindow::on_pbSave_clicked() {

    if (ui->comboGraph->currentText() == ui->comboBackground->currentText()) {
        warning("Graph and background cannot be same color!");
    }else{
        QString res ="background-color: " +  ui->comboBackground->currentText();
        ui->graphicsView->setStyleSheet(res);
    }

    if(ui->rbGeneric->isChecked()){
        m_normalGraphChecked = true;
        m_scatterPlotChecked = false;
    }else{
        m_normalGraphChecked = false;
        m_scatterPlotChecked = true;
    }

    if(ui->cbXaxis->isChecked()){
        m_timeUnit = true;
    }else{
        m_timeUnit = false;
    }

    if(m_path != "")
        updateGraph();
    if(m_parsers.size() != 0)
        updateMultipleGraphs();
}

void GraphWindow::drawGraph(std::string filename)
{
    Parser* parser = new Parser(filename);
    parser->parseFile();

    if(!parser->isValidMassifFile()){
        warning("Error! This is not a massif file! Please enter a valid massif file!");
    }

    if(m_normalGraphChecked)
        drawNormalGraph(parser);
    else
        drawScatterPlot(parser);
}

void GraphWindow::updateGraph()
{
    drawGraph(m_path);
}

void GraphWindow::drawNormalGraph(const Parser *parser)
{

    QLineSeries *points = new QLineSeries;
    QVector<quint64> bytes = parser->getTotalBytes();
    QVector<quint64> times = parser->getTimesI();
    QVector<int> snapshots;
    QPointF dot;

    for(int i = 0; i < bytes.size(); i++){
        snapshots.append(i);
        if(m_timeUnit)
            dot = QPointF(times[i], bytes[i]);
        else
            dot = QPointF(snapshots[i], bytes[i]);
        points->append(dot);
    }


    QPen pen(QColor(ui->comboGraph->currentText()));
    pen.setWidth(ui->spinWidth->text().toInt());
    points->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(points);
    QString title = "valgrind --tool=massif ";
    QVector<std::string> args = parser->getMassifArgs();
    std::string command = parser->getCommand();
    for(auto arg : args){
        title = title + QString::fromStdString(arg) + " ";
    }
    title += QString::fromStdString(command);

    chart->setTitle(title);
    chart->createDefaultAxes();
    chart->setBackgroundBrush(QColor(ui->comboBackground->currentText()));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGraphicsView *graphicsView = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);

    chartView->setGeometry(graphicsView->rect());
    scene->addWidget(chartView);

    QHBoxLayout *screenLayout = new QHBoxLayout;
    QVBoxLayout *leftScreenLayout = new QVBoxLayout;
    QVBoxLayout *rightScreenLayout = new QVBoxLayout;
    rightScreenLayout->addWidget(graphicsView);

    QTabWidget *tab = ui->tabLeft;

    leftScreenLayout->addWidget(tab);
    screenLayout->addLayout(leftScreenLayout);
    screenLayout->addLayout(rightScreenLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(screenLayout);
    centralWidget->setStyleSheet("background-color: #222222;");


    setCentralWidget(centralWidget);
    resize(750, 600);
    show();
}

void GraphWindow::drawScatterPlot(const Parser *parser)
{
    QScatterSeries *points = new QScatterSeries();

    points->setName("Heap memory usage of the program during time");
    points->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    points->setMarkerSize(5.0);

    QVector<quint64> bytes = parser->getTotalBytes();
    QVector<quint64> times = parser->getTimesI();
    QVector<int> snapshots;
    QPointF dot;

    for(int i = 0; i < bytes.size(); i++){
        snapshots.append(i);
        if(m_timeUnit)
            dot = QPointF(times[i], bytes[i]);
        else
            dot = QPointF(snapshots[i], bytes[i]);
        points->append(dot);
    }

    QPen pen(QColor(ui->comboGraph->currentText()));
    pen.setWidth(ui->spinWidth->text().toInt());
    points->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(points);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    QString title = "valgrind --tool=massif ";
    QVector<std::string> args = parser->getMassifArgs();
    std::string command = parser->getCommand();
    for(auto arg : args){
        title = title + QString::fromStdString(arg) + " ";
    }
    title += QString::fromStdString(command);

    chart->setTitle(title);
    chart->createDefaultAxes();
    chart->setBackgroundBrush(QColor(ui->comboBackground->currentText()));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGraphicsView *graphicsView = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);

    chartView->setGeometry(graphicsView->rect());
    scene->addWidget(chartView);

    QHBoxLayout *screenLayout = new QHBoxLayout;
    QVBoxLayout *leftScreenLayout = new QVBoxLayout;
    QVBoxLayout *rightScreenLayout = new QVBoxLayout;
    rightScreenLayout->addWidget(graphicsView);

    QTabWidget *tab = ui->tabLeft;

    leftScreenLayout->addWidget(tab);
    screenLayout->addLayout(leftScreenLayout);
    screenLayout->addLayout(rightScreenLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(screenLayout);
    centralWidget->setStyleSheet("background-color: #222222;");


    setCentralWidget(centralWidget);
    resize(750, 600);
    show();
}

void GraphWindow::drawMultipleGraphChart()
{
    if(m_normalGraphChecked)
        drawMultipleNormalGraph();
    else
        drawMultipleScatterPlot();
}

void GraphWindow::drawMultipleNormalGraph()
{
    QChart *chart = new QChart();
    chart->setTitle("Comparison of multiple massif files:");

    QVector<QLineSeries*> seriesArray;

    QVector<QColor> colors;
    int itemIndex = ui->comboGraph->findText(ui->comboGraph->currentText());
    int itemNum = ui->comboGraph->count();

    for(int i = itemIndex; i < itemNum; i++){
        if(ui->comboGraph->itemText(i) != ui->comboBackground->currentText())
            colors.append(QColor(ui->comboGraph->itemText(i)));
    }

    for(int i = 0; i < itemIndex; i++){
        if(ui->comboGraph->itemText(i) != ui->comboBackground->currentText())
            colors.append(QColor(ui->comboGraph->itemText(i)));
    }

    int i = 0;

    for(auto parser : m_parsers){
        QPen pen(colors[i++]);
        pen.setWidth(ui->spinWidth->text().toInt());

        QLineSeries *series = new QLineSeries();
        series->setName(QString::fromStdString(parser->getCommand()));
        QVector<quint64> bytes = parser->getTotalBytes();
        QVector<quint64> times = parser->getTimesI();
        QVector<int> snapshots;
        QPointF dot;

        for(int i = 0; i < bytes.size(); i++){
            snapshots.append(i);
            if(m_timeUnit)
                dot = QPointF(times[i], bytes[i]);
            else
                dot = QPointF(snapshots[i], bytes[i]);
            series->setPen(pen);
            series->append(dot);
        }
        chart->addSeries(series);
    }

    chart->createDefaultAxes();
    chart->setBackgroundBrush(QColor(ui->comboBackground->currentText()));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGraphicsView *graphicsView = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);
    graphicsView->setScene(scene);
    chartView->setGeometry(graphicsView->rect());
}

void GraphWindow::drawMultipleScatterPlot()
{
    std::cout<<"Ovde i treba da budem"<<std::endl;
    QChart *chart = new QChart();
    chart->setTitle("Comparison of multiple massif files:");

    QVector<QScatterSeries*> seriesArray;

    QVector<QColor> colors;
    int itemIndex = ui->comboGraph->findText(ui->comboGraph->currentText());
    int itemNum = ui->comboGraph->count();

    for(int i = itemIndex; i < itemNum; i++){
        if(ui->comboGraph->itemText(i) != ui->comboBackground->currentText())
            colors.append(QColor(ui->comboGraph->itemText(i)));
    }

    for(int i = 0; i < itemIndex; i++){
        if(ui->comboGraph->itemText(i) != ui->comboBackground->currentText())
            colors.append(QColor(ui->comboGraph->itemText(i)));
    }

    int i = 0;

    for(auto parser : m_parsers){
        QPen pen(colors[i++]);
        pen.setWidth(ui->spinWidth->text().toInt());

        QScatterSeries *series = new QScatterSeries();
        series->setName(QString::fromStdString(parser->getCommand()));
        series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series->setMarkerSize(5.0);
        QVector<quint64> bytes = parser->getTotalBytes();
        QVector<quint64> times = parser->getTimesI();
        QVector<int> snapshots;
        QPointF dot;

        for(int i = 0; i < bytes.size(); i++){
            snapshots.append(i);
            //std::cout<<bytes[i]<<" "<<snapshots[i]<<" "<<times[i]<<std::endl;
            if(m_timeUnit)
                dot = QPointF(times[i], bytes[i]);
            else
                dot = QPointF(snapshots[i], bytes[i]);
            series->setPen(pen);
            series->append(dot);
        }
        chart->addSeries(series);
    }

    chart->createDefaultAxes();
    chart->setBackgroundBrush(QColor(ui->comboBackground->currentText()));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGraphicsView *graphicsView = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);
    graphicsView->setScene(scene);
    chartView->setGeometry(graphicsView->rect());
}

void GraphWindow::updateMultipleGraphs()
{
    drawMultipleGraphChart();
}

void GraphWindow::on_actionClose_triggered() {
    switch(QMessageBox::question(this, "Warning",
                                  "<FONT COLOR='#171717'>Do you want to save changes?</FONT>",

                                  QMessageBox::Save |
                                      QMessageBox::No |
                                      QMessageBox::Cancel |

                                      QMessageBox::Cancel) )
    {
    case QMessageBox::Save:
        emit on_actionSave_triggered();
        close();
        break;
    case QMessageBox::No:
        close();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
    return;
}

void GraphWindow::on_actionSave_triggered(){
    if (m_path==""){
        if (this->isLeftToRight()){
            QMessageBox::information(this, tr("Error"), "The scene is empty");
        }else{
            QString file = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", "MASSIF files (*.massif)", nullptr, QFileDialog::DontUseNativeDialog);
            if (!file.isEmpty()){
                std::string filename = file.toStdString();
                if (filename.substr(filename.size()- 7).compare(".massif")!=0){
                    filename+=".massif";
                }
                m_path = filename;
                std::ofstream saveFile;
                saveFile.open(filename);
            }
        }
    }else{
        std::ofstream saveFile;
        saveFile.open(m_path);
    }
}

void GraphWindow::on_actionSave2_triggered()
{
    emit on_actionSave_triggered();
}
