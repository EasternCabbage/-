#include "invoice.h"
#include "ui_invoice.h"
#include <iostream>

using namespace std;

invoice::invoice(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::invoice)
{
    ui->setupUi(this);
}

invoice::~invoice()
{
    delete ui;
}

void invoice::on_pushButton_released()
{
    QString invoices_hold = ui->textEdit->toPlainText();//获取拥有的发票数值信息
    float budget = ui->textEdit_2->toPlainText().toFloat();//获取预算
    QStringList list = invoices_hold.split("\n");//将已有的发票存到容器里
    vector<float>container;
    vector<bool>lowest_container;
    for (int i=0; i<list.size(); ++i)
    {
            container.push_back(list.at(i).toFloat());
            //qDebug() << list.at(i).toLocal8Bit().constData() << container[i];
    }

    float recent_sum = 0;
    float last_sum = 0;

    for(int i=1;i<=(int)container.size();i++)//从1到n开始组合
    {
        vector<bool>dictionary;
        int j=0;
        while(j<(int)container.size()-i){//构造字典序入栈0
            dictionary.emplace_back(0);
            ++j;
        }
        while(j<(int)container.size()){//构造字典序入栈1,构造完毕例如vector[0,0,0,0,0,0,0,1]
            dictionary.emplace_back(1);
            ++j;
        }

        bool over = 0;
        while(!over)//如果r=i时的字典序还有下一个
        {
            vector<bool>::iterator itprintf;
           /* for(itprintf = dictionary.begin();itprintf<dictionary.end();itprintf++)
            {
                cout << *itprintf;
            }
            cout << " "<< endl;*/
            for(j=0;j<(int)container.size();j++){//字典序|容器数值，处理这个组合
                if(dictionary[j] == 1)
                {
                    recent_sum += container[j];
                    //qDebug()<<container[j]<<" ";
                }

            }

            if(budget - recent_sum < budget - last_sum && budget -recent_sum >= 0)
            {
                last_sum = recent_sum;
                lowest_container = dictionary;
            }
            //qDebug()<<"sum of this combination:"<< recent_sum<<"\r";
            recent_sum = 0;

            /*跳到下一个字典序*/
            vector<bool>::iterator it;
            it = dictionary.end() - 1;
           if(*it == 1)//字典序末尾为1
            {
               while(*it == 1)//从末尾往首位检测，直到不为1
                {
                        if(it == dictionary.begin()){
                            over = 1;
                            break;
                        }
                        else{
                            --it;
                        }
                 }
                 *it = 1;
                 *(++it) = 0;
             }
            else//字典序末尾为0
            {
                unsigned long t=0;
                unsigned long m=0;
                while(*it == 0)
                {
                    ++t;//末尾有t个连续的0
                    --it;
                }

                while(*it == 1 && it >= dictionary.begin())//这里小心指针越界
                {
                    ++m;//末尾连续t个0之前有连续m个1
                    --it;
                }
                if(t + m == dictionary.size())//11110000这种情况，说明已经到r=i最后一个字典序了,下一个字典序应该是00011111，所以跳出循环
                {
                    over = 1;//其实写不写都行，这里也没有像if一样套二层循环，写是为了和if里保持对称。
                    break;
                }

                *(dictionary.end()-t-m)=0;//我们可以将倒数第 t + m 位置的 1 和倒数第 t + m + 1 位的 0 对换
                *(dictionary.end()-t-m-1)=1;//我们可以将倒数第 t + m 位置的 1 和倒数第 t + m + 1 位的 0 对换
                if(i != 1)//只有一个位=1的时候实际上不需要这一步。
                {
                    vector<bool>::iterator head_cursor,end_cursor;
                    head_cursor = dictionary.end() - (t+m-1);
                    end_cursor = dictionary.end() - 1;
                    while(*head_cursor == 1 && *end_cursor == 0 && head_cursor != end_cursor)//然后把倒数第 t + 1 位到倒数第 t + m - 1 位的 1 移动到最低位。
                    {
                        *head_cursor = 0;
                        *end_cursor = 1;
                        ++head_cursor;
                        --end_cursor;
                    }
                }

           }//到此为止两种类型的字典序递增完毕
        }

      }

    string select ;

    vector<bool>::iterator greasy;
   // cout <<"lowest dictionary value combination" << endl;
    for(greasy = lowest_container.begin() ; greasy < lowest_container.end();++greasy)//输出最靠近预算的排列组合的字典序
    {
        select += to_string(*greasy);
        select += "\n";
        //cout << *greasy;
    }
   // cout << " "<< endl;
    QString result = QString::fromStdString(select);
    ui->textBrowser->setText(result);
    ui->textBrowser_2->setText(QString::number (last_sum));
    // cout << "sum value closet to budget:" << last_sum << endl;

}

/*计算组合数数量,没用到。*/
int invoice:: num_of_combination(unsigned int total_num,unsigned int sub_num)
{
    unsigned int guard_molecule = 1;
    unsigned int guard_denominator = 1;
    unsigned long long molecule = 1;
    unsigned long long denominator = 1;
    while(guard_molecule <= total_num)//计算n!
    {
        molecule *= guard_molecule;
        ++guard_molecule;
    }
    qDebug() <<"分子是"<< molecule;

    while(guard_denominator <= sub_num)//计算r!
    {
        denominator *= guard_denominator;
        ++guard_denominator;
    }


    guard_denominator = 1;
    while(guard_denominator <= (total_num - sub_num))//计算r!(n-r)!
    {
        denominator *= guard_denominator;
        ++guard_denominator;
    }
     qDebug() <<"分母是"<< denominator;
    return molecule/denominator;
}
