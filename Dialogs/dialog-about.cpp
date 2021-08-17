//== ВКЛЮЧЕНИЯ.
#include "dialog-about.h"
#include "ui_dialog-about.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс окна диалога.
// Конструктор.
DialogAbout::DialogAbout(QWidget* p_parent) : QDialog(p_parent), p_ui(new Ui::DialogAbout)
{
	p_ui->setupUi(this);
}

// Деструктор.
DialogAbout::~DialogAbout()
{
	delete p_ui;
}

// Обновление языка.
void DialogAbout::Retranslate()
{
	p_ui->retranslateUi(this);
}
