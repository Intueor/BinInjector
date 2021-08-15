#ifndef DIALOGERROR_H
#define DIALOGERROR_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogError; }

//== КЛАССЫ.
/// Класс окна диалога.
class DialogError : public QDialog
{
	Q_OBJECT

public:
	/// Конструктор.
	explicit DialogError(QWidget* p_parent = nullptr);
							///< \param[in] p_parent Указатель на родительский виджет.
	/// Деструктор.
	~DialogError();
	/// Установка текста лога.
	void SetLogText(QString strText);
							///< \param[in] a_strText Строка с текстом лога.
	/// Обновление языка.
	void Retranslate();

private:
	Ui::DialogError* p_ui; ///< Указатель на UI.
};

#endif // DIALOGERROR_H
