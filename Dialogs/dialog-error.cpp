//== ВКЛЮЧЕНИЯ.
#include "dialog-error.h"
#include "ui_dialog-error.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс окна диалога.
// Конструктор.
DialogError::DialogError(QWidget* p_parent) : QDialog(p_parent), p_ui(new Ui::DialogError)
{
	p_ui->setupUi(this);
}

// Деструктор.
DialogError::~DialogError()
{
	delete p_ui;
}

// Установка текста лога.
void DialogError::SetLogText(QString strText)
{
	p_ui->plainTextEdit_Log->document()->setPlainText(strText);
}

// Обновление языка.
void DialogError::Retranslate()
{
	p_ui->retranslateUi(this);
}
