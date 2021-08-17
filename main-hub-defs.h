#ifndef MAINHUBDEFS_H
#define MAINHUBDEFS_H

//== КОНСТАНТЫ.
// Многократно используемые строки.
// Типа: const char m_chLogStart[] = "START.";

//== МАКРОСЫ.
// Общее.
#define DEF_CHAR_PTH(def)               &(_chpPH = def)
#define CHAR_PTH                        char _chpPH
// Разное.
#define ResetBits(field,bits)				field |= bits; field ^= bits
#define SetBits(field,bits)					ResetBits(field,bits); field |= bits
#define CopyBits(field_src,field_dst,bits)	ResetBits(field_dst,bits); field_dst |= (field_src & bits)
// Диалоги.
#define DIALOGS_ACCEPT                  0
#define DIALOGS_REJECT                  -1
// Функции.
#define SEEK_FILE						true
#define SEEK_FOLDER						false

#endif // MAINHUBDEFS_H
