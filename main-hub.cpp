//== ВКЛЮЧЕНИЯ.
#include <QFileInfo>
#include <QDir>
#include "main-hub.h"

//== ФУНКЦИИ.
// Копирование строчного массива в другой.
void CopyStrArray(char m_chFrom[], char m_chTo[], uint uiDestSize)
{
	strncpy(m_chTo, m_chFrom, uiDestSize);
	m_chTo[uiDestSize - 1] = 0;
}

// Проверка, содержит ли строка исключительно число.
bool IsStrInteger(char* p_chNumber, bool bIsHex)
{
	char m_chDec[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	char m_chHex[] = {'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'};
	//
	while(*p_chNumber != 0)
	{
		for(int iP = 0; iP != 10; iP++)
		{
			if(*p_chNumber == m_chDec[iP])
			{
				goto gOk;
			}
		}
		if(bIsHex)
		{
			for(int iP = 0; iP != 12; iP++)
			{
				if(*p_chNumber == m_chHex[iP])
				{
					goto gOk;
				}
			}
		}
		return false;
gOk:    p_chNumber++;
	}
	return true;
}

