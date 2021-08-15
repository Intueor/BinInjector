//== ВКЛЮЧЕНИЯ.
#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include "main-window.h"
#include "ui_main-window.h"
#include "ui_dialog-settings.h"

//== МАКРОСЫ.
#define LOG_NAME										"main-window"
#define LOG_DIR_PATH									"/tmp/bininjector/logs/"
//
#define ELP(text)										sLogMsgCollector += text;
#define EXT_LOG_MESSAGING(level, type, prefix, elps)	sLogMsgCollector.clear();										\
														elps															\
														if(bLogging)													\
														{																\
															level(type, sLogMsgCollector);								\
														}																\
														else std::cout << sCmd << prefix << sLogMsgCollector << "\n";

//== ДЕКЛАРАЦИИ СТАТИЧЕСКИХ ПЕРЕМЕННЫХ.
// Основное.
LOGDECL_INIT_INCLASS(MainWindow)
LOGDECL_INIT_PTHRD_INCLASS_OWN_ADD(MainWindow)
MainWindow* MainWindow::p_MainWindow = nullptr;
unsigned char MainWindow::uchInitRes = RETVAL_OK;
Ui::MainWindow* MainWindow::p_ui = new Ui::MainWindow;
QSettings* MainWindow::p_QSettings = nullptr;
QList<QSplitter*> MainWindow::lstpQSplitters;
QList<QRadioButton*> MainWindow::lstpQRadioButtons;
QList<QLineEdit*> MainWindow::lstpQLineEdits;
QFileDialog* MainWindow::p_QFileDialog;
bool MainWindow::bFolderOk = false;
bool MainWindow::bSingleOk = false;
bool MainWindow::bImageOk = false;
bool MainWindow::bMounted = false;
bool MainWindow::bSendEnable = false;
bool MainWindow::bOpenEnable = false;
bool MainWindow::bCloseEnable = false;
QTimer MainWindow::oQTimerButtons;
bool MainWindow::bQTimerButtonsIsRunning = false;
QTimer MainWindow::oQTimerFileOperations;
char MainWindow::chFileOperationType = SEND_TO_IMAGE_FILE_OPERATION;
DialogAbout* MainWindow::p_DialogAbout = nullptr;
DialogError* MainWindow::p_DialogError = nullptr;
DialogSettings* MainWindow::p_DialogSettings = nullptr;
DialogManual* MainWindow::p_DialogManual = nullptr;
QString MainWindow::strExplPathI;
bool MainWindow::bAutoCloseEnabled = true;
bool MainWindow::bOnExitProccess = false;
QLabel* MainWindow::p_QLabelStatus = nullptr;
bool MainWindow::bSudoI = false;
QStringList MainWindow::oQStringListCurrentDevState;
QTranslator* MainWindow::p_QTranslatorI;
QApplication* MainWindow::p_QApplicationI;
std::string MainWindow::sLogMsgCollector;

