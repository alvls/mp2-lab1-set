// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 1)
        throw string("Wrong value of len in TBitField::TBitField(int len)");
    BitLen = len;
    MemLen = static_cast<int>(ceil(static_cast<float>(len) / (sizeof(TELEM) * 8)));
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = static_cast<TELEM>(0);
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

int TBitField::GetMemIndex(const int n) const // индекс pМем для бита n
{
    return (n / (sizeof(TELEM) * 8));
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
    if (n < 0 || n > BitLen)
        throw string("Wrong value of n in void TBitField::SetBit(const int n)");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen)
        throw string("Wrong value of n in void TBitField::ClrBit(const int n)");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen)
        throw string("Wrong value of n in int TBitField::GetBit(const int n) const");
    return ((pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8)));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
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
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen > bf.BitLen)
    {
        TBitField tmp(*this);
        for (int i = 0; i < MemLen; i++)
            tmp.pMem[i] |= bf.pMem[i];
        return tmp;
    }
    else
    {
        TBitField tmp(bf);
        for (int i = 0; i < MemLen; i++)
            tmp.pMem[i] |= pMem[i];
        return tmp;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen > bf.BitLen)
    {
        TBitField tmp(*this);
        for (int i = 0; i < MemLen; i++)
            tmp.pMem[i] &= bf.pMem[i];
        return tmp;
    }
    else
    {
        TBitField tmp(bf);
        for (int i = 0; i < MemLen; i++)
            tmp.pMem[i] &= pMem[i];
        return tmp;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++)
        if (GetBit(i) != 0)
            tmp.ClrBit(i);
        else
            tmp.SetBit(i);
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string tmpstr;
    istr >> tmpstr;
    bf = TBitField(tmpstr.size()); // bf == 0 , bf.MemLen == tmpstr.size()
    for (int i = 0; i < tmpstr.size(); i++)
        if (tmpstr[i] == '1')
            bf.SetBit(i);
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
