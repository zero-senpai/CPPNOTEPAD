#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <string>
using namespace std;
QString activefile;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    //fdialog.fileSelected(filename);
    //fdialog.exec();
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
    QMessageBox::about(this, "About", "By Jake Brunton 2023\n\nSimple C++ Notepad\nGoogle Material Icons used under Apache License 2.0:\nhttps://www.apache.org/licenses/LICENSE-2.0.html");
}

