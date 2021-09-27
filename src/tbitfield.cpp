// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <string.h>
#include "tbitfield.h"

// Fake variables used as placeholders in tests

TBitField::TBitField(int len)
{
  if (len < 0)
      throw string("Incorrect lenght of bit field");
  BitLen = len;
  MemLen = len / (sizeof(TELEM)*8) + 1;
  pMem = new TELEM[MemLen];
  for(int i = 0; i < MemLen; i++){
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for(int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  return 1 << (n % (sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  return pMem[index] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if(&bf == this) {
    return *this;
  }
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  delete[] pMem;
  pMem = new TELEM [MemLen];
  for(int i = 0; i < MemLen; i++){
   pMem[i] = bf.pMem[i]; 
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if(&bf == this) {
    return 1;
  }
  if(BitLen == bf.BitLen){
    int cnt = 0;
    for(int i = 0; i < MemLen; i++ ){
      if(pMem[i] == bf.pMem[i])
        cnt++;
    }
    if(cnt == MemLen)
      return 1;
    else
      return 0;
  }
  else
    return 0;
}
int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (&bf == this) {
      return 0;
  }
  else
      return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int len, minBitlen;
  if (bf.BitLen < BitLen) {
      len = bf.MemLen;
      minBitlen = bf.BitLen;
  }
  else {
      len = MemLen;
      minBitlen = BitLen;
  }
  TBitField out(bf);
  for (int i = 0; i < len - 1; i++) {
      out.pMem[i] = bf.pMem[i] | pMem[i];
  }
  for (int i = (len - 1) * sizeof(TELEM) * 8; i < minBitlen; i++) {
      if (bf.GetBit(i) || GetBit(i)) 
          out.SetBit(i);
  }
  return out;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int len, maxBitlen, minBitlen;
  if (bf.BitLen < BitLen) {
      len = bf.MemLen;
      maxBitlen = BitLen;
      minBitlen = bf.BitLen;
  }
  else {
      len = MemLen;
      maxBitlen = bf.BitLen;
      minBitlen = BitLen;
  }
  TBitField out(maxBitlen);
  for (int i = 0; i < len - 1; i++) {
      out.pMem[i] = bf.pMem[i] & pMem[i];
  }
  for (int i = (len - 1) * sizeof(TELEM) * 8; i < minBitlen; i++) {
      if (bf.GetBit(i) && GetBit(i)) 
          out.SetBit(i);
  }
  return out;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField out(*this);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i)) out.ClrBit(i);
        else out.SetBit(i);
    }
    return out;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string stream;
  int len;
  istr >> stream;
  delete[] bf.pMem;
  bf.BitLen = len = stream.length();
  bf.MemLen = bf.BitLen / (sizeof(TELEM) * 8) + 1;
  bf.pMem = new TELEM[bf.MemLen];
  for (int i = 0; i < len; i++) {
    if (stream[i] == 1) bf.SetBit(i);
    else bf.ClrBit(i);
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for(int i = 0; i < bf.BitLen; i++){
      bf.GetBit(i) ? ostr << '1' : ostr << '0';
  }
  return ostr;
}
