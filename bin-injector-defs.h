#ifndef SJA2USDEFS_H
#define SJA2USDEFS_H

//== МАКРОСЫ.
#define USER_DATA_PARTIAL_PATH					"/.local/share/bininjector/"
#define MAINWINDOW_SETTINGS_FILENAME			"bininjector.ini"
#define TMP_PATH								"/tmp/bininjector/"
#define MOUNT_POINT_PATH						"/tmp/bininjector/mnt/"
#define SHELL_OUTPUT_TMP_FILE_PATH				"/tmp/bininjector/shell_output.tmp"
#define STARTING_SCRIPT_PATH					"/usr/bin/bininjector"
#define CLEANER_SCRIPT_PATH						"/usr/bin/bininjector_cleaner"
#define NEMO_PATH								"/usr/bin/nemo"
#define SAMBA_FIX_PATH							"/var/lib/samba"
#define SAMBA_FIX_FOLDER							"/var/lib/samba/usershares"
#define NEMO_COMMAND							"nemo"
#define FLYFM_PATH								"/usr/bin/fly-fm"
#define FLYFM_COMMAND							"fly-fm"
#define NAUTILUS_PATH							"/usr/bin/nautilus"
#define NAUTILUS_COMMAND						"nautilus"
#define TIMER_BUTTONS_DELAY						100
#define TIMER_ACTIONS_DELAY						1
#define SEND_TO_IMAGE_FILE_OPERATION			0
#define OPEN_IMAGE_FILE_OPERATION				1
#define CLOSE_IMAGE_FILE_OPERATION				2
#define COPY_TO_IMAGE_FILE_OPERATION			3
#define SHELL_COMMAND_CALL_ERROR				"!ERROR!"
#define CMD_REPLY								"BinInjector ==> "
#define DATA_TYPE_SINGLE_FILE					false
#define DATA_TYPE_FOLDER						true
#define USE_LOG									true
#define DONT_USE_LOG							false
#define SEND_OUTPUT								true
#define DONT_SEND_OUTPUT						false
#define DEFAULT_EXPLORER_PATH					"xdg-open"
#define OPERATION_STATUS_MESSAGE_DELAY			3000
#define LANG_RU									0
#define LANG_EN									1
//
#define MSG_READY								"Готов"
//
#define CARTE_BLANCHE	QFile::ReadOwner | QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther | QFile::WriteOwner |	\
	QFile::WriteUser | QFile::WriteGroup | QFile::WriteOther | QFile::ExeOwner | QFile::ExeUser |						\
	QFile::ExeGroup | QFile::ExeOther
#define FreedomFor(sudo, str, path)				if(sudo) str = "sudo "; else str = "";									\
												str += "chmod 777 ";													\
												str += path;															\
												MainWindow::CallShell(str, DONT_USE_LOG, DONT_SEND_OUTPUT)


//== КОНСТАНТЫ.
// Многократно используемые строки.
const char m_chLogStart[] = "START.";
const char m_chLogExit[] = "EXIT.";
const char m_chLogErrorExit[] = "EXIT WITH ERROR: [";
const char m_chLogSettingsLoaded[] = "Settings loaded.";
const char m_chLogSettingsLoadedErrors[] = "Settings loaded with errors.";
const char m_chLogCantRestore[] = "Can`t restore ";
const char m_chLogNoGeometry[] = "application [Geometry] UI state.";
const char m_chLogNoGeometryDE[] = "DialogError [Geometry] UI state.";
const char m_chLogNoState[] = "application [State] UI state.";
const char m_chLogNoSplGeometry[] = "splitter [Geometry] UI state: [";
const char m_chLogNoSplState[] = "splitter [State] UI state: [";
const char m_chLogNoCWState[] = "control widget state: [";
const char m_chLogNoSettingsState[] = "settings state: [";
const char m_chLogMainWindowIniAbsent[] = "[mainwidow_ui.ini] is missing and will be created by default at the exit from program.";
const char m_chGeometry[] = "Geometry";
const char m_chState[] = "State";
const char m_chGeometryDE[] = "GeometryDialogError";
const char m_chExplorer[] = "Explorer";
const char m_chAutoClose[] = "AutoClose";
const char m_chLanguage[] = "Language";
const char m_chSplitterGeometry[] = "SplitterGeometry-";
const char m_chSplitterState[] = "SplitterState-";
const char m_chRadioState[] = "RadioState-";
const char m_chLineEdit[] = "LineEdit-";
const char m_chLogEnding[] = "].";
const char m_chSudo[] = "sudo ";
const char m_chErrLineEditStyle[] = "QLineEdit { background:rgb(150, 60, 60); selection-background-color:rgb(0, 200, 200); }";
const char m_chLogInternalError[] = "INTERNAL ERROR.";
const char m_chLogSettingsError[] = "Can`t save settings.";
const char m_chLogSettingsSaved[] = "Settings saved.";
const char m_chExtLogError[] = "Error: ";
const char m_chWrongScript[] = "Wrong [/usr/bin/bininjector] script content.";

#endif // SJA2USDEFS_H
