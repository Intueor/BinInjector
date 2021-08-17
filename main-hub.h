#ifndef MAINHUB_H
#define MAINHUB_H

//== ВКЛЮЧЕНИЯ.
#include "main-hub-defs.h"

//== ФУНКЦИИ.
/// Проверка на наличие файла или папки.
bool IsFileOrFolderExists(char* p_chPath, bool bSeekFile);
						///< \param[in] p_chPath Указатель на массив строки с путём к файлу.
						///< \param[in] bSeekFile При true - поик файла и наоборот.
						///< \return true, при удаче.
/// Копирование строчного массива в другой.
void CopyStrArray(char m_chFrom[], char m_chTo[], uint uiDestSize);
												///< \param[in] m_chFrom Ссылка на массив откуда.
												///< \param[in] m_chTo Ссылка на массив куда.
												///< \param[in] uiDestSize Размер буфера доставки.
/// Проверка строки на содержание числа.
bool IsStrInteger(char* p_chNumber, bool bIsHex = false);
												///< \param[in] p_chNumber Указатель на массив строки с цифрой.
												///< \param[in] bIsHex Признак шестнадцатиричной системы.
												///< \return true, при удаче

#endif // MAINHUB_H
