#include "myinputpanel.h"
#include <QSignalMapper>
#include <QDebug>

//! [0]

MyInputPanel::MyInputPanel()
    : QWidget(0, Qt::WindowStaysOnTopHint),
      lastFocusedWidget(0)
{
    form.setupUi(this);
    setStyleSheet("font: 12px;");

    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);//���ر�����

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));
    connect(form.horizontalSlider,SIGNAL(valueChanged(int)),
            this,SLOT(onHorizontalSliderValueChanged(int)));

    setMapper();        //����ӳ��
    connectMapper();    //���Ӱ���
    capsLock = false;

    shouldMove = false;
    setAttribute(Qt::WA_TranslucentBackground, true);//���ñ���͸��

    //�������
    QDesktopWidget* desktop = qApp->desktop();
    move((desktop->width() - width())/2, (desktop->height() - height())/2);
}

MyInputPanel::~MyInputPanel()
{

}

/*���ƣ�����ӳ��
*���ܣ��԰�������ӳ��
*/
void MyInputPanel::setMapper()
{
    //=============================char��ĸ=================================================
        signalMapper.setMapping(form.toolButton_a, form.toolButton_a);
        signalMapper.setMapping(form.toolButton_b, form.toolButton_b);
        signalMapper.setMapping(form.toolButton_c, form.toolButton_c);
        signalMapper.setMapping(form.toolButton_d, form.toolButton_d);
        signalMapper.setMapping(form.toolButton_e, form.toolButton_e);
        signalMapper.setMapping(form.toolButton_f, form.toolButton_f);
        signalMapper.setMapping(form.toolButton_g, form.toolButton_g);
        signalMapper.setMapping(form.toolButton_h, form.toolButton_h);
        signalMapper.setMapping(form.toolButton_i, form.toolButton_i);
        signalMapper.setMapping(form.toolButton_j, form.toolButton_j);
        signalMapper.setMapping(form.toolButton_k, form.toolButton_k);
        signalMapper.setMapping(form.toolButton_l, form.toolButton_l);
        signalMapper.setMapping(form.toolButton_m, form.toolButton_m);
        signalMapper.setMapping(form.toolButton_n, form.toolButton_n);
        signalMapper.setMapping(form.toolButton_o, form.toolButton_o);
        signalMapper.setMapping(form.toolButton_p, form.toolButton_p);
        signalMapper.setMapping(form.toolButton_q, form.toolButton_q);
        signalMapper.setMapping(form.toolButton_r, form.toolButton_r);
        signalMapper.setMapping(form.toolButton_s, form.toolButton_s);
        signalMapper.setMapping(form.toolButton_t, form.toolButton_t);
        signalMapper.setMapping(form.toolButton_u, form.toolButton_u);
        signalMapper.setMapping(form.toolButton_v, form.toolButton_v);
        signalMapper.setMapping(form.toolButton_w, form.toolButton_w);
        signalMapper.setMapping(form.toolButton_x, form.toolButton_x);
        signalMapper.setMapping(form.toolButton_y, form.toolButton_y);
        signalMapper.setMapping(form.toolButton_z, form.toolButton_z);

    //===========================num����===========================================
        signalMapper.setMapping(form.toolButton_num0, form.toolButton_num0);
        signalMapper.setMapping(form.toolButton_num1, form.toolButton_num1);
        signalMapper.setMapping(form.toolButton_num2, form.toolButton_num2);
        signalMapper.setMapping(form.toolButton_num3, form.toolButton_num3);
        signalMapper.setMapping(form.toolButton_num4, form.toolButton_num4);
        signalMapper.setMapping(form.toolButton_num5, form.toolButton_num5);
        signalMapper.setMapping(form.toolButton_num6, form.toolButton_num6);
        signalMapper.setMapping(form.toolButton_num7, form.toolButton_num7);
        signalMapper.setMapping(form.toolButton_num8, form.toolButton_num8);
        signalMapper.setMapping(form.toolButton_num9, form.toolButton_num9);

    //=========================symbol����=========================================
        signalMapper.setMapping(form.toolButton_dot, form.toolButton_dot);
        signalMapper.setMapping(form.toolButton_line1, form.toolButton_line1);
        signalMapper.setMapping(form.toolButton_line2, form.toolButton_line2);
        signalMapper.setMapping(form.toolButton_wenhao, form.toolButton_wenhao);
        signalMapper.setMapping(form.toolButton_line3, form.toolButton_line3);

        signalMapper.setMapping(form.toolButton_space, form.toolButton_space);
}