//== ФУНКЦИИ КЛАССОВ.
//== Класс главного окна.
// Конструктор.
MainWindow::MainWindow(QString strExplPath, bool bSudo, bool bR, QTranslator* p_QTranslator, QApplication* p_QApplication, QWidget* p_parent) :
	QMainWindow(p_parent)
{
	QString strSettingsName;
	QString strHelper;
	QDir oQDir(LOG_DIR_PATH);
	QFile oQFile(LOG_PATH);
	//
	strExplPathI = strExplPath;
	bSudoI = bSudo;
	p_QTranslatorI = p_QTranslator;
	p_QApplicationI = p_QApplication;
	// Подготовка к логированию из-за возможного захода от рута и наоборот.
	if(!oQDir.exists())
	{
		if(bSudoI) strHelper = "sudo "; else strHelper = "";
		strHelper += "mkdir ";
		strHelper += LOG_DIR_PATH;
		CallShell(strHelper, DONT_USE_LOG, DONT_SEND_OUTPUT);
		FreedomFor(bSudo, strHelper, LOG_DIR_PATH);
	}
	if(oQFile.exists())
	{
		if(bSudoI) strHelper = "sudo "; else strHelper = "";
		strHelper += "rm ";
		strHelper += LOG_PATH;
		CallShell(strHelper, DONT_USE_LOG, DONT_SEND_OUTPUT);
	}
	// Инициализация логирования.
	LOG_CTRL_INIT;
	LOG_P_0(LOG_CAT_I, m_chLogStart);
	FreedomFor(bSudo, strHelper, LOG_PATH);
	//
	strHelper = QDir::homePath() + USER_DATA_PARTIAL_PATH;
	oQDir.setPath(strHelper);
	if(!oQDir.exists())
	{
		oQDir.mkdir(strHelper);
	}
	//
	p_MainWindow = this;
	p_ui->setupUi(this);
	p_QLabelStatus = new QLabel;
	p_QLabelStatus->setText("Запуск...");
	p_ui->statusbar->addPermanentWidget(p_QLabelStatus);
	p_ui->menubar->installEventFilter(this);
	p_DialogAbout = new DialogAbout;
	p_DialogError = new DialogError;
	p_DialogSettings = new DialogSettings;
	p_DialogManual = new DialogManual;
	strSettingsName = strHelper; strSettingsName += MAINWINDOW_SETTINGS_FILENAME;
	p_QSettings = new QSettings(strSettingsName, QSettings::IniFormat);
	p_QFileDialog = new QFileDialog(this);
	lstpQSplitters = p_ui->centralwidget->findChildren<QSplitter*>();
	lstpQRadioButtons = p_ui->centralwidget->findChildren<QRadioButton*>();
	lstpQLineEdits = p_ui->centralwidget->findChildren<QLineEdit*>();
	p_QLabelStatus->setText(tr("Получение настроек..."));
	oQFile.setFileName(strSettingsName);
	if(oQFile.exists())
	{
		QVariant oQVariant;
		// Главное окно.
		if(!restoreGeometry(p_QSettings->value(m_chGeometry).toByteArray()))
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoGeometry);
			RETVAL_SET(RETVAL_ERR);
		}
		if(!restoreState(p_QSettings->value(m_chState).toByteArray()))
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoState);
			RETVAL_SET(RETVAL_ERR);
		}
		// Диалог об ошибке.
		if(!p_DialogError->restoreGeometry(p_QSettings->value(m_chGeometryDE).toByteArray()))
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoGeometryDE);
			RETVAL_SET(RETVAL_ERR);
		}
		// Разделители.
		for(int iF = 0; iF != lstpQSplitters.count(); iF++)
		{
			QSplitter* p_QSplitter = lstpQSplitters.at(iF);
			QString strName = p_QSplitter->objectName();
			//
			if(!p_QSplitter->restoreGeometry(p_QSettings->value(m_chSplitterGeometry + strName).toByteArray()))
			{
				LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoSplGeometry << strName.toStdString().c_str() << m_chLogEnding);
				RETVAL_SET(RETVAL_ERR);
			}
			if(!p_QSplitter->restoreState(p_QSettings->value(m_chSplitterState + strName).toByteArray()))
			{
				LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoSplState << strName.toStdString().c_str() << m_chLogEnding);
				RETVAL_SET(RETVAL_ERR);
			}
		}
		// Переключатели.
		for(int iF = 0; iF != lstpQRadioButtons.count(); iF++)
		{
			QRadioButton* p_QRadioButton = lstpQRadioButtons.at(iF);
			QString strName = p_QRadioButton->objectName();
			//
			oQVariant = p_QSettings->value(m_chRadioState + strName);
			if(!oQVariant.isNull()) p_QRadioButton->setChecked(oQVariant.toBool());
			else
			{
				LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoCWState << strName.toStdString().c_str() << m_chLogEnding);
				RETVAL_SET(RETVAL_ERR);
			}
		}
		// Строчные редакторы.
		for(int iF = 0; iF != lstpQLineEdits.count(); iF++)
		{
			QLineEdit* p_QLineEdit = lstpQLineEdits.at(iF);
			QString strName = p_QLineEdit->objectName();
			//
			oQVariant = p_QSettings->value(m_chLineEdit + strName);
			if(!oQVariant.isNull()) p_QLineEdit->setText(oQVariant.toString());
			else
			{
				LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoCWState << strName.toStdString().c_str() << m_chLogEnding);
				RETVAL_SET(RETVAL_ERR);
			}
		}
		// Настройки.
		oQVariant = p_QSettings->value(m_chExplorer); // Файловый менеджер.
		if(oQVariant.isNull())
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoSettingsState << m_chExplorer << m_chLogEnding);
			RETVAL_SET(RETVAL_ERR);
		}
		else strExplPathI = oQVariant.toString();
		oQVariant = p_QSettings->value(m_chAutoClose); // Авто-закрытие на выходе.
		if(oQVariant.isNull())
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoSettingsState << m_chAutoClose << m_chLogEnding);
			RETVAL_SET(RETVAL_ERR);
		}
		else bAutoCloseEnabled = oQVariant.toBool();
		oQVariant = p_QSettings->value(m_chLanguage); // Язык.
		if(oQVariant.isNull())
		{
			LOG_P_1(LOG_CAT_E, m_chLogCantRestore << m_chLogNoSettingsState << m_chLanguage << m_chLogEnding);
			RETVAL_SET(RETVAL_ERR);
		}
		else
		{
			QString strLangName = oQVariant.toString();
			//
			if(strLangName == "EN")
			{
				p_ui->action_English->setChecked(true);
				p_ui->action_Russian->setChecked(false);
				ChangeLang(LANG_EN);
			}
			else if(strLangName == "RU")
			{
				p_ui->action_Russian->setChecked(true);
				p_ui->action_English->setChecked(false);
				ChangeLang(LANG_RU);
			}
		}
		if(RETVAL == RETVAL_ERR)
		{
			LOG_P_0(LOG_CAT_E, m_chLogSettingsLoadedErrors);
			p_ui->statusbar->showMessage(tr("Настройки загружены с ошибками."), OPERATION_STATUS_MESSAGE_DELAY);
		}
		else
		{
			LOG_P_2(LOG_CAT_I, m_chLogSettingsLoaded);
			p_ui->statusbar->showMessage(tr("Настройки загружены."), OPERATION_STATUS_MESSAGE_DELAY);
		}
	}
	else
	{
		LOG_P_0(LOG_CAT_W, m_chLogMainWindowIniAbsent);
		p_ui->statusbar->showMessage(tr("Настройки не загружены, будут созданы новые."), OPERATION_STATUS_MESSAGE_DELAY);
	}
	//
	on_lineEdit_Folder_editingFinished();
	on_lineEdit_Single_editingFinished();
	on_lineEdit_Image_editingFinished();
	p_DialogSettings->p_ui->lineEdit_Explorer->setText(strExplPathI);
	p_DialogSettings->p_ui->checkBox_AutoClose->setChecked(bAutoCloseEnabled);
	p_DialogSettings->p_ui->checkBox_Sudo->setChecked(bSudo);
	if(bR) p_DialogSettings->p_ui->checkBox_Sudo->setEnabled(false);
	p_QLabelStatus->setText(tr("Готов"));
}

