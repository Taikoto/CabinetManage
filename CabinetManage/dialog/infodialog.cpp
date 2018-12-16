#include "infodialog.h"
#include "ui_infodialog.h"

#include <QPainter>

InfoDialog* InfoDialog::m_dialog = NULL;

InfoDialog::InfoDialog(QWidget *parent) :
    Dialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

InfoDialog* InfoDialog::instance(QWidget *parent)
{
    if(m_dialog == NULL) m_dialog = new InfoDialog(parent);
    return m_dialog;
}

void InfoDialog::setTitle(QString text)
{
    ui->title->setText(text);
}

void InfoDialog::setText(QString text)
{
    ui->label_info->setText(text);
}

void InfoDialog::setOkHide()
{
    ui->pb_ok->hide();
}

void InfoDialog::setOkShow()
{
    ui->pb_ok->show();
}

void InfoDialog::setCancelHide()
{
    ui->pb_cancel->hide();
}

void InfoDialog::setCancelShow()
{
    ui->pb_cancel->show();
}

int InfoDialog::message(QWidget *parent, QString title, QString text, QMessageBox::StandardButton button)
{
    if(m_dialog == NULL) m_dialog = new InfoDialog(parent);
    m_dialog->setTitle(title);
    m_dialog->setText(text);
    switch (button) {
    case QMessageBox::NoButton: m_dialog->setOkHide(); m_dialog->setCancelHide(); break;
    case QMessageBox::Ok:       m_dialog->setOkShow(); m_dialog->setCancelHide(); break;
    case QMessageBox::Cancel:   m_dialog->setOkHide(); m_dialog->setCancelShow(); break;
    case QMessageBox::Default:  m_dialog->setOkShow(); m_dialog->setCancelShow(); break;
    default: break;
    }
    if(m_dialog->isHidden()) return m_dialog->exec();
    return 0;
}

int InfoDialog::information(QWidget *parent, QString text, QMessageBox::StandardButton button)
{
    return message(parent, tr("Information"), text, button);
}

int InfoDialog::error(QWidget *parent, QString text, QMessageBox::StandardButton button)
{
    return message(parent, tr("Error"), text, button);
}
