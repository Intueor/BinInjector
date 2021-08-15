# BinInjector
[RU]
Программа предназначена для удобства и автоматизации копирования данных в образы дисков (побитовые копии) содержимого папок с сохранением структуры, а так же одиночных файлов.
Поддерживаются три режима работы:
1 - запуск UI с запросом пароля суперпользователя, настройки сохраняются и загружаются из соответствующего каталога;
2 - запуск UI со скрытыми запросами прав суперпользователя из программы (удобно в системах, выполняющих sudo без авторизации, на остальных - может привести к ошибкам при запуске ярлыком и запросу прав от sudo при запуске из командной строки), настройки в каталоге пользователя;
3 - запуск без UI для записи в образ (командная строка, необходимы права суперпользователя).
 
Запуск в режиме 1 осуществляется с помощью ярлыка BinInjector из списка установленных приложений, либо из командной строки командой bininjector.
Запуск в режиме 2 возможен после включения соответствующей опции в меню Настройки/Запуск без pkexec.
Запуск в режиме 3 осуществляется командой bininjector со следущими аргументами:
bininjector -C путь_к_образу S путь_к_данным для копирования одного файла, либо
bininjector -C путь_к_образу F путь_к_данным для копирования структуры папки.
Так же bininjector распознаёт одиночный аргумент -r как запрос на выполнение режима 2, но в этом случае перключение режимов будет заблокировано.

[EN]
This application is designed for the convenience and automation of copying data into the disk images (bitwise copies) the contents of folders with preservation of structure, as well as the single files.
Three operating modes are supported:
1 - launching the UI with a request for the superuser password, settings are saves and loads from a corresponding directory;
2 - launching the UI with hidden requests for superuser rights from the program (convenient on systems running sudo without authorization, on others - it can cause an errors when starting with a shortcut and asking for rights from sudo when starting from the command line), settings are in the user directory;
3 - launch without UI to write to the image (command line, superuser rights required).
  
Starting in mode 1 is possible using the BinInjector shortcut from the list of installed applications, or from the command line with bininjector command.
Starting in mode 2 is possible after enabling the corresponding option by the Settings / Launching as user checkbox.
Starting in mode 3 is possible by bininjector command with following arguments:
bininjector -C image_path S data_path for copying one file, or
bininjector -C image_path F data_path for copying the folder structure.
Also, bininjector recognizes a single argument -r as a request to execute mode 2, but in this case mode switching will be blocked.
