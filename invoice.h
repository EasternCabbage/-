#ifndef INVOICE_H
#define INVOICE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class invoice; }
QT_END_NAMESPACE

class invoice : public QMainWindow
{
    Q_OBJECT

public:
    invoice(QWidget *parent = nullptr);
    int num_of_combination(unsigned int total_num,unsigned int sub_num);
    ~invoice();

private slots:
    void on_pushButton_released();

private:

    Ui::invoice *ui;
};
#endif // INVOICE_H
