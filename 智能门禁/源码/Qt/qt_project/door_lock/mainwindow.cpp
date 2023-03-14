#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>
#include "QString"
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSound>

QSerialPort *serial=  new QSerialPort;
QPropertyAnimation *propertyAnimation = new QPropertyAnimation;
QPropertyAnimation *propertyAnimation2 = new QPropertyAnimation;


QSound *star = new QSound(":/1.wav");
QSound *inputnum = new QSound(":/2.wav");
QSound *door_o_c = new QSound(":/3.wav");
QSound *m_z_kai = new QSound(":/4.wav");
QSound *click = new QSound(":/5.wav");
QSound *bj = new QSound(":/bj.wav");



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setFixedSize(364,600);
    this->setWindowTitle("智能门禁系统");
    this->setWindowIcon(QIcon(":/sys_icon.png"));
    this->setWindowOpacity(1.0);//窗体透明度
    bj->play();
    bj->setLoops(-1);
    ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset");//无边框且透明
    ui->lineEdit->setReadOnly(true);//取消手动输入
    ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setStyleSheet("background:transparent;color: rgb(32,32,32);");
    ui->lineEdit_3->setReadOnly(true);
    ui->pushButton->setIconSize(QSize(35,35));
    ui->pushButton_2->setIconSize(QSize(35,35));
    ui->pushButton->setIcon(QIcon(":/t1.png"));
    ui->pushButton_2->setIcon(QIcon(":/h1.png"));

    ui->pushButton_3->setIconSize(QSize(70,70));
    ui->pushButton_4->setIconSize(QSize(70,70));
    ui->pushButton_5->setIconSize(QSize(70,70));
    ui->pushButton_3->setIcon(QIcon(":/star.png"));
    ui->pushButton_4->setIcon(QIcon(":/指纹.png"));
    ui->pushButton_5->setIcon(QIcon(":/密码.png"));
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    ui->pushButton_18->setIconSize(QSize(100,100));
    ui->pushButton_18->setIcon(QIcon(":/锁关.png"));
    ui->pushButton_19->setIconSize(QSize(70,70));
    ui->pushButton_19->setIcon(QIcon(":/reword.png"));
    ui->pushButton_20->setIconSize(QSize(70,70));
    ui->pushButton_20->setIcon(QIcon(":/day.png"));


    //设置串口名
    serial->setPortName("COM3");
    //设置波特率
    serial->setBaudRate(115200);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
     //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);

    QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::Uart_Read_Data);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdata_date()));
    ui->lcdNumber->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    QPalette lcdp1 = ui->lcdNumber->palette();
    lcdp1.setColor(QPalette::Foreground,QColor(qRgb(233,233,216)));
    ui->lcdNumber->setPalette(lcdp1);

    t_h.append("00");
    t_h.append("00");
    QTimer *timer2 = new QTimer(this);
    timer2->start(100);
    connect(timer2,SIGNAL(timeout()),this,SLOT(change_data()));

    side_widget_flag = true;
    ui->widget->move(this->rect().width() + ui->widget->width(),350);
    propertyAnimation = new QPropertyAnimation(ui->widget,"geometry");
    propertyAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    propertyAnimation->setDuration(1000);

    ui->textEdit->move(this->rect().width() + ui->textEdit->width(),400);
    propertyAnimation2 = new QPropertyAnimation(ui->textEdit,"geometry");
    propertyAnimation2->setEasingCurve(QEasingCurve::InOutQuint);
    propertyAnimation2->setDuration(1000);

    //ui界面纵轴关系  this-ui在最下层；textEdit（开锁记录）在中间层；widget（密码键盘）在最上层
    ui->widget->raise();
    ui->textEdit->stackUnder(ui->widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//大屏时钟
void MainWindow::timerUpdata_date()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("hh:mm:ss");   //yyyy-MM-dd
    ui->lcdNumber->display(str);
}

void MainWindow::change_data()
{
    ui->lineEdit->setText("今日温度:" + t_h[0]);
    ui->lineEdit_2->setText(t_h[1] + ":度湿日今");
    t_change();
    d_change();
}

//温度颜色值
void MainWindow::t_change()
{
    //qDebug() << a;
    if(a < 10)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,191,255);");
        ui->pushButton->setIcon(QIcon(":/t1.png"));
    }
    else if(a >= 10 && a < 20)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(30,144,255);");
        ui->pushButton->setIcon(QIcon(":/t1.png"));
    }
    else if(a >= 20 && a < 23)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(218,165,32);");
        ui->pushButton->setIcon(QIcon(":/t1.png"));
    }
    else if(a >= 23 && a < 25)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(255,165,0);");
        ui->pushButton->setIcon(QIcon(":/t2.png"));
    }
    else if(a >= 25 && a < 27)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(255,140,0);");
        ui->pushButton->setIcon(QIcon(":/t2.png"));
    }
    else if(a >= 27 && a < 29)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(255,70,0);");
        ui->pushButton->setIcon(QIcon(":/t2.png"));
    }
    else if(a >= 29 && a < 30)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(255,99,71);");
        ui->pushButton->setIcon(QIcon(":/t3.png"));
    }
    else if(a >= 30 && a < 35)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(255,0,0);");
        ui->pushButton->setIcon(QIcon(":/t3.png"));
    }
    else if(a >= 35)
    {
        ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(139,0,0);");
        ui->pushButton->setIcon(QIcon(":/t3.png"));
    }

}

