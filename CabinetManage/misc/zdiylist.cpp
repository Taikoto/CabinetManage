#include "zdiylist.h"
#include "ui_zdiylist.h"

#include <QLabel>
#include <QResizeEvent>
#include <QDebug>

using namespace ZDiy;

ZDiyList::ZDiyList(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ZDiyList)
{
    ui->setupUi(this);
    listHeight = 0;
    showNumber = 5;     //默认每页显示5个
    currentPage = -1;   //初始化当前显示的第几页
    pageCount = 0;      //初始化总页数
    index = 0;          //初始化被选中index
}

ZDiyList::~ZDiyList()
{
    delete ui;
    for(int i=0; i < titleWidgetList.length(); i++) delete titleWidgetList[i];
    for(int i=0; i < itemList.length(); i++) delete itemList[i];
}

void ZDiyList::setTitle(QString text)
{
    ui->title->setText(text);
}

//设置每页显示项目个数
void ZDiyList::setPageItemCount(int number)
{
    showNumber = number;
    //setItemHeight( listHeight / showNumber );
}

void ZDiyList::addColumn(ColumnType type, QString text, int maxWidth)
{
    QLabel *widget = new QLabel(text, this);//加了this,可跟随本类一起释放
    widget->setAlignment(Qt::AlignCenter);//设置居中
    //qDebug() << listHeight << listWidth;

    ui->titleHLayout->addWidget(widget);//将控件加入布局
    titleWidgetList.append(widget);//保存指针

    if(maxWidth > 10) {
        widget->setMaximumWidth(maxWidth);
        //widget->setMinimumWidth(maxWidth);
    }

    //生成控件参数
    ColumnData column;
    column.type = type;
    column.maxWidth = maxWidth;
    columnList.append(column);
}

void ZDiyList::addItem()
{
    ZDiyListItem *widget = new ZDiyListItem(this);
    widget->setWidget(columnList);//设置界面
    widget->setLineNumber(itemList.length());//设置行号
    connect(widget, SIGNAL(clicked(int)), this, SIGNAL(itemClicked(int)));
    connect(widget, SIGNAL(clicked(int)), this, SLOT(itemIsClicked(int)));
    connect(widget, SIGNAL(textChanged(int,int)), this, SIGNAL(textChanged(int,int)));

    ui->listVLayout->insertWidget(ui->listVLayout->count()-1, widget);
    itemList.append(widget);

    //计算总页数
    pageCount = itemList.length() / showNumber;
    if(itemList.length()%showNumber != 0) pageCount++;//没有一页，做一页计算

    setItemHeight( listHeight / showNumber );//设置每行高度

    showPage(pageCount-1);
}

QString ZDiyList::itemText(int itemNo, int widgetNo)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return "";

    return itemList[itemNo]->text(widgetNo);
}

void ZDiyList::setItemText(int itemNo, int widgetNo, QString text)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return ;

    itemList[itemNo]->setText(widgetNo, text);
}

void ZDiyList::setItemIcon(int itemNo, int widgetNo, QIcon icon, int w, int h)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return ;

    itemList[itemNo]->setIcon(widgetNo, icon, w, h);
}

//设置Check box图标
void ZDiyList::setCheckBoxIcon(int itemNo, int widgetNo, QString checked, QString unChecked, int w, int h)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return ;

    if(w < 1) w = itemList[itemNo]->width();
    if(h < 1) h = itemList[itemNo]->height();
    itemList[itemNo]->setCheckBoxIcon(widgetNo, checked, unChecked, w, h);
}

//设置Check状态
void ZDiyList::setItemChecked(int itemNo, int widgetNo, bool checked)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return ;

    itemList[itemNo]->setChecked(widgetNo, checked);
}

//Check是否被选中
bool ZDiyList::itemIsChecked(int itemNo, int widgetNo)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return false;

    return itemList[itemNo]->isChecked(widgetNo);
}

//产生点击行事件
void ZDiyList::sendClicked(int itemNo)
{
    if( itemNo >= itemList.length() || itemNo < 0 ) return ;

    itemList[itemNo]->sendClicked();
}