/*���ƣ����Ӱ���ӳ��
*���ܣ������ťʱ�򣬽��������ӵ��ź�ӳ����
*
*/
void MyInputPanel::connectMapper()
{
    connect(form.toolButton_a, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_b, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_c, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_d, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_e, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_f, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_g, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_h, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_i, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_j, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_k, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_l, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_n, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_m, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_o, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_p, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_q, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_r, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_s, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_t, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_u, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_v, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_w, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_x, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_y, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_z, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
//=======================����======================================
    connect(form.toolButton_num0, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(form.toolButton_num1, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num2, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num3, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num4, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num5, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num6, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num7, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num8, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_num9, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));

//=======================����=====================================
    connect(form.toolButton_dot, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_line1, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_line2, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_wenhao, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));
    connect(form.toolButton_line3, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));

//=================================================================
    connect(form.toolButton_space, SIGNAL(clicked()),
        &signalMapper, SLOT(map()));

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),    //����Ҫ����Ĵ���
            this, SLOT(buttonClicked(QWidget*)));
}

bool MyInputPanel::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Paint:
        if (lastFocusedWidget) lastFocusedWidget->activateWindow();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void MyInputPanel::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) return ;
    shouldMove = true;
    //���㣺�������λ�� - ��������Ͻ� = �봰�����Ͻǵľ���
    formerMousePos = event->globalPos() - this->pos();
}

void MyInputPanel::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) return ;
    shouldMove = false;
}

void MyInputPanel::mouseMoveEvent(QMouseEvent *event)
{
    //���㣺�������λ�� - �봰�����Ͻǵľ��� = ��������Ͻ�
    QPoint pos = event->globalPos() - formerMousePos;

    if (shouldMove) move(pos.x(), pos.y());
}

void MyInputPanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0, 0));//���û�����ɫ
    painter.setBrush(QColor(200, 200, 200, 200));//������ɫ
    painter.drawRoundedRect(0, 0, width(), height(), 3, 3);//��Բ������
}

//���ƣ����潹�㴰��
void MyInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        lastFocusedWidget = newFocus;
    }
}


//���ܣ�������뷨��������һ���źţ����͵�����ַ���Ҫ����ĵط���lineEdit��
void MyInputPanel::buttonClicked(QWidget *w)
{
    QByteArray dat = ((QToolButton*)w)->text().toLatin1();
    QChar chr = ' ';
    if( dat.length() > 0 ) chr = (QChar)dat[0];
    emit characterGenerated(chr);
}


//���ܰ�����ɾ��
void MyInputPanel::on_toolButton_backspace_clicked()
{
    emit delter();
}


//���ܰ������������
void MyInputPanel::on_toolButton_left_clicked()
{
    emit left_move();
}

//���ܰ������������
void MyInputPanel::on_toolButton_right_clicked()
{
    emit right_move();
}

//���ܰ������������
void MyInputPanel::on_toolButton_up_clicked()
{
    emit up_move();
}

//���ܰ������������
void MyInputPanel::on_toolButton_down_clicked()
{
    emit down_move();
}

//���ƣ����뷨����͸���ȵ���
void MyInputPanel::onHorizontalSliderValueChanged(int value)
{
    form.horizontalSlider->setRange(100, 255);//�趨�������Ĵ�С��Χ
    qreal opacity=(qreal)((1.0/256.0)*value);
    setWindowOpacity(opacity);             //����͸����
    form.lcdNumber->display(value);
}

