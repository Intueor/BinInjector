//== ВКЛЮЧЕНИЯ.
#include <QFile>
#include <QTextStream>
#include "dialog-settings.h"
#include "ui_dialog-settings.h"
#include "bin-injector-defs.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс окна диалога.
// Конструктор.
DialogSettings::DialogSettings(QWidget* p_parent) : QDialog(p_parent), p_ui(new Ui::DialogSettings)
{
	p_ui->setupUi(this);
}

// Деструктор.
DialogSettings::~DialogSettings()
{
	delete p_ui;
}

// Сохранение настроек.
bool DialogSettings::SaveSettings(QSettings* p_QSettings)
{
	p_QSettings->setValue(m_chExplorer, p_ui->lineEdit_Explorer->text());
	p_QSettings->setValue(m_chAutoClose, p_ui->checkBox_AutoClose->isChecked());
	p_QSettings->sync();
	return p_QSettings->status() == QSettings::NoError;
}

// Обновление языка.
void DialogSettings::Retranslate()
{
	p_ui->retranslateUi(this);
}