// Деструктор.
MainWindow::~MainWindow()
{
	if(RETVAL == RETVAL_OK)
	{
		LOG_P_0(LOG_CAT_I, m_chLogExit);
	}
	LOG_CLOSE;
	p_QLabelStatus->setText(tr("Готов к отключению."));
	delete p_QLabelStatus;
	delete p_ui;
}

// Процедуры при закрытии окна приложения.
void MainWindow::closeEvent(QCloseEvent *p_Event)
{
	bOnExitProccess = true;
	if(bCloseEnable && bAutoCloseEnabled)
	{
		chFileOperationType = CLOSE_IMAGE_FILE_OPERATION;
		ImageOpsTimerStartHelper();
	}
	// Сохранение параметров.
	p_QLabelStatus->setText(tr("Сохранение настроек..."));
	// Настройки.
	if(p_ui->action_English->isChecked()) p_QSettings->setValue(m_chLanguage, "EN");
	else if (p_ui->action_Russian->isChecked()) p_QSettings->setValue(m_chLanguage, "RU");
	p_QSettings->setValue(m_chExplorer, strExplPathI);
	p_QSettings->setValue(m_chAutoClose, bAutoCloseEnabled);
	// Строчные редакторы.
	for(int iF = 0; iF != lstpQLineEdits.count(); iF++)
	{
		QLineEdit* p_QLineEdit = lstpQLineEdits.at(iF);
		//
		p_QSettings->setValue(m_chLineEdit + p_QLineEdit->objectName(), p_QLineEdit->text());
	}
	// Переключатели.
	for(int iF = 0; iF != lstpQRadioButtons.count(); iF++)
	{
		QRadioButton* p_QRadioButton = lstpQRadioButtons.at(iF);
		//
		p_QSettings->setValue(m_chRadioState + p_QRadioButton->objectName(), p_QRadioButton->isChecked());
	}
	// Разделители.
	for(int iF = 0; iF != lstpQSplitters.count(); iF++)
	{
		QSplitter* p_QSplitter = lstpQSplitters.at(iF);
		//
		p_QSettings->setValue(m_chSplitterGeometry + p_QSplitter->objectName(), p_QSplitter->saveGeometry());
		p_QSettings->setValue(m_chSplitterState + p_QSplitter->objectName(), p_QSplitter->saveState());
	}
	// Диалог об ошибке.
	p_QSettings->setValue(m_chGeometryDE, p_DialogError->saveGeometry());
	// Главное окно.
	p_QSettings->setValue(m_chGeometry, saveGeometry());
	p_QSettings->setValue(m_chState, saveState());
	p_QSettings->sync();
	if(p_QSettings->status() != QSettings::NoError)
	{
		LOG_P_0(LOG_CAT_E, m_chLogSettingsError);
	}
	else LOG_P_1(LOG_CAT_I, m_chLogSettingsSaved);
	//
	p_QLabelStatus->setText(tr("Очистка..."));
	delete p_DialogManual;
	delete p_QFileDialog;
	delete p_DialogSettings;
	delete p_DialogError;
	delete p_DialogAbout;
	delete p_QSettings;
	QMainWindow::closeEvent(p_Event);
}

// Перехватчик событий.
bool MainWindow::eventFilter(QObject* p_watched, QEvent* p_event)
{
	if((p_watched == p_ui->menubar) && (p_event->type() == QEvent::StatusTip)) return true;
	return QMainWindow::eventFilter(p_watched, p_event);
}

// Установка состояний кнопок по корректности строк путей.
void MainWindow::SetButtonStatesByPathFlags()
{
	bool bOnFolder = p_ui->radioButton_Folder->isChecked();
	// Установка флагов для кнопки отсылки (не зависит от статуса монтирования).
	if(bOnFolder) bSendEnable = bFolderOk & bImageOk; // Если на папке, при корректности папки и образа - можно отсылать.
	else bSendEnable = bSingleOk & bImageOk; /// Если на файле, при корректности файла и образа - можно отсылать.
	// Установка флагов для кнопок монтирования и размонтирования.
	if(bMounted) // Если смотнировано...
	{
		bOpenEnable = false; // Всегда откл.
		bCloseEnable = true; // Всегда вкл.
	}
	else // Если не смонтировано...
	{
		bCloseEnable = false; // Всегда откл.
		bOpenEnable = bImageOk; // Корректный образ - можно открывать.
	}
	//
	if(!bQTimerButtonsIsRunning) // Предупреждение многократного запуска таймера.
	{
		bQTimerButtonsIsRunning = true;
		oQTimerButtons.singleShot(TIMER_BUTTONS_DELAY, p_MainWindow, SLOT(On_ButtonsDelayTimer()));
	}
}

