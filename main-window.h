#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//== ВКЛЮЧЕНИЯ.
#include <QMainWindow>
#include <QSettings>
#include <QLabel>
#include <QVector>
#include <QSplitter>
#include <QRadioButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QTimer>
#include <QTranslator>
#include "logger.h"
#include "main-hub.h"

#include "Dialogs/dialog-about.h"
#include "Dialogs/dialog-error.h"
#include "Dialogs/dialog-settings.h"
#include "Dialogs/dialog-manual.h"
#include "bin-injector-defs.h"

//== ПРОСТРАНСТВА ИМЁН.
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//== КЛАССЫ.
/// Класс главного окна.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	struct ShellRet
	{
		QString strOut;
		int iRes;
	};

public:
	/// Конструктор.
	explicit MainWindow(QString strExplPath, bool bSudo, bool bR, QTranslator* p_QTranslator,
						QApplication* p_QApplication, QWidget* p_parent = nullptr);
							///< \param[in] strExplPath Строка с путём к эксплореру по умолчанию (будет установлен при пустых настройках).
							///< \param[in] bSudo Признак требования запуска внутренних запросов с sudo (для отладки).
							///< \param[in] bR Признак запуска в тестовом режиме с аргументом -r (для блокировки переключения режимов загрузки).
							///< \param[in] p_QTranslator Указатель на переводчик.
							///< \param[in] p_parent Указатель на родительский виджет.
	/// Деструктор.
	~MainWindow();
	/// Процедуры при закрытии окна приложения.
	void closeEvent(QCloseEvent* p_Event);
							///< \param[in] p_Event Указатель на событие.
	/// Перехватчик событий.
	bool eventFilter(QObject* p_watched, QEvent* p_event);
							///< \param[in] p_watched
							///< \param[in] p_event Событие.
							///< \param[out] true - подтверждение обработки фильтром.
	/// Операции с образом.
	static bool ImageOps(char chOpType,  QString strImagePath = "", bool bDataType = DATA_TYPE_SINGLE_FILE,
						 QString strDataPath = "", bool bLogging = true);
							///< \param[in] chOpType Тип операции.
							///< \param[in] strImagePath Строка с путём к образу.
							///< \param[in] bDataType Выбор одиночного файла или папки.
							///< \param[in] strDataPath Строка с путём к файлу или папке.
							///< \param[in] bLogging true - использовать логирование.
							///< \param[out] true - при успехе.
	/// Вызов Shell с командной строкой.
	static ShellRet CallShell(QString& a_strCommand, bool bLogging = true, bool bOutEnabled = true);
							///< \param[in] a_strCommand Ссылка на строку с командой.
							///< \param[in] bLogging При true - отметка о возможной ошибке в логе.
							///< \param[in] bOutEnabled При true - отправка вывода с команды в файл.
							///< \param[out] Структура с данными по результатам, SHELL_COMMAND_CALL_ERROR в строке при ошибке вызова.
private slots:
	/// При срабатывании таймера задержки для клавиш.
	static void On_ButtonsDelayTimer();
	// При срабатывании таймера файловых операций.
	static void On_FileOperationsTimer();
	/// При нажатии на переключатель Папка.
	void on_radioButton_Folder_clicked(bool bChecked);
							///< \param[in] bChecked true - при выборе отправки структуры папки.
	/// При нажатии на переключатель Файл.
	void on_radioButton_Single_clicked(bool bChecked);
							///< \param[in] bChecked true - при выборе отправки одного файла.
	/// При нажатии на кнопку поиска и установки 'Папки'.
	void on_pushButton_Sel_Folder_clicked();
	/// При нажатии на кнопку поиска и установки 'Файла'.
	void on_pushButton_Sel_Single_clicked();
	/// При нажатии на кнопку поиска и установки 'Образа'.
	void on_pushButton_Sel_Image_clicked();
	/// При нажатии на кнопку отправки выбранного в 'Образ'.
	void on_pushButton_Send_clicked();
	/// При нажатии на кнопку монтирования и открытия Образа.
	void on_pushButton_Open_clicked();
	/// При нажатии на кнопку размотнирования 'Образа'.
	void on_pushButton_Close_clicked();
	/// При нажатии Enter в строке 'Папка'.
	void on_lineEdit_Folder_returnPressed();
	/// При нажатии Enter в строке 'Файл'.
	void on_lineEdit_Single_returnPressed();
	/// При нажатии Enter в строке 'Образ'.
	void on_lineEdit_Image_returnPressed();
	/// При выходе из строки 'Папка'.
	void on_lineEdit_Folder_editingFinished();
	/// При выходе из строки 'Файл'.
	void on_lineEdit_Single_editingFinished();
	/// При выходе из строки 'Образ'.
	void on_lineEdit_Image_editingFinished();
	/// При выборе пункта меню 'О программе'.
	void on_action_About_triggered();
	/// При выборе пункта меню 'Настройки'.
	void on_action_Settings_triggered();
	/// При переключении языка чекбоксом 'Русский'.
	void on_action_Russian_triggered(bool bChecked);
							///< \param[in] true - выбран русский.
	/// При переключении языка чекбоксом 'English'.
	void on_action_English_triggered(bool bChecked);
							///< \param[in] true - выбран английский.
	/// При выборе пункта меню 'Описание'.
	void on_action_Manual_triggered();

