// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <string>

const int bitsInElem = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
  if (len < 0)
    throw invalid_argument("negative length");
  BitLen = len;
  MemLen = (len + bitsInElem - 1) / bitsInElem;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / bitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return TELEM(1) << (n % bitsInElem);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("bit index out of range");
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("bit index out of range");
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("bit index out of range");
  if (pMem[GetMemIndex(n)] & GetMemMask(n))
    return 1;
  return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf)
    return *this;
  if (MemLen != bf.MemLen)
  {
    delete[] pMem;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
  }
  BitLen = bf.BitLen;
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
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
  int len = BitLen;
  if (bf.BitLen > len)
    len = bf.BitLen;
  TBitField res(len);
  for (int i = 0; i < MemLen; i++)
    res.pMem[i] = pMem[i];
  for (int i = 0; i < bf.MemLen; i++)
    res.pMem[i] |= bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int len = BitLen;
  if (bf.BitLen > len)
    len = bf.BitLen;
  TBitField res(len);
  int minMem = MemLen;
  if (bf.MemLen < minMem)
    minMem = bf.MemLen;
  for (int i = 0; i < minMem; i++)
    res.pMem[i] = pMem[i] & bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen; i++)
    res.pMem[i] = ~pMem[i];
  int tail = BitLen % bitsInElem;
  if (tail != 0)
    res.pMem[MemLen - 1] &= (TELEM(1) << tail) - 1;
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string s;
  istr >> s;
  int len = s.length();
  TBitField tmp(len);
  for (int i = 0; i < len; i++)
  {
    if (s[i] == '1')
      tmp.SetBit(i);
    else if (s[i] != '0')
    {
      istr.setstate(ios::failbit);
      return istr;
    }
  }
  bf = tmp;
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++)
    ostr << bf.GetBit(i);
  return ostr;
}
