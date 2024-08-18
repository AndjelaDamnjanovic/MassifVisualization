#include "graphwindow.h"
#include "ui_graphwindow.h"

#include "parser.h"
#include <QMainWindow>

#include <QJsonDocument>
#include <future>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <fstream>
#include <string>
#include <QString>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTextEdit>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <QShortcut>
#include <cmath>
#include <QTime>
#include <QIcon>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
    fillMap();
    indexColors();

}

GraphWindow::~GraphWindow()
{
    delete ui;
}


void GraphWindow::fillMap() {
    m_colors.insert("red", "#FF0000");
    m_colors.insert("blue", "#ADD8E6");
    m_colors.insert("green", "#87ab69");
    m_colors.insert("yellow", "#FFFF00");
    m_colors.insert("orange", "#FFE5B4");
    m_colors.insert("purple", "#A020F0");
    m_colors.insert("brown", "#964B00");
    m_colors.insert("cyan", "#00008B");
    m_colors.insert("pink", "#FFC0CB");
    m_colors.insert("off white", "#E8E4D6");

}

void GraphWindow::indexColors() {
    m_indices.insert("off white", 0);
    m_indices.insert("cyan", 8);
    m_indices.insert("blue", 1);
    m_indices.insert("pink", 9);
    m_indices.insert("brown", 7);
    m_indices.insert("purple", 6);
    m_indices.insert("orange", 5);
    m_indices.insert("yellow", 4);
    m_indices.insert("green", 3);
    m_indices.insert("red", 2);
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

    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "", nullptr, QFileDialog::DontUseNativeDialog);
    std::string filename = file.toStdString();
    std::ifstream openFile;
    m_path=filename;
    openFile.open(filename);

    if (!openFile.fail()){
        std::cout<<"Sve ok!"<<std::endl;
    }

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

    //Parser* parser = new Parser("/home/pc/Desktop/massif.out.6282");
    //Parser* parser = new Parser("/home/pc/Desktop/massif.out.30488");
    //Parser* parser = new Parser("/home/pc/Desktop/massif.out.6100");
    Parser* parser = new Parser("/home/pc/Desktop/massif.out.30871");
    parser->parseFile();
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