// Вызов Shell с командной строкой.
MainWindow::ShellRet MainWindow::CallShell(QString& a_strCommand, bool bLogging, bool bOutEnabled)
{
	QString strCommand;
	if(bSudoI) strCommand = m_chSudo;
	strCommand += a_strCommand;
	if(bOutEnabled)
	{
		strCommand += " > "; strCommand += SHELL_OUTPUT_TMP_FILE_PATH;
	}
	FILE* p_File = popen(strCommand.toStdString().c_str(), "r");
	ShellRet oShellRet;
	//
	oShellRet.iRes = -1;
	//
	sync();
	if(p_File)
	{

		oShellRet.iRes = pclose(p_File);
	}
	else
	{
		if(bLogging)
		{
			LOG_P_0(LOG_CAT_E, "Can`t execute [" << a_strCommand.toStdString().c_str() << "].");
		}
		oShellRet.strOut = SHELL_COMMAND_CALL_ERROR;
		return oShellRet;
	}
	if(bOutEnabled)
	{
		std::ifstream oIfstream(SHELL_OUTPUT_TMP_FILE_PATH);
		std::string sContent((std::istreambuf_iterator<char>(oIfstream)), (std::istreambuf_iterator<char>()));
		oIfstream.close();
		oShellRet.strOut = QString::fromStdString(sContent);
	}
	return oShellRet;
}

// Выполнение диалога об ошибке с логом.
void MainWindow::ExecErrorDialogWithLog()
{
	std::ifstream oIfstream(LOG_PATH);
	std::string sContent((std::istreambuf_iterator<char>(oIfstream)), (std::istreambuf_iterator<char>()));
	oIfstream.close();
	QString strLog = QString::fromStdString(sContent);
	//
	p_DialogError->SetLogText(strLog);
	p_DialogError->exec();
}

// Вспомогательная функция логирования ошибки операций с образом.
void MainWindow::LogImgOpsHelper(char chOpType)
{
	if(chOpType == SEND_TO_IMAGE_FILE_OPERATION)
	{
		LOG_P_0(LOG_CAT_E, "Copying failed.");
	}
	else
	{
		if(chOpType == CLOSE_IMAGE_FILE_OPERATION)
		{
			LOG_P_0(LOG_CAT_E, "Can`t close image.");
		}
		else
		{
			LOG_P_0(LOG_CAT_E, "Can`t open image.");
		}
	}
}

// Переключение переводов.
void MainWindow::ChangeLang(char chLang)
{
	switch(chLang)
	{
		case LANG_RU:
		{
			p_QApplicationI->removeTranslator(p_QTranslatorI);
			break;
		}
		case LANG_EN:
		{
			p_QApplicationI->installTranslator(p_QTranslatorI);
			break;
		}
		default:
		{
			LOG_P_0(LOG_CAT_E, m_chLogInternalError);
			return;
		}
	}
	p_ui->retranslateUi(p_MainWindow);
	p_DialogAbout->Retranslate();
	p_DialogError->Retranslate();
	p_DialogSettings->Retranslate();
	p_DialogManual->Retranslate();
	p_QLabelStatus->setText(tr("Готов"));
}

