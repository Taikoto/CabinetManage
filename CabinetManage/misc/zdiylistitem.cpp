#include "zdiylistitem.h"

#include <QLabel>
#include <QToolButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QPainter>
#include <QDebug>

using namespace ZDiy;

ZDiyListItem::ZDiyListItem(QWidget *parent) :
    QWidget(parent)
{
    hLayout.setSpacing(0);//控件间隔
    hLayout.setContentsMargins(0, 0, 0, 0);//四周间隔
    this->setLayout(&hLayout);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(signalMapperSlot(QWidget*)));

    m_isClicked = false;
    mLineNumber = 0;
}

ZDiyListItem::~ZDiyListItem()
{
    for(int i=0; i < widgetList.length(); i++) delete widgetList[i];
}

void ZDiyListItem::setWidget(ColumnDataList list)
{
    QWidget *w = NULL;
    columnList = list;
    for(int i=0; i < list.length(); i++)
    {
        switch (list[i].type) {
        case Label:
            w = new QLabel(this);
            ((QLabel*)w)->setAlignment(Qt::AlignCenter);
            break;
        case Button:
            w = new QToolButton(this);
            ((QToolButton*)w)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            ((QToolButton*)w)->setAutoRaise(true);
            break;
        case CheckBox:
            w = new QCheckBox(this);
            ((QCheckBox*)w)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            connect(((QCheckBox*)w), SIGNAL(clicked()), this, SLOT(checkedSlot()));
            break;
        case LineEdit:
            w = new QLineEdit(this);
            ((QLineEdit*)w)->setAlignment(Qt::AlignCenter);
            ((QLineEdit*)w)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            ((QLineEdit*)w)->setStyleSheet("border:0px;background-color:rgba(0,0,0,0);");
            connect(((QLineEdit*)w), SIGNAL(textChanged(QString)), signalMapper, SLOT(map()));
            signalMapper->setMapping(w, w);
            break;
        default: return;
        }
        if(list[i].maxWidth > 0) {
            w->setMaximumWidth(list[i].maxWidth);
            //w->setMinimumWidth(list[i].maxWidth);
        }
        hLayout.addWidget(w);
        widgetList.append(w);
    }
}

void ZDiyListItem::setText(int no, QString text)
{
    if( no >= widgetList.length() || no < 0 ) return ;

    switch (columnList[no].type) {
    case Label:     ((QLabel*)widgetList[no])->setText(text); break;
    case Button:    ((QToolButton*)widgetList[no])->setText(text); break;
    case CheckBox:  ((QCheckBox*)widgetList[no])->setText(text); break;
    case LineEdit:  ((QLineEdit*)widgetList[no])->setText(text); break;
    default: break;
    }
}

QString ZDiyListItem::text(int no)
{
    if( no >= widgetList.length() || no < 0 ) return "";

    switch (columnList[no].type) {
    case Label:     return ((QLabel*)widgetList[no])->text();
    case Button:    return ((QToolButton*)widgetList[no])->text();
    case CheckBox:  return ((QCheckBox*)widgetList[no])->text();
    case LineEdit:  return ((QLineEdit*)widgetList[no])->text();
    default: break;
    }
    return "";
}

void ZDiyListItem::setIcon(int no, QIcon icon, int w, int h)
{
    if( no >= widgetList.length() || no < 0 ) return ;

    if(w < 5) w = widgetList[no]->width();
    if(h < 5) h = widgetList[no]->height();

    switch (columnList[no].type) {
    case Label:
        ((QLabel*)widgetList[no])->setPixmap(icon.pixmap(w, h));
        break;
    case Button:
        ((QToolButton*)widgetList[no])->setIcon(icon);
        ((QToolButton*)widgetList[no])->setIconSize(QSize(w, h));
        break;
    default:
        break;
    }
}

void ZDiyListItem::setCheckBoxIcon(int no, QString checked, QString unChecked, int w, int h)
{
    if( no >= widgetList.length() || no < 0 ) return ;

    if( columnList[no].type == CheckBox ){
        QString style = "QCheckBox::indicator{ ";
        style += "width: " + QString::number(w) + "px;";
        style += "height: " + QString::number(h) + "px; }\n";
        style += "QCheckBox::indicator:unchecked{ ";
        style += "image: url(" + unChecked + "); }\n";
        style += "QCheckBox::indicator:checked{ ";
        style += "image: url(" + checked + "); }\n";
        ((QCheckBox*)widgetList[no])->setStyleSheet(style);
    }
}

void ZDiyListItem::setChecked(int no, bool checked)
{
    if( no >= widgetList.length() || no < 0 ) return ;

    switch (columnList[no].type) {
    case CheckBox:  ((QCheckBox*)widgetList[no])->setChecked(checked); break;
    default: break;
    }
}

bool ZDiyListItem::isChecked(int no)
{
    if( no >= widgetList.length() || no < 0 ) return false;

    switch (columnList[no].type) {
    case CheckBox:  return ((QCheckBox*)widgetList[no])->isChecked();
    default: break;
    }
    return false;
}

void ZDiyListItem::mouseReleaseEvent(QMouseEvent *)
{
    checkedSlot();
}

void ZDiyListItem::paintEvent(QPaintEvent *)
{
    if( m_isClicked ){
        QPainter painter(this);
        painter.setPen(QColor(200, 170, 200));//设置画笔颜色
        painter.setBrush(QColor(0, 0, 0, 0));//矩形颜色
        painter.drawRoundedRect(0, 0, width()-1, height()-1, 3, 3);//画圆弧矩形
    }

    //QWidget::paintEvent(event);
}




/**
 ********************************************************************
 * private slots:
 ********************************************************************
*/

void ZDiyListItem::checkedSlot()
{
    m_isClicked = true;
    update();//产生重绘事件

    emit clicked(mLineNumber);
}

void ZDiyListItem::signalMapperSlot(QWidget *w)
{
    for( int i = 0; i < widgetList.length(); i++ )
    {
        if( widgetList[i] == w ){
            emit textChanged(mLineNumber, i);
            break;
        }
    }
}
