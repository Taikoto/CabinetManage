#ifndef ZDIYLISTITEM_H
#define ZDIYLISTITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include <QIcon>
#include <QSignalMapper>

namespace ZDiy {

//每列 控件类型
enum ColumnType {
    Label
    , Button
    , CheckBox
    , LineEdit
};

//每列 控件参数
typedef struct {
    ColumnType type;//控件类型
    int maxWidth;   //最大宽度
}ColumnData;

typedef QList<ColumnData> ColumnDataList;

}



class ZDiyListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ZDiyListItem(QWidget *parent = 0);
    ~ZDiyListItem();

    void setWidget(ZDiy::ColumnDataList list);
    void setText(int no, QString text);
    QString text(int no);
    void setIcon(int no, QIcon icon, int w, int h);
    void setCheckBoxIcon(int no, QString checked, QString unChecked, int w, int h);
    void setChecked(int no, bool checked);
    bool isChecked(int no);

    bool isClicked(){ return m_isClicked; }
    void setClicked(bool click){ m_isClicked = click; update(); }//设置选中状态，并刷新界面
    void sendClicked(){ checkedSlot(); }//产生项目被点击事件，发射点击事件
    void setLineNumber(int number){ mLineNumber = number; }//设置行号

signals:
    void clicked(int lineNumber);
    void textChanged(int itemNo, int widgetNo);

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    QHBoxLayout hLayout;
    QList<QWidget*> widgetList;
    ZDiy::ColumnDataList columnList;//保存控件参数表
    bool m_isClicked;//选中flag
    int mLineNumber;//行号
    QSignalMapper *signalMapper;

private slots:
    void checkedSlot();//项目被点击槽
    void signalMapperSlot(QWidget *w);
};

#endif // ZDIYLISTITEM_H
