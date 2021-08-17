#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogAbout; }

//== КЛАССЫ.
/// Класс окна диалога.
class DialogAbout : public QDialog
{
	Q_OBJECT

public:
	/// Конструктор.
	explicit DialogAbout(QWidget* p_parent = nullptr);
							///< \param[in] p_parent Указатель на родительский виджет.
	/// Деструктор.
	~DialogAbout();
	/// Обновление языка.
	void Retranslate();

private:
	Ui::DialogAbout* p_ui; ///< Указатель на UI.
};

#endif // DIALOGABOUT_H
