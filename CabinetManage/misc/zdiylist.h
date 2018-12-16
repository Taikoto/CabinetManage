#ifndef ZDIYLIST_H
#define ZDIYLIST_H

#include <QGroupBox>

#include "zdiylistitem.h"

namespace Ui {
class ZDiyList;
}

class ZDiyList : public QGroupBox
{
    Q_OBJECT

public:
    explicit ZDiyList(QWidget *parent = 0);
    ~ZDiyList();

    void setTitle(QString text);//设置标题
    void setPageItemCount(int number);//设置每页显示项目个数

    void addColumn(ZDiy::ColumnType type = ZDiy::Label, QString text = 0, int maxWidth = 0);//新增标题

    void addItem();//新增item
    QString itemText(int itemNo, int widgetNo);
    void setItemText(int itemNo, int widgetNo, QString text);//设置item文本
    void setItemIcon(int itemNo, int widgetNo, QIcon icon, int w = 0, int h = 0);//设置item图标
    void setCheckBoxIcon(int itemNo, int widgetNo, QString checked, QString unChecked, int w = 0, int h = 0);
    void setItemChecked(int itemNo, int widgetNo, bool checked);//设置Check状态
    bool itemIsChecked(int itemNo, int widgetNo);//返回Check是否被选中

    void sendClicked(int itemNo);//产生点击行事件

    void setItemHeight(int height);//设置每行的高度
    void setItemWidth(int width);//设置每行各项目的宽度

    void setPreviousButtonIcon(QIcon icon, int w = 30, int h = 30);//设置上一页按钮图标
    void setNextButtonIcon(QIcon icon, int w = 30, int h = 30);//设置下一页按钮图标

    void showPage(int index);//显示一页item

    int currentIndex(){ return index; }
    int itemCount(){ return itemList.count(); }//列表项目总数
    int length(){ return itemList.count(); }

    void delItem(int index);//删除项目
    void clear();//清空项目

signals:
    void itemClicked(int index);
    void textChanged(int itemNo, int widgetNo);

protected:
    void resizeEvent(QResizeEvent *);//调整大小事件
    void paintEvent(QPaintEvent *);//窗口重绘事件

private slots:
    void itemIsClicked(int index);//项目被点击

    void on_previousButton_clicked();

    void on_nextButton_clicked();

private:
    Ui::ZDiyList *ui;
    QList<QWidget*> titleWidgetList;//保存标题栏界面指针
    ZDiy::ColumnDataList columnList;//存储每行的控件参数
    QList<ZDiyListItem*> itemList;
    int listHeight;//listWidget窗体的高度
    //int listWidth;//listWidget窗体的宽度
    int showNumber;//每一页显示的项目个数
    int currentPage;//当前显示的第几页
    int pageCount;//总页数
    int index;//被选中index
};

#endif // ZDIYLIST_H