// Операции с образом.
bool MainWindow::ImageOps(char chOpType, QString strImagePath, bool bDataType, QString strDataPath, bool bLogging)
{
	bool bSuccess = true;
	std::string sCmd = CMD_REPLY;
	QString strCommand;
	QDir oQDir(MOUNT_POINT_PATH);
	ShellRet oShellRet;
	QFile oQFile;
	//
	EXT_LOG_MESSAGING(LOG_P_2, LOG_CAT_I, "", ELP("Starting image operations."));
	if(chOpType == CLOSE_IMAGE_FILE_OPERATION) goto gUm;
	if(chOpType == COPY_TO_IMAGE_FILE_OPERATION) goto gCp;
	// <= Открытие образа. =>
	if(oQDir.exists()) // Расчистка места для монтирования.
	{
		oQDir.removeRecursively();
	}
	if(!oQDir.mkdir(MOUNT_POINT_PATH))
	{
		EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP("Can`t create mount point at [") ELP(MOUNT_POINT_PATH) ELP(m_chLogEnding));
		return false;
	}
	if(bLogging) LOG_P_2(LOG_CAT_I, "Getting mount info.");
	strCommand = "kpartx -ls \"" + strImagePath + "\"";
	oShellRet = CallShell(strCommand, bLogging);
	if((oShellRet.strOut == SHELL_COMMAND_CALL_ERROR) || oShellRet.iRes) // Если kpartx не заработал...
	{
		EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString())); bSuccess = false;
		goto gQ;
	}
	oQStringListCurrentDevState = oShellRet.strOut.split(" ");
	if(oQStringListCurrentDevState.count() < 5)
	{
		EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP("No info from [kpartx].")); bSuccess = false;
		goto gQ;
	}
	if(bLogging) LOG_P_2(LOG_CAT_I, "Image mounting info ok.");
	MSleep(500);
	strCommand = "kpartx -avs \"" + strImagePath + "\"";
	oShellRet = CallShell(strCommand, bLogging);
	//
	if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если kpartx заработал...
	{
		if((oShellRet.strOut.contains("failed")) || oShellRet.iRes) // Если не смог открыть образ...
		{
			oShellRet.strOut.prepend("kpartx: ");
			EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString())); bSuccess = false;
			if(bLogging) LogImgOpsHelper(chOpType);
		}
		else // Если другие варианты...
		{
			if(oShellRet.strOut.contains("add map")) // Если пока всё ок...
			{
				// <= Монтирование образа. =>
				strCommand = "mount /dev/mapper/"; strCommand += oQStringListCurrentDevState.at(0) + " "; strCommand += MOUNT_POINT_PATH;
				oShellRet = CallShell(strCommand, bLogging);
				if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если mount заработал...
				{
					if(!oShellRet.iRes) // Если пока всё ок...
					{
						EXT_LOG_MESSAGING(LOG_P_1, LOG_CAT_I, "", ELP("Opening and mounting - ok."));
						if(chOpType == OPEN_IMAGE_FILE_OPERATION) return  bSuccess;
						// <= Копирование выбранного. =>
gCp:					if(bDataType == DATA_TYPE_FOLDER) // Если выбрана папка...
						{
							strCommand = "cp -f -R \"" + strDataPath + "\"/. "; strCommand += MOUNT_POINT_PATH;
						}
						else // Если выбран файл...
						{
							strCommand = "cp -f \"" + strDataPath + "\" "; strCommand += MOUNT_POINT_PATH;
						}
						oShellRet = CallShell(strCommand, bLogging);
						if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если cp заработал...
						{
							if(!oShellRet.iRes) // Если пока всё ок...
							{
								EXT_LOG_MESSAGING(LOG_P_1, LOG_CAT_I, "", ELP("Copying successful."));
								if(chOpType == COPY_TO_IMAGE_FILE_OPERATION) return  bSuccess;
								// <= Размонтирование образа. =>
gUm:							strCommand = "umount /dev/mapper/"; strCommand += oQStringListCurrentDevState.at(0);
								oShellRet = CallShell(strCommand, bLogging);
								if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если umount заработал...
								{
									if(!oShellRet.iRes) // Если пока всё ок...
									{
										// <= Закрытие образа. =>
gCl:									strCommand = "kpartx -dvs \"" + strImagePath + "\"";
										oShellRet = CallShell(strCommand, bLogging);
										if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если kpartx заработал...
										{
											if(!oShellRet.iRes) // Если пока всё ок.
											{
												strCommand = "/dev/mapper/";
												oQFile.setFileName(strCommand + oQStringListCurrentDevState.at(0));
												if(!oQFile.exists()) goto gKm; // Если kpartx сам отключил (на Ubuntu)...
												// <= Отключение петли. =>
gKl:											strCommand = "losetup -d "; strCommand += oQStringListCurrentDevState.at(4);
												oShellRet = CallShell(strCommand, bLogging);
												if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если losetup заработал...
												{
													if(!oShellRet.iRes) // Если пока всё ок.
													{
														// <= Отключение мэппера. =>
														strCommand = "dmsetup remove " + oQStringListCurrentDevState.at(0);
														oShellRet = CallShell(strCommand, bLogging);
														if(oShellRet.strOut != SHELL_COMMAND_CALL_ERROR) // Если dmsetup заработал...
														{
															if(!oShellRet.iRes) // Если до сих пор всё ок...
															{
gKm:															if(bSuccess)
																{
																	EXT_LOG_MESSAGING(LOG_P_1, LOG_CAT_I, "",
																					  ELP("Unmounting and closing - ok."));
																}
																else bSuccess = false;
															}
															else // Если не смог снять мэппинг... то всё...
															{
																EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError,
																				  ELP(oShellRet.strOut.toStdString()));
																bSuccess = false;
															}
														}
														else bSuccess = false;
													}
													else // Если не смог снять петлю...
													{
														EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError,
																		  ELP(oShellRet.strOut.toStdString()));
														bSuccess = false;
														goto gKm; // Попытка снять мэппинг.
													}
												}
												else bSuccess = false;
											}
											else // Если не смог закрыть образ...
											{
												oShellRet.strOut.prepend("kpartx: "); // Т.к. он себя не называет.
												EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString()));
												bSuccess = false;
												goto gKl; // Попытка снять петлю.
											}
										}
										else bSuccess = false;
									}
									else // Если не смог размонтировать образ...
									{
										EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString()));
										bSuccess = false;;
										goto gCl; // Попытка закрыть образ.
									}
								}
								else bSuccess = false;
							}
							else // Если не смог скопировать в образ...
							{
								EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString())); bSuccess = false;
								goto gUm; // Попытка размонтировать и закрыть образ.
							}
						}
						else bSuccess = false;
					}
					else // Если не смог примонтировать образ...
					{
						EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString()));
						bSuccess = false;
						goto gCl; // Попытка закрыть образ.
					}
				}
				else bSuccess = false;
			}
			else // Если другие причины сбоя открытия образа...
			{
				if(oShellRet.iRes)
				{
					oShellRet.strOut.prepend("kpartx: "); // Т.к. он себя не называет.
					EXT_LOG_MESSAGING(LOG_P_0, LOG_CAT_E, m_chExtLogError, ELP(oShellRet.strOut.toStdString()));
				}
				bSuccess = false;
				goto gKl; // Попытка снять петлю и мэппинг.
			}
		}
	}
	else bSuccess = false;
