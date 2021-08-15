#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>
#include <QSettings>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogSettings; }

//== КЛАССЫ.
/// Класс окна диалога.
class DialogSettings : public QDialog
{
	Q_OBJECT

public:
	/// Конструктор.
	explicit DialogSettings(QWidget* p_parent = nullptr);
							///< \param[in] p_parent Указатель на родительский виджет.
	/// Деструктор.
	~DialogSettings();
	/// Сохранение настроек.
	bool SaveSettings(QSettings* p_QSettings);
							///< \param[in] p_QSettings Указатель на интерфейс настроек.
							///< \param[out] true - при успехе.
	/// Обновление языка.
	void Retranslate();

public:
	Ui::DialogSettings* p_ui; ///< Указатель на UI.
};

#endif // DIALOGSETTINGS_H
