#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "phpfpmstatus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refreshPoolStatus();
    void refreshProcesses();
private slots:
    void on_refreshbutton_clicked();
    void refreshStatus();

private:
    Ui::MainWindow *ui;
    phpFpmStatus *m_phpFpmStatus;

};

#endif // MAINWINDOW_H