gQ:	if(!bSuccess && bLogging) LogImgOpsHelper(chOpType);
	return bSuccess;
}

// Вспомогательная функция для старта таймера операций с образом.
void MainWindow::ImageOpsTimerStartHelper()
{
	p_MainWindow->setEnabled(false);
	oQTimerFileOperations.singleShot(TIMER_ACTIONS_DELAY, p_MainWindow, SLOT(On_FileOperationsTimer()));
}

// Вспомогательная функция блокировки\разблокировки части UI на время операций с образом.
void MainWindow::ImageOpsUIBlockerHelper(bool bEnabled)
{
	p_ui->lineEdit_Folder->setEnabled(bEnabled);
	p_ui->lineEdit_Image->setEnabled(bEnabled);
	p_ui->lineEdit_Single->setEnabled(bEnabled);
	p_ui->pushButton_Sel_Folder->setEnabled(bEnabled);
	p_ui->pushButton_Sel_Image->setEnabled(bEnabled);
	p_ui->pushButton_Sel_Single->setEnabled(bEnabled);
	p_ui->radioButton_Folder->setEnabled(bEnabled);
	p_ui->radioButton_Single->setEnabled(bEnabled);
}

// При срабатывании таймера задержки для клавиш.
void MainWindow::On_ButtonsDelayTimer()
{
	p_ui->pushButton_Send->setEnabled(bSendEnable);
	p_ui->pushButton_Open->setEnabled(bOpenEnable);
	p_ui->pushButton_Close->setEnabled(bCloseEnable);
	bQTimerButtonsIsRunning = false;
}

// При срабатывании таймера файловых операций.
void MainWindow::On_FileOperationsTimer()
{
	switch(chFileOperationType)
	{
		// Отправка в образ.
		case SEND_TO_IMAGE_FILE_OPERATION:
		{
			bool bDataType = p_ui->radioButton_Folder->isChecked();
			QString strDataPath;
			char chOpType;
			//
			if(bDataType) strDataPath = p_ui->lineEdit_Folder->text();
			else strDataPath = p_ui->lineEdit_Single->text();
			if(bCloseEnable) chOpType = COPY_TO_IMAGE_FILE_OPERATION;
			else chOpType = SEND_TO_IMAGE_FILE_OPERATION;
			if(!ImageOps(chOpType, p_ui->lineEdit_Image->text(), bDataType, strDataPath))
			{
				ExecErrorDialogWithLog();
			}
			p_ui->statusbar->showMessage(tr("Запись в образ - успешно."), OPERATION_STATUS_MESSAGE_DELAY);
			break;
		}
		case OPEN_IMAGE_FILE_OPERATION:
		{
			if(ImageOps(OPEN_IMAGE_FILE_OPERATION, p_ui->lineEdit_Image->text()))
			{
				QString strCommand;
				ShellRet oShellRet;
				//
				ImageOpsUIBlockerHelper(false);
				bOpenEnable = false;
				bCloseEnable = true;
				p_ui->pushButton_Open->setEnabled(false);
				p_ui->pushButton_Close->setEnabled(true);
				if(bSudoI)
				{
					strCommand = "env XDG_RUNTIME_DIR=/tmp/runtime-root "; strCommand += strExplPathI;
				}
				else strCommand = strExplPathI;
				strCommand += " "; strCommand += MOUNT_POINT_PATH; strCommand += " &";
				oShellRet = CallShell(strCommand, USE_LOG, DONT_SEND_OUTPUT);
				if(!oShellRet.strOut.isEmpty())
				{
					LOG_P_0(LOG_CAT_E, "Can`t open desired file explorer.");
					ExecErrorDialogWithLog();
					p_ui->statusbar->showMessage(tr("Открытие образа - успешно."), OPERATION_STATUS_MESSAGE_DELAY);
					break;
				}
				p_ui->statusbar->showMessage(tr("Открытие образа и файлового менеджера - успешно."), OPERATION_STATUS_MESSAGE_DELAY);
			}
			else
			{
				ExecErrorDialogWithLog();
			}
			break;
		}
		case CLOSE_IMAGE_FILE_OPERATION:
		{
			if(ImageOps(CLOSE_IMAGE_FILE_OPERATION, p_ui->lineEdit_Image->text()))
			{
				p_ui->pushButton_Open->setEnabled(true);
				p_ui->pushButton_Close->setEnabled(false);
				bOpenEnable = true;
				bCloseEnable = false;
				ImageOpsUIBlockerHelper(true);
			}
			else
			{
				if(!bOnExitProccess) ExecErrorDialogWithLog();
			}
			p_ui->statusbar->showMessage(tr("Закрытие образа - успешно."), OPERATION_STATUS_MESSAGE_DELAY);
			break;
		}
		default:
		{
			LOG_P_0(LOG_CAT_E, m_chLogInternalError);
		}
	}
	p_MainWindow->setEnabled(true);
	p_QLabelStatus->setText(tr("Готов"));
}

