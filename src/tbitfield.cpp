// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{   
    if (len < 0)
        throw invalid_argument("The length must be positive");
    BitLen = len;
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
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
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("The index must be between 0 and BitLen");
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("The index must be between 0 and BitLen");
    return 1 << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const// получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("The index must be between 0 and BitLen");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("The index must be between 0 and BitLen");
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("The index must be between 0 and BitLen");
    if (pMem[GetMemIndex(n)] & GetMemMask(n))
        return 1;
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{   
    if (*this == bf)
        return *this;
    if (BitLen != bf.BitLen)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(operator==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int BitLen_result = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int MemLen_result = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    TBitField result(BitLen_result);
    for (int i = 0; i < MemLen_result; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];
    return result;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int BitLen_result = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int MemLen_result = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    TBitField result(BitLen_result);
    for (int i = 0; i < MemLen_result; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 1)
            result.ClrBit(i);
        else
            result.SetBit(i);
    }
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{   
    char bit;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> bit;
        if (bit == '0')
            bf.ClrBit(i);
        if (bit == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{   
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
