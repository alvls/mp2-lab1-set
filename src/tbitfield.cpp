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

TBitField::TBitField(int len)
{
    if (len < 0)
        throw "The length is wrong";
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * CHAR_BIT) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = TELEM(0);
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
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
    if ((n >= BitLen) && (n < 0))
        throw "Index is out of range";
    return (n / (sizeof(TELEM) * CHAR_BIT));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n >= BitLen) && (n < 0))
        throw "Index is out of range";
    TELEM mask;
    mask = 1 << (n % (sizeof(TELEM) * CHAR_BIT));
    mask &= pMem[n / (sizeof(TELEM) * CHAR_BIT)];
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
        throw "Index is out of range";
    TELEM tmp;
    tmp = (1 << n);
    pMem[GetMemIndex(n)] |= tmp;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
        throw "Index is out of range";
    TELEM tmp = ~(1 << n);
    pMem[GetMemIndex(n)] &= tmp;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0)
        throw "Index is out of range";
    if (GetMemMask(n) > 0)
        return 1;
    else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf != this)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
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
    if (operator==(bf) == 1)
        return 0;
    else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    const TBitField& longer = (BitLen > bf.BitLen) ? *this : bf, & shorter = (BitLen > bf.BitLen) ? bf : *this;
    int len = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
    int difference = abs(MemLen - bf.MemLen);
    TBitField result(__max(BitLen, bf.BitLen));
    for (int i = 0; i < difference; i++)
        result.pMem[i] = longer.pMem[i];
    for (int i = 0; i < len; i++)
        result.pMem[i + difference] = longer.pMem[i + difference] | shorter.pMem[i];
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    const TBitField& longer = (BitLen > bf.BitLen) ? *this : bf, & shorter = (BitLen > bf.BitLen) ? bf : *this;
    int len = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
    int difference = abs(MemLen - bf.MemLen);
    TBitField result(__max(BitLen, bf.BitLen));
    for (int i = 0; i < difference; i++)
        result.pMem[i] = TELEM(0);
    for (int i = 0; i < len; i++)
        result.pMem[i + difference] = longer.pMem[i + difference] & shorter.pMem[i];
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i))
            result.SetBit(i);
    }
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string bitstr;
    istr >> bitstr;
    for (int i = 0; i < bitstr.length(); i++)
        if (bitstr[i] == '1')
            bf.SetBit(i);
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    ostr << "{ ";
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i) << " ";
    ostr << "}";
    return ostr;
}