// При нажатии на переключатель 'Папка'.
void MainWindow::on_radioButton_Folder_clicked(bool bChecked)
{
	if(!bChecked) p_ui->radioButton_Folder->setChecked(true);
	p_ui->radioButton_Single->setChecked(false);
	SetButtonStatesByPathFlags();
}

// При нажатии на переключатель 'Файл'.
void MainWindow::on_radioButton_Single_clicked(bool bChecked)
{
	if(!bChecked) p_ui->radioButton_Single->setChecked(true);
	p_ui->radioButton_Folder->setChecked(false);
	SetButtonStatesByPathFlags();
}

// При нажатии на кнопку поиска и установки 'Папки'.
void MainWindow::on_pushButton_Sel_Folder_clicked()
{
	p_QFileDialog->setOption(QFileDialog::ShowDirsOnly, true);
	p_QFileDialog->setFileMode(QFileDialog::Directory);
	if(bFolderOk) p_QFileDialog->setDirectory(p_ui->lineEdit_Folder->text());
	if (p_QFileDialog->exec())
	{
		p_ui->lineEdit_Folder->setText(p_QFileDialog->selectedFiles().at(0));
		p_ui->lineEdit_Folder->setStyleSheet("");
		bFolderOk = true;
		SetButtonStatesByPathFlags();
	}
}

// При нажатии на кнопку поиска и установки 'Файла'.
void MainWindow::on_pushButton_Sel_Single_clicked()
{
	p_QFileDialog->setOption(QFileDialog::ShowDirsOnly, false);
	p_QFileDialog->setFileMode(QFileDialog::AnyFile);
	p_QFileDialog->setNameFilter(tr("Все файлы (*)"));
	if(bSingleOk) p_QFileDialog->setDirectory(p_ui->lineEdit_Single->text());
	if (p_QFileDialog->exec())
	{
		p_ui->lineEdit_Single->setText(p_QFileDialog->selectedFiles().at(0));
		p_ui->lineEdit_Single->setStyleSheet("");
		bSingleOk = true;
		SetButtonStatesByPathFlags();
	}
}

// При нажатии на кнопку поиска и установки 'Образа'.
void MainWindow::on_pushButton_Sel_Image_clicked()
{
	p_QFileDialog->setOption(QFileDialog::ShowDirsOnly, false);
	p_QFileDialog->setFileMode(QFileDialog::AnyFile);
	p_QFileDialog->setNameFilter(tr("Образы дисков (*.bin)"));
	if(bImageOk) p_QFileDialog->setDirectory(p_ui->lineEdit_Image->text());
	if (p_QFileDialog->exec())
	{
		p_ui->lineEdit_Image->setText(p_QFileDialog->selectedFiles().at(0));
		p_ui->lineEdit_Image->setStyleSheet("");
		bImageOk = true;
		SetButtonStatesByPathFlags();
	}
}

// При нажатии на кнопку отправки выбранного в 'Образ'.
void MainWindow::on_pushButton_Send_clicked()
{
	On_ButtonsDelayTimer();
	if(bSendEnable)
	{
		p_QLabelStatus->setText(tr("Запись выбранного в образ..."));
		chFileOperationType = SEND_TO_IMAGE_FILE_OPERATION;
		ImageOpsTimerStartHelper();
	}
}

// При нажатии на кнопку монтирования и открытия 'Образа'.
void MainWindow::on_pushButton_Open_clicked()
{
	On_ButtonsDelayTimer();
	if(bOpenEnable)
	{
		p_QLabelStatus->setText(tr("Открытие образа и вызов файлового менеджера..."));
		chFileOperationType = OPEN_IMAGE_FILE_OPERATION;
		ImageOpsTimerStartHelper();
	}
}

// При нажатии на кнопку размотнирования 'Образа'.
void MainWindow::on_pushButton_Close_clicked()
{
	On_ButtonsDelayTimer();
	if(bCloseEnable)
	{
		p_QLabelStatus->setText(tr("Закрытие образа..."));
		chFileOperationType = CLOSE_IMAGE_FILE_OPERATION;
		ImageOpsTimerStartHelper();
	}
}

// При нажатии Enter в строке 'Папка'.
void MainWindow::on_lineEdit_Folder_returnPressed()
{
	p_ui->centralwidget->setFocus();
}

// При нажатии Enter в строке 'Файл'.
void MainWindow::on_lineEdit_Single_returnPressed()
{
	p_ui->centralwidget->setFocus();
}

// При нажатии Enter в строке 'Образ'.
void MainWindow::on_lineEdit_Image_returnPressed()
{
	p_ui->centralwidget->setFocus();
}

// При выходе из строки 'Папка'.
void MainWindow::on_lineEdit_Folder_editingFinished()
{
	QString strText = p_ui->lineEdit_Folder->text();
	//
	if(strText.isEmpty())
	{
		p_ui->lineEdit_Folder->setStyleSheet("");
		bFolderOk = false;
	}
	else
	{
		QDir oQDir(strText);
		if(oQDir.exists())
		{
			p_ui->lineEdit_Folder->setStyleSheet("");
			bFolderOk = true;
		}
		else
		{
			p_ui->lineEdit_Folder->setStyleSheet(m_chErrLineEditStyle);
			bFolderOk = false;
		}
	}
	SetButtonStatesByPathFlags();
}