//设置每行的高度
void ZDiyList::setItemHeight(int height)
{
    if( height < 20 ) height = 20;
    if( height > 50 ) height = 50;
    //更新设置每个项目的最大高度
    for( int i = 0; i < itemList.length(); i++ )
    {
        itemList[i]->setMinimumHeight(height-3);
        //itemList[i]->setMaximumHeight(height);
    }
}

//设置每行各项目的宽度
void ZDiyList::setItemWidth(int width)
{
    width = width / columnList.length();
    if( width < 20 ) return ;

    //计算 有些控件设定了maxWidth，将多占（少占）的空间均分给其余控件
    int count = 0;
    for( int i = 0; i < columnList.length(); i++ )
    {
        if( columnList[i].maxWidth != 0 ){
            count += width - columnList[i].maxWidth;
        }
    }
    count = count / columnList.length();
    width += count;

    //设置每行每项的最大宽度
    for( int i = 0; i < columnList.length(); i++ )
    {
        if( columnList[i].maxWidth == 0 ){
            columnList[i].maxWidth = width;
            titleWidgetList[i]->setMaximumWidth(width);
        }
    }
}

void ZDiyList::setPreviousButtonIcon(QIcon icon, int w, int h)
{
    ui->previousButton->setIcon(icon);
    ui->previousButton->setIconSize(QSize(w, h));
}

void ZDiyList::setNextButtonIcon(QIcon icon, int w, int h)
{
    ui->nextButton->setIcon(icon);
    ui->nextButton->setIconSize(QSize(w, h));
}

void ZDiyList::showPage(int index)
{
    if(pageCount <= 0) return ;
    if(index < 0 || index >= pageCount) return ;
    //列表翻页不循环

    //隐藏所有item
    for(int i = 0; i < itemList.length(); i++) itemList[i]->hide();

    //显示一页item
    int count = (index+1)*showNumber;
    for(int i = index*showNumber; i < count && i < itemList.length(); i++)
        itemList[i]->show();

    currentPage = index;//更新当前显示的页面

    ui->pageLabel->setText(QString("%1 / %2").arg(currentPage+1).arg(pageCount));
}

void ZDiyList::delItem(int index)
{
    if( index >= itemList.length() || index < 0 ) return ;

    this->index = 0;

    ui->listVLayout->removeWidget(itemList[index]);

    delete itemList[index];
    itemList.removeAt(index);

    //刷新行号
    for( int i = 0; i < itemList.length(); i++ ) itemList[i]->setLineNumber(i);

    //计算总页数
    pageCount = itemList.length() / showNumber;
    if(itemList.length()%showNumber != 0) pageCount++;//没有一页，做一页计算

    showPage(currentPage);
}

void ZDiyList::clear()
{
    for(int i=0; i < itemList.length(); i++) delete itemList[i];
    itemList.clear();
    pageCount = 0;
    index = 0;//初始化被选中index

    //计算总页数
    pageCount = itemList.length() / showNumber;
    if(itemList.length()%showNumber != 0) pageCount++;//没有一页，做一页计算

    showPage(0);
}



//调整大小事件
void ZDiyList::resizeEvent(QResizeEvent *)
{
    listHeight = ui->listWidget->height();

    setItemHeight( listHeight / showNumber );
    setItemWidth(ui->listWidget->width());
    showPage(currentPage);
}

void ZDiyList::paintEvent(QPaintEvent *event)
{
    //listHeight = ui->listWidget->height();
    //listWidth = ui->listWidget->width();
    //qDebug() << listHeight << listWidth;
    QGroupBox::paintEvent(event);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

//项目被点击
void ZDiyList::itemIsClicked(int index)
{
    this->index = index;//保存当前选中行号
    for( int i = 0; i < itemList.length(); i++ )
    {
        if( index != i ) itemList[i]->setClicked(false);
    }

    // 若当前行不是显示的这页，则翻页
    if( itemList[index]->isHidden() ) showPage( index / showNumber );
}

void ZDiyList::on_previousButton_clicked()
{
    showPage(currentPage - 1);
}

void ZDiyList::on_nextButton_clicked()
{
    showPage(currentPage + 1);
}

