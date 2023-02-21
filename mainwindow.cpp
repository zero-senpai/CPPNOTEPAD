#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <string>
using namespace std;
QString activefile;
QFont font("Calibri", 12);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/images/notepad.ico"));
    ui->menubar->setNativeMenuBar(false);
    ui->menubar->setVisible(true);
    this->setCentralWidget(ui->textEdit);
    ToggleSave(false);
    ToggleNew(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ToggleSave(bool flag) {
    if (flag) {
        ui->actionSave->setEnabled(true);
    }
    else {
        ui->actionSave->setEnabled(false);
    }
}

void MainWindow::ToggleNew(bool flag) {
    if (flag) {
        ui->actionNew->setEnabled(true);
    }
    else {
        ui->actionNew->setEnabled(false);
    }
}

void MainWindow::on_actionNew_triggered()
{
    currentfile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fdialog(this);
    fdialog.setAttribute(Qt::WA_DeleteOnClose);
    fdialog.setFileMode(QFileDialog::ExistingFile);
    fdialog.setNameFilter(tr("Text File (*.txt)"));
    fdialog.selectNameFilter("Text file (*.txt)");
    fdialog.setViewMode(QFileDialog::Detail);
    fdialog.setDirectory(QDir::homePath());
    QString filename = fdialog.getOpenFileName(this);
    QFile file(filename);
    currentfile = filename;
    if (filename.isEmpty()) {
        fdialog.reject();
    }
    else if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    else {
        setWindowTitle(filename);
        activefile = filename;        //Call global QString activefile to set the open file name again
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();
    }


}

void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this);
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    currentfile = filename;
    activefile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();

}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot access printer.");
        return;
    }
    ui->textEdit->print(&printer);

}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "By Jake Brunton 2023\n\nSimple C++ Notepad v1.2.0\n\nGoogle Material Icons used under Apache License 2.0:\nhttps://www.apache.org/licenses/LICENSE-2.0.html\n\n Built on the Qt 6.4.x platform License: https://www.qt.io/terms-conditions/\n");
}

/*
*   @func       - Saves the current open file without the use of QDialogs based on the activefile QString value which is set when the Open function is used.
*   @params     - None
*/
void MainWindow::on_actionSave_triggered()
{
    if (activefile == " ") {
        QMessageBox::warning(this, "Notice:", "\n No file is currently opened and edited.");
        return;
    }
    else {
        fstream file(activefile.toStdString());
        QString text = ui->textEdit->toPlainText();
        file << text.toStdString();
        file.close();
    }
}


void MainWindow::on_textEdit_textChanged()
{
    if (activefile.isEmpty()) {
        ToggleSave(false);
    }
    else {
        ToggleSave(true);
    }
    ToggleNew(true);
}


void MainWindow::on_actionFont_triggered()
{
    bool t;
    QString fts;
 
    fts = ::font.toString();
    ::font = QFontDialog::getFont(&t, QFont(fts, 12), this);

    if (t) {
        ui->textEdit->setFont(::font);
        fts = ::font.toString();
    }
}


void MainWindow::on_actionBackground_Color_triggered()
{
    QColor chosencolor = QColorDialog::getColor(Qt::white, this);
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, chosencolor);
    ui->textEdit->setPalette(p);
}

void MainWindow::on_actionFont_Color_triggered()
{
    QColor chosencolor = QColorDialog::getColor(Qt::black, this);
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Text, chosencolor);
    ui->textEdit->setPalette(p);
}

