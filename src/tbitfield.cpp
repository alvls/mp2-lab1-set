// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static const int TELEM_BITS = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    if (len < 0)
        throw FAKE_INT;
    BitLen = len;
    MemLen = (len + TELEM_BITS - 1) / TELEM_BITS;
    pMem = new TELEM[MemLen];
    for(int i=0;i<MemLen;i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    pMem = new TELEM[bf.MemLen];
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    for(int i=0;i<MemLen;i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw FAKE_INT;
    return n / TELEM_BITS;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw FAKE_INT;
    return TELEM(1) << n % TELEM_BITS;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if(n < 0 || n >= BitLen)
    throw FAKE_INT;
  pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if(n < 0 || n >= BitLen)
    throw FAKE_INT;
  pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if(n < 0 || n >= BitLen)
    throw FAKE_INT;
  return int(((pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % TELEM_BITS))&1);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    TELEM* _pMem = new TELEM[bf.MemLen];
    delete[] pMem; pMem = _pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    for(int i=0;i<MemLen;i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  int i;
  if(BitLen != bf.BitLen)
    return 0;
  if(pMem == bf.pMem)
    return 1;
  for(i = 0;i<MemLen;i++)
    if(pMem[i] != bf.pMem[i])
      break;
  if(i != MemLen)
    return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (pMem == bf.pMem)
        return *this;
    if (BitLen >= bf.BitLen) {
        TBitField res(*this);
        for (int i = 0; i < MemLen; i++)
            res.pMem[i] |= bf.pMem[i];
        return res;
    }else {
        TBitField res(bf);
        for (int i = 0; i < bf.MemLen; i++)
            res.pMem[i] |= pMem[i];
        return res;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (pMem == bf.pMem)
        return *this;
    if (BitLen >= bf.BitLen) {
        TBitField res(*this);
        for (int i = 0; i < MemLen; i++)
            res.pMem[i] &= bf.pMem[i];
        return res;
    }
    else {
        TBitField res(bf);
        for (int i = 0; i < bf.MemLen; i++)
            res.pMem[i] &= pMem[i];
        return res;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(BitLen);
  for(int i = 0;i<MemLen;i++)
    res.pMem[i] = ~pMem[i];
  res.pMem[MemLen - 1] &= (~TELEM(0) >> (TELEM_BITS - BitLen % TELEM_BITS));
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char* enter = new char[bf.GetLength()];
    istr >> enter;
    for (int i = 0; i < bf.GetLength(); i++)
        if (enter[i] == '1')
            bf.SetBit(i);
        else if (enter[i] == '0')
            bf.ClrBit(i);
        else
            throw FAKE_INT;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