//湿度颜色值
void MainWindow::d_change()
{
    if(b < 30)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,255,255);");
        ui->pushButton_2->setIcon(QIcon(":/h1.png"));
    }
    else if(b >= 30 && b < 40)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,206,209);");
        ui->pushButton_2->setIcon(QIcon(":/h1.png"));
    }
    else if(b >=40 && b < 50)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,191,255);");
        ui->pushButton_2->setIcon(QIcon(":/h1.png"));
    }
    else if(b >=50 && b < 60)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(30,144,255);");
        ui->pushButton_2->setIcon(QIcon(":/h2.png"));
    }
    else if(b >=60 && b < 70)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(100,149,237);");
        ui->pushButton_2->setIcon(QIcon(":/h2.png"));
    }
    else if(b >= 70 && b < 80)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(65,105,250);");
        ui->pushButton_2->setIcon(QIcon(":/h3.png"));
    }
    else if(b >= 80 && b < 90)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,0,255);");
        ui->pushButton_2->setIcon(QIcon(":/h3.png"));
    }
    else if(b >= 90)
    {
        ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset;color: rgb(0,0,139);");
        ui->pushButton_2->setIcon(QIcon(":/h3.png"));
    }
}

void MainWindow::Uart_Read_Data()
{
    QString str;
    QByteArray buf;
    QStringList list;
    buf = serial->readAll();//串口读到的数据
    if(!buf.isEmpty())
    {
        str = tr(buf);
        if(str == "openclock")
        {
            ui->pushButton_18->setIcon(QIcon(":/锁开.png"));
            success_open_door = true;
            m_z_kai->play();
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
        }
        if(str == "havepeople")
        {
            QMessageBox::warning(this,tr("警报"),tr("有可疑人员"));
        }
        if(str == "fingerno")
        {
            QMessageBox::warning(this,tr("错误"),tr("指纹不匹配"));
        }
        QRegExp rx("-?(([1-9]\\d*\\.\\d*)|(0\\.\\d*[1-9]\\d*)|([1-9]\\d*))");
        int p = 0;
        if(tr(buf).contains("T:",Qt::CaseSensitive)&& tr(buf).contains("H:",Qt::CaseSensitive)&& tr(buf).contains("%",Qt::CaseSensitive))
        {
            while ((p = rx.indexIn(str, p)) != -1)
            {
                list.append(rx.cap(1));
                p += rx.matchedLength();
            }
            a = list[0].toDouble();
            b = list[1].toDouble();
            t_h[0] = list[0];
            t_h[1] = list[1];
            //qDebug() << t_h;
        }

    }
    buf.clear();
    list.clear();
    str.clear();
}


//启动
int flag=0;
void MainWindow::on_pushButton_3_clicked()
{
    star->play();
    flag++;
    if(flag==1)
    {
        serial->open(QIODevice::ReadWrite);
        if(serial->isOpen())
        {
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_5->setEnabled(true);
            ui->lineEdit_3->setText("工作中...");

        }
        else
        {
           QMessageBox::warning(this,tr("Warnning！"),tr("连接有误，请重试！"));
           flag=0;
           ui->pushButton_4->setEnabled(false);
           ui->pushButton_5->setEnabled(false);
        }
    }
    if(flag==2 && serial->isOpen())
    {
        flag=0;
        ui->lineEdit_3->setText("已关闭");
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        serial->clear();
        serial->close();
        side_widget_flag = false;
        ui->widget->move(this->rect().width() + ui->widget->width(),350);
        ui->textEdit->move(this->rect().width() + ui->textEdit->width(),400);

    }
}



//指纹开锁
void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->move(this->rect().width() + ui->widget->width(),350);
    if(serial->isOpen())
    {
        door_o_c->play();
        serial->write("aa");
        ui->lineEdit_3->setText("请按压指纹");
    }
}

