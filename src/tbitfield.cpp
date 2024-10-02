// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#include <stdexcept>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) // конструктор инициализатор
{
  if (len <= 0)
    throw std::out_of_range("The length of the bit field must be positive");
  BitLen = len;
  MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  if (bf.BitLen <= 0)
    throw std::out_of_range("The length of the bit field must be positive");
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() // деструткор
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if ((n >= BitLen) || (n < 0))
    throw std::out_of_range("The bit number must not exceed the length of the bit field and must not be negative");
  int index = GetMemIndex(n); // индекс элемента Mem для бита n
  pMem[index] = pMem[index] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if ((n >= BitLen) || (n < 0))
    throw std::out_of_range("The bit number must not exceed the length of the bit field and must not be negative");
  int index = GetMemIndex(n); // индекс элемента Mem для бита n
  pMem[index] = pMem[index] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n >= BitLen) || (n < 0))
    throw std::out_of_range("The bit number must not exceed the length of the bit field and must not be negative");
  int index = GetMemIndex(n); // индекс элемента Mem для бита n
  int result = pMem[index] & GetMemMask(n);
  return result != 0;
}

// битовые операции

TBitField &TBitField::operator=(const TBitField &bf) // присваивание
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  delete[] pMem;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
  // MemLen зависит от BitLen, поэтому нет смысла сравнивать это поле у объектов
  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int maxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen; // длина результирующего битового поля
  TBitField result(maxBitLen);
  int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen; // длина наименьшего битового поля
  for (int i = 0; i < minMemLen; i++)
    result.pMem[i] = pMem[i] | bf.pMem[i]; // вычисляемая часть битового поля
  if (BitLen > bf.BitLen)
    for (int i = minMemLen; i < MemLen; i++)
      result.pMem[i] = pMem[i];
  else if (bf.BitLen > BitLen)
    for (int i = minMemLen; i < bf.MemLen; i++)
      result.pMem[i] = bf.pMem[i];
  return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int maxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen; // длина результирующего битового поля
  TBitField result(maxBitLen);
  int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen; 
  for (int i = 0; i < minMemLen; i++)
    result.pMem[i] = pMem[i] & bf.pMem[i]; // вычисляемая часть битового поля
  return result;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField result(BitLen);
  for (int i = 0; i < MemLen; i++)
    result.pMem[i] = ~pMem[i];
  // обнуляем "лишние" биты за пределами BitLen
  int extraBits = MemLen * sizeof(TELEM) * 8 - BitLen; // число "лишних" битов
  if (extraBits > 0)
  {
    TELEM mask = (1 << (sizeof(TELEM) * 8 - extraBits)) - 1;  // маска для очистки "лишних" битов
    result.pMem[MemLen - 1] = result.pMem[MemLen - 1] & mask; // применение маски к последнему элементу pMem
  }
  return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string str;
  istr >> str; // считываем строку из потока
  if (str.length() != bf.BitLen)
    throw std::invalid_argument("The length of the string must be equal to the length of the bit field");
  for (int i = 0; i < bf.BitLen; i++) // пробегаем по строке и устанавливаем биты
    if (str[i] == '1')
      bf.SetBit(i); // установка бита
    else if (str[i] == '0')
      bf.ClrBit(i); // очистка бита
    else
      throw std::invalid_argument("The string contains invalid characters");
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++)
    ostr << bf.GetBit(i);
  return ostr;
}
