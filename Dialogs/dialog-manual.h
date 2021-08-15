#ifndef DIALOGMANUAL_H
#define DIALOGMANUAL_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogManual; }

//== КЛАССЫ.
/// Класс окна диалога.
class DialogManual : public QDialog
{
	Q_OBJECT

public:
	/// Конструктор.
	explicit DialogManual(QWidget* p_parent = nullptr);
							///< \param[in] p_parent Указатель на родительский виджет.
	/// Деструктор.
	~DialogManual();
	/// Обновление языка.
	void Retranslate();

private:
	Ui::DialogManual* p_ui; ///< Указатель на UI.
};

#endif // DIALOGMANUAL_H