private:
	/// Установка состояний кнопок по корректности строк путей.
	static void SetButtonStatesByPathFlags();
	/// Вспомогательная функция для старта таймера операций с образом.
	static void ImageOpsTimerStartHelper();
	/// Вспомогательная функция блокировки\разблокировки части UI на время операций с образом.
	static void ImageOpsUIBlockerHelper(bool bEnabled);
							///< \param[in] bEnabled true - разрешить виджеты.
	/// Выполнение диалога об ошибке с логом.
	static void ExecErrorDialogWithLog();
	/// Вспомогательная функция логирования ошибки операций с образом.
	static void LogImgOpsHelper(char chOpType);
							///< \param[in] chOpType Тип операции.
	/// Переключение переводов.
	static void ChangeLang(char chLang);
							///< \param[in] chLang Номер языка.
public:
	static unsigned char uchInitRes; ///< Результат инициализации.
	static QSettings* p_QSettings; ///< Указатель на установки.
	static bool bSudoI; ///< Внутренний признак требования запуска запросов с sudo (для отладки).

private:
	LOGDECL
	LOGDECL_PTHRD_INCLASS_ADD
	static MainWindow* p_MainWindow; ///< Указатель на себя для статических функций.
	static Ui::MainWindow* p_ui; ///< Указатель на UI.
	static QList<QSplitter*> lstpQSplitters; ///< Список с указателями на все разделители.
	static QList<QRadioButton*> lstpQRadioButtons; ///< Список с указателями на все переключатели.
	static QList<QLineEdit*> lstpQLineEdits; ///< Список с указателями на все строчные редакторы.
	static QFileDialog* p_QFileDialog; ///< Указатель на диалог выбора файлов и папок.
	static bool bFolderOk; ///< Признак корректного каталога.
	static bool bSingleOk; ///< Признак корректного файла.
	static bool bImageOk; ///< Признак корректного образа.
	static bool bMounted; ///< Признак смонтированного образа.
	static bool bSendEnable; ///< Признак разрешённой отсылки.
	static bool bOpenEnable; ///< Признак разрешённого монтирования.
	static bool bCloseEnable ; ///< Признак разрешённого размонтирования.
	static QTimer oQTimerButtons; ///< Таймер для установки статусов кнопок без мерцания.
	static bool bQTimerButtonsIsRunning; ///< Признак работающего таймера.
	static QTimer oQTimerFileOperations; ///< Таймер для файловых операций и разблокирования главного окна.
	static char chFileOperationType; ///< Тип файловой операции в таймере файловых операций.
	static DialogAbout* p_DialogAbout; ///< Указатель на диалог сведений о программе.
	static DialogError* p_DialogError; ///< Указатель на диалог сведений об ошибке.
	static DialogSettings* p_DialogSettings; ///< Указатель на диалог настроек приложения.
	static DialogManual* p_DialogManual; ///< Указатель на диалог описания.
	static QString strExplPathI; ///< Строка с путём к предпочитаемому менеджеру файлов.
	static bool bAutoCloseEnabled; ///< Признак разрешения автоматического закрытия образа при выходе.
	static QString strSudo; ///< Строка с текстом из m_chSudo или пустая, в зависимости от настроек.
	static bool bOnExitProccess; ///< Признак закрывающегося приложения.
	static QLabel* p_QLabelStatus; ///< Указатель на текст статуса.
	static QStringList oQStringListCurrentDevState; ///< Писок строк из отчёта kpartx по текущему доступному устройству.
	static QTranslator* p_QTranslatorI; ///< Внутренний указатель на переводчик.
	static QApplication* p_QApplicationI; ///< Внутренний указатель на приложения.
	static std::string sLogMsgCollector; ///< Строка-коллектор сообщения для отправки выбранный тип логирования.
};

#endif // MAINWINDOW_H