void MyInputPanel::on_CapsLock_clicked()
{
    capsLock = !capsLock;
    //��д
    if( capsLock ){
        form.toolButton_a->setText(form.toolButton_a->text().toUpper());
        form.toolButton_b->setText(form.toolButton_b->text().toUpper());
        form.toolButton_c->setText(form.toolButton_c->text().toUpper());
        form.toolButton_d->setText(form.toolButton_d->text().toUpper());
        form.toolButton_e->setText(form.toolButton_e->text().toUpper());
        form.toolButton_f->setText(form.toolButton_f->text().toUpper());
        form.toolButton_g->setText(form.toolButton_g->text().toUpper());
        form.toolButton_h->setText(form.toolButton_h->text().toUpper());
        form.toolButton_i->setText(form.toolButton_i->text().toUpper());
        form.toolButton_j->setText(form.toolButton_j->text().toUpper());
        form.toolButton_k->setText(form.toolButton_k->text().toUpper());
        form.toolButton_l->setText(form.toolButton_l->text().toUpper());
        form.toolButton_m->setText(form.toolButton_m->text().toUpper());
        form.toolButton_n->setText(form.toolButton_n->text().toUpper());
        form.toolButton_o->setText(form.toolButton_o->text().toUpper());
        form.toolButton_p->setText(form.toolButton_p->text().toUpper());
        form.toolButton_q->setText(form.toolButton_q->text().toUpper());
        form.toolButton_r->setText(form.toolButton_r->text().toUpper());
        form.toolButton_s->setText(form.toolButton_s->text().toUpper());
        form.toolButton_t->setText(form.toolButton_t->text().toUpper());
        form.toolButton_u->setText(form.toolButton_u->text().toUpper());
        form.toolButton_v->setText(form.toolButton_v->text().toUpper());
        form.toolButton_w->setText(form.toolButton_w->text().toUpper());
        form.toolButton_x->setText(form.toolButton_x->text().toUpper());
        form.toolButton_y->setText(form.toolButton_y->text().toUpper());
        form.toolButton_z->setText(form.toolButton_z->text().toUpper());
    } else {
        form.toolButton_a->setText(form.toolButton_a->text().toLower());
        form.toolButton_b->setText(form.toolButton_b->text().toLower());
        form.toolButton_c->setText(form.toolButton_c->text().toLower());
        form.toolButton_d->setText(form.toolButton_d->text().toLower());
        form.toolButton_e->setText(form.toolButton_e->text().toLower());
        form.toolButton_f->setText(form.toolButton_f->text().toLower());
        form.toolButton_g->setText(form.toolButton_g->text().toLower());
        form.toolButton_h->setText(form.toolButton_h->text().toLower());
        form.toolButton_i->setText(form.toolButton_i->text().toLower());
        form.toolButton_j->setText(form.toolButton_j->text().toLower());
        form.toolButton_k->setText(form.toolButton_k->text().toLower());
        form.toolButton_l->setText(form.toolButton_l->text().toLower());
        form.toolButton_m->setText(form.toolButton_m->text().toLower());
        form.toolButton_n->setText(form.toolButton_n->text().toLower());
        form.toolButton_o->setText(form.toolButton_o->text().toLower());
        form.toolButton_p->setText(form.toolButton_p->text().toLower());
        form.toolButton_q->setText(form.toolButton_q->text().toLower());
        form.toolButton_r->setText(form.toolButton_r->text().toLower());
        form.toolButton_s->setText(form.toolButton_s->text().toLower());
        form.toolButton_t->setText(form.toolButton_t->text().toLower());
        form.toolButton_u->setText(form.toolButton_u->text().toLower());
        form.toolButton_v->setText(form.toolButton_v->text().toLower());
        form.toolButton_w->setText(form.toolButton_w->text().toLower());
        form.toolButton_x->setText(form.toolButton_x->text().toLower());
        form.toolButton_y->setText(form.toolButton_y->text().toLower());
        form.toolButton_z->setText(form.toolButton_z->text().toLower());
    }
}