//密码开锁
void MainWindow::on_pushButton_5_clicked()
{
    door_o_c->play();
    if(side_widget_flag)
    {
        ui->lineEdit_3->setText("请输入密码");
        propertyAnimation->setStartValue(QRect(this->rect().width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->setEndValue(QRect(this->rect().width()-ui->widget->width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->start();
        side_widget_flag = false;
    }
    else
    {
        propertyAnimation->setStartValue(QRect(this->rect().width()-ui->widget->width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->setEndValue(QRect(this->rect().width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->start();
        side_widget_flag = true;
    }
}

//密码键盘
int password_flag = 0;
void MainWindow::on_pushButton_6_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "1";
        password_flag++;
    }
}
void MainWindow::on_pushButton_7_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "2";
        password_flag++;
    }
}
void MainWindow::on_pushButton_8_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "3";
        password_flag++;
    }
}
void MainWindow::on_pushButton_9_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "4";
        password_flag++;
        inputnum->play();
    }
}
void MainWindow::on_pushButton_10_clicked()
{
    if(password_flag < 4)
    {
        password += "5";
        password_flag++;
    }
}
void MainWindow::on_pushButton_11_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "6";
        password_flag++;
        inputnum->play();
    }
}
void MainWindow::on_pushButton_12_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "7";
        password_flag++;
    }
}
void MainWindow::on_pushButton_13_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "8";
        password_flag++;
    }
}
void MainWindow::on_pushButton_14_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "9";
        password_flag++;
    }
}
void MainWindow::on_pushButton_15_clicked()
{
    inputnum->play();
    if(password_flag < 4)
    {
        password += "0";
        password_flag++;
    }
}
//密码输入重置
void MainWindow::on_pushButton_17_clicked()
{
    inputnum->play();
    password.clear();
    password_flag = 0;
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QString reword_str;
    if(password.isEmpty())
    {
        QMessageBox::warning(this,tr("Tips"),tr("输入重置"));
        reword_str = str + " 输入重置";
        reword.append(reword_str);
    }
    reword_str.clear();
}

//发送密码
void MainWindow::on_pushButton_16_clicked()
{
    inputnum->play();
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QString reword_str;
    if(password.length()==4 && password == "1111")
    {
        serial->write("bb");
        password.clear();
        ui->pushButton_18->setIcon(QIcon(":/锁开.png"));
        password_flag = 0;
        QMessageBox::warning(this,tr("Tips"),tr("等待门开启"));
        propertyAnimation->setStartValue(QRect(this->rect().width()-ui->widget->width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->setEndValue(QRect(this->rect().width(),350,ui->widget->width(),ui->widget->height()));
        propertyAnimation->start();
        side_widget_flag = true;
        ui->lineEdit_3->setText("门已打开");
        reword_str = str + " 开门成功";
        reword.append(reword_str);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        success_open_door = true;
        m_z_kai->play();

    }
    else
    {
        QMessageBox::warning(this,tr("Tips"),tr("密码错误"));
        password.clear();
        password_flag = 0;
        reword_str = str + " 开门失败";
        reword.append(reword_str);
    }
    reword_str.clear();
}

//开锁记录
int reword_flag=0;
void MainWindow::on_pushButton_19_clicked()
{
    click->play();
    reword_flag++;
    ui->lineEdit_3->clear();
    if(reword_flag == 1)
    {
        ui->lineEdit_3->setText("Record viewing...");
        if(!reword.isEmpty())
        {
            int len = reword.length();
            int i =0;
            ui->textEdit->clear();
            for(i=0;i<len;i++)
            {
                ui->textEdit->append(reword[i]);
            }
            propertyAnimation2->setStartValue(QRect(this->rect().width(),400,ui->textEdit->width(),ui->textEdit->height()));
            propertyAnimation2->setEndValue(QRect(this->rect().width()-ui->textEdit->width(),400,ui->textEdit->width(),ui->textEdit->height()));
            propertyAnimation2->start();
        }
        else
        {
            ui->lineEdit_3->setText("No Record");
        }

    }
    else if(reword_flag >= 2)
    {
        reword_flag=0;
        if(!reword.isEmpty())
        {
            propertyAnimation2->setStartValue(QRect(this->rect().width()-ui->textEdit->width(),400,ui->textEdit->width(),ui->textEdit->height()));
            propertyAnimation2->setEndValue(QRect(this->rect().width(),400,ui->textEdit->width(),ui->textEdit->height()));
            propertyAnimation2->start();
        }
    }

}

//锁门
void MainWindow::on_pushButton_18_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QString reword_str;
    if(success_open_door)
    {
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_18->setIcon(QIcon(":/锁关.png"));
        reword_str = str + " 门已锁好";
        serial->write("cc");
        success_open_door = false;
        ui->lineEdit_3->setText("门已关闭");
        m_z_kai->play();
    }
}


//日，夜间模式
int night_flag=0;
void MainWindow::on_pushButton_20_clicked()
{
    click->play();
    night_flag++;
    if(night_flag==1 && serial->isOpen())
    {
        serial->write("dd");
        ui->lineEdit_3->setText("夜间模式");
        ui->pushButton_20->setIcon(QIcon(":/night.png"));
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_18->setEnabled(false);
        this->setStyleSheet("MainWindow{border-image: url(:/bj_night.jpg);}");
        ui->widget->move(this->rect().width() + ui->widget->width(),350);
        ui->textEdit->move(this->rect().width() + ui->textEdit->width(),400);

    }
    else if(night_flag >= 2 && serial->isOpen())
    {
        night_flag = 0;
        serial->write("ee");
        ui->lineEdit_3->setText("日间模式");
        ui->pushButton_20->setIcon(QIcon(":/day.png"));
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_18->setEnabled(true);
        this->setStyleSheet("MainWindow{border-image: url(:/bj.jpg);}");
    }
}

