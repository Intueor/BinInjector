//== ВКЛЮЧЕНИЯ.
#include <QApplication>
#include <QTranslator>
#include <QTextStream>
#include "main-window.h"

//== ФУНКЦИИ.
// Точка входа в приложение.
int main(int argc, char* argv[])
{
	int iExecResult;
	bool bSudo = false;
	bool bR = false;
	QStringList oQStringList;
	MainWindow::ShellRet oShellRet;
	QString strHelper;
	QApplication oApplication(argc, argv);
	QTranslator oQTranslator;
	std::string sCmd = CMD_REPLY;
	MainWindow* p_MainWindow;
	QDir oQDir(TMP_PATH);
	QFile oQFile(SHELL_OUTPUT_TMP_FILE_PATH);
	QString strExplPath = DEFAULT_EXPLORER_PATH;
	bool bSambaFixNeeded = false;
	//
	if(!oQTranslator.load("bininjector_en_EN", ":/translations")) return RETVAL_ERR;
	if(!oQDir.exists())
	{
		oQDir.mkdir(TMP_PATH);
	}
	if(!oQFile.exists())
	{
		oQFile.open(QIODevice::WriteOnly | QIODevice::Text);
		oQFile.close();
	}
	oQFile.setPermissions(CARTE_BLANCHE);
	//
	oQFile.setFileName(STARTING_SCRIPT_PATH);
	if(!oQFile.exists())
	{
		std::cout << sCmd << "Error: file /usr/bin/bininjector is absent.\n";
		return RETVAL_ERR;
	}
	else
	{
		QString strScript;
		//
		strHelper = "sudo chmod 777 " + QString(STARTING_SCRIPT_PATH);
		MainWindow::CallShell(strHelper, DONT_USE_LOG, DONT_SEND_OUTPUT);
		MSleep(500);
		oQFile.open(QIODevice::ReadOnly | QIODevice::Text);
		strScript = strScript.fromStdString(oQFile.readAll().toStdString());
		oQFile.close();
		if(strScript.contains("pkexec")) bSudo = false; else bSudo = true;
	}
	// При наличии аргументов.
	if(argc > 1)
	{
		int iF = 1;
		QString strArg = argv[iF];
		//
		if(strArg == "-r")
		{
			bSudo = true;
			bR = true;
		}
		else if(strArg == "-C")
		{
			QString strImagePath, strDataPath;
			bool bDataType;
			// Путь к образу.
			iF++;
			if(iF >= argc)
			{
				std::cout << sCmd << "Error: no first argument.\n";
				return RETVAL_ERR;
			}
			strImagePath = argv[iF];
			// Тип данных.
			iF++;
			if(iF >= argc)
			{
				std::cout << sCmd << "Error: no second argument.\n";
				return RETVAL_ERR;
			}
			strArg = argv[iF];
			if(strArg == "S") bDataType = DATA_TYPE_SINGLE_FILE;
			else bDataType = DATA_TYPE_FOLDER;
			// Путь к данным.
			iF++;
			if(iF >= argc)
			{
				std::cout << sCmd << "Error: no third argument.\n";
				return RETVAL_ERR;
			}
			strDataPath = argv[iF];
			MainWindow::bSudoI = false;
			if(MainWindow::ImageOps(SEND_TO_IMAGE_FILE_OPERATION, strImagePath, bDataType, strDataPath, DONT_USE_LOG)) return RETVAL_OK;
			else return RETVAL_ERR;
		}
		else
		{
			std::cout << sCmd << "Error: command unknown.\n";
			return RETVAL_ERR;
		}
	}
	else if(argc > 5)
	{
		std::cout << sCmd << "Error: too many arguments.\n";
	}
	//
	FreedomFor(bSudo, strHelper, TMP_PATH);
	// Добавление пользовательских настроек на удаление с пакетом.
	oQFile.setFileName(CLEANER_SCRIPT_PATH);
	if(oQFile.exists())
	{
		QString strScript;
		//
		FreedomFor(bSudo, strHelper, CLEANER_SCRIPT_PATH);
		oQFile.open(QIODevice::ReadWrite | QIODevice::Text);
		strHelper = QDir::homePath();
		strHelper += "/.local/share/bininjector";
		strScript = oQFile.readAll().toStdString().c_str();
		if(!strScript.contains(strHelper))
		{
			QTextStream oQTextStream(&oQFile);
			oQTextStream << "rm -R -f " << strHelper.toStdString().c_str() << "\n";
		}
		oQFile.close();
	}
	else
	{
		std::cout << sCmd << "Error: file /usr/bin/bininjector_cleaner is absent.\n";
		return RETVAL_ERR;
	}
	// Поиск файлового менеджера.
	strHelper = "xdg-mime query default inode/directory";
	oShellRet = MainWindow::CallShell(strHelper, DONT_USE_LOG, SEND_OUTPUT);
	oQStringList = oShellRet.strOut.split(".");
	int iCount = oQStringList.count();
	if(iCount > 1)
	{
		strHelper = oQStringList.at(iCount - 2);
		strExplPath = strHelper.toLower();
	}
	if((strExplPath == NEMO_COMMAND) || (strExplPath == NAUTILUS_COMMAND)) bSambaFixNeeded = true;
	if(strExplPath.endsWith("su")) strExplPath = strExplPath.left(strExplPath.count() - 2);
	// Фикс остутствия папки для Самбы.
	if(bSambaFixNeeded)
	{
		oQDir.setPath(SAMBA_FIX_PATH);
		if(!oQDir.exists())
		{
			if(bSudo) strHelper = "sudo "; else strHelper = "";
			strHelper += "mkdir ";
			strHelper += SAMBA_FIX_PATH;
			MainWindow::CallShell(strHelper, DONT_USE_LOG, DONT_SEND_OUTPUT);
		}
		oQDir.setPath(SAMBA_FIX_FOLDER);
		if(!oQDir.exists())
		{
			if(bSudo) strHelper = "sudo "; else strHelper = "";
			strHelper += "mkdir ";
			strHelper += SAMBA_FIX_FOLDER;
			MainWindow::CallShell(strHelper, DONT_USE_LOG, DONT_SEND_OUTPUT);
		}
	}
	//
	p_MainWindow = new MainWindow(strExplPath, bSudo, bR, &oQTranslator, &oApplication);
	//
	if(p_MainWindow->uchInitRes == RETVAL_OK)
	{
		setlocale(LC_NUMERIC, "en_US.UTF-8");
		p_MainWindow->show();
		iExecResult = oApplication.exec();
	}
	else iExecResult = RETVAL_ERR;
	delete p_MainWindow;
	return iExecResult;
}
