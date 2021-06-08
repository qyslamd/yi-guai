#include "widget.h"
#include "ui_widget.h"
#include "MyDef.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initData();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initData()
{
    int totalNum = 50;
    int  redNum = 0;
    int  blueNum = 0;
    int  yellowNum = 0;
    m_model = new QStandardItemModel();

    for (int i = 0; i < totalNum; ++i) {
        QStandardItem *Item = new QStandardItem;

        ItemData itemData;

        itemData.name = QString("Name %1").arg(i);
        itemData.tel = QString("TEL:1331234567%1").arg(i);
        itemData.icon = style()->standardPixmap(QStyle::SP_ArrowBack);
        int randNum = rand()% 3;
        ItemStatus itemStatus;
        switch (randNum) {
        case 0:
            itemStatus = S_RED;
            redNum++;
            break;
        case 1:
            itemStatus = S_BLUE;
            blueNum++;
            break;
        case 2:
            itemStatus = S_YELLOW;
            yellowNum++;
            break;
        }
        Item->setData(itemStatus,Qt::UserRole);  // 单一存取
        Item->setData(QVariant::fromValue(itemData),Qt::UserRole+1);//整体存取

        m_model->appendRow(Item);      //追加Item
    }
}