// При выходе из строки 'Файл'.
void MainWindow::on_lineEdit_Single_editingFinished()
{
	QString strText = p_ui->lineEdit_Single->text();
	//
	if(strText.isEmpty())
	{
		p_ui->lineEdit_Single->setStyleSheet("");
		bSingleOk = false;
	}
	else
	{
		QFile oQFile(strText);
		if(oQFile.exists())
		{
			p_ui->lineEdit_Single->setStyleSheet("");
			bSingleOk = true;
		}
		else
		{
			p_ui->lineEdit_Single->setStyleSheet(m_chErrLineEditStyle);
			bSingleOk = false;
		}
	}
	SetButtonStatesByPathFlags();
}

// При выходе из строки 'Образ'.
void MainWindow::on_lineEdit_Image_editingFinished()
{
	QString strText = p_ui->lineEdit_Image->text();
	//
	if(strText.isEmpty())
	{
		p_ui->lineEdit_Image->setStyleSheet("");
		bImageOk = false;
	}
	else
	{
		QFile oQFile(strText);
		if(oQFile.exists())
		{
			int iDot = strText.lastIndexOf('.');
			QString strExtension = strText.mid(iDot);
			//
			if(strExtension.compare(".bin", Qt::CaseInsensitive)) goto gWE;
			p_ui->lineEdit_Image->setStyleSheet("");
			bImageOk = true;
		}
		else
		{
gWE:		p_ui->lineEdit_Image->setStyleSheet(m_chErrLineEditStyle);
			bImageOk = false;
		}
	}
	SetButtonStatesByPathFlags();
}

// При выборе пункта меню 'О программе'.
void MainWindow::on_action_About_triggered()
{
	p_DialogAbout->exec();
}

// При выборе пункта меню 'Настройки'.
void MainWindow::on_action_Settings_triggered()
{
	if(p_DialogSettings->exec() == QDialog::Accepted)
	{
		p_QLabelStatus->setText(tr("Сохранение настроек..."));
		if(p_DialogSettings->SaveSettings(p_QSettings))
		{
			LOG_P_1(LOG_CAT_I, m_chLogSettingsSaved);
			p_ui->statusbar->showMessage(tr("Настройки сохранены."), OPERATION_STATUS_MESSAGE_DELAY);
		}
		else
		{
			LOG_P_0(LOG_CAT_E, m_chLogSettingsError);
			ExecErrorDialogWithLog();
		}
		strExplPathI = p_DialogSettings->p_ui->lineEdit_Explorer->text();
		bAutoCloseEnabled = p_DialogSettings->p_ui->checkBox_AutoClose->isChecked();
		//
		if(p_DialogSettings->p_ui->checkBox_Sudo->isEnabled())
		{
			bool bNewSudo = p_DialogSettings->p_ui->checkBox_Sudo->isChecked();
			if(bNewSudo != bSudoI)
			{
				bSudoI = bNewSudo;
				QFile oQFileStarting(STARTING_SCRIPT_PATH);
				QString strScript;
				QString strPKExec = "pkexec env XDG_RUNTIME_DIR=/tmp/runtime-root ";
				QStringList oQStringList;
				//
				oQFileStarting.open(QIODevice::ReadOnly | QIODevice::Text);
				strScript = strScript.fromStdString(oQFileStarting.readAll().toStdString());
				oQFileStarting.close();
				oQFileStarting.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate);
				if(bSudoI)
				{
					if(strScript.contains(strPKExec))
					{
						oQStringList = strScript.split(strPKExec);
						if(oQStringList.count() == 2)
						{
							strScript = oQStringList.at(0);
							strScript += "env ";
							strScript += oQStringList.at(1);
							QTextStream oQTextStream(&oQFileStarting);
							oQTextStream << strScript;
						}
						else
						{
							LOG_0(LOG_CAT_E, m_chWrongScript);
						}
					}
				}
				else
				{
					if(!strScript.contains(strPKExec))
					{
						oQStringList = strScript.split("exec env ");
						if(oQStringList.count() == 2)
						{
							strScript = oQStringList.at(0) + "exec " + strPKExec + oQStringList.at(1);
							QTextStream oQTextStream(&oQFileStarting);
							oQTextStream << strScript;
						}
						else
						{
							LOG_0(LOG_CAT_E, m_chWrongScript);
						}
					}
				}
				oQFileStarting.close();
			}
		}
		//
		p_QLabelStatus->setText(tr("Готов"));
	}
	else
	{
		p_DialogSettings->p_ui->lineEdit_Explorer->setText(strExplPathI);
		p_DialogSettings->p_ui->checkBox_AutoClose->setChecked(bAutoCloseEnabled);
		p_DialogSettings->p_ui->checkBox_Sudo->setChecked(bSudoI);
	}
}

// При переключении языка чекбоксом 'Русский'.
void MainWindow::on_action_Russian_triggered(bool bChecked)
{
	if(bChecked) ChangeLang(LANG_RU);
	else ChangeLang(LANG_EN);
}

// При переключении языка чекбоксом 'English'.
void MainWindow::on_action_English_triggered(bool bChecked)
{
	if(bChecked) ChangeLang(LANG_EN);
	else ChangeLang(LANG_RU);
}

// При выборе пункта меню 'Описание'.
void MainWindow::on_action_Manual_triggered()
{
	p_DialogManual->exec();
}
