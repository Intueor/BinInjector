//== ВКЛЮЧЕНИЯ.
#include "dialog-manual.h"
#include "ui_dialog-manual.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс окна диалога.
// Конструктор.
DialogManual::DialogManual(QWidget* p_parent) : QDialog(p_parent), p_ui(new Ui::DialogManual)
{
	p_ui->setupUi(this);
}

// Деструктор.
DialogManual::~DialogManual()
{
	delete p_ui;
}

// Обновление языка.
void DialogManual::Retranslate()
{
	p_ui->retranslateUi(this);
}
