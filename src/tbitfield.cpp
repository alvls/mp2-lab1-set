// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
static const int FAKE_INT = -1;
// Fake variables used as placeholders in test
TBitField::TBitField(int len)
{
    /*if (len < 0)
        throw;*/
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * CHAR_BIT) + 1;
    pMem = new TELEM[MemLen];
}

TBitField::TBitField(const TBitField &bf): BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n +
{
    if (n >= BitLen)
        throw - 1;
    return ((n - 1) / (sizeof(TELEM) * CHAR_BIT));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n +
{
    const int num = GetMemIndex(n);
    TELEM mask;
    const int offset = n % (sizeof(TELEM) * CHAR_BIT);//смещение
    mask = 1 << offset;
    mask &= pMem[n / (sizeof(TELEM) * CHAR_BIT)];
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов) +
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит +
{
    if (n < 0)
        throw - 1;
    else
        if (n > BitLen)
            throw "Over Len\n";
    TELEM tmp;
    tmp = (1 << n);
    pMem[n / (sizeof(TELEM) * CHAR_BIT) + 1] |= tmp;
}

void TBitField::ClrBit(const int n) // очистить бит +
{  
    if (n < 0 || n>BitLen - 1)
        throw - 1;
    TELEM tmp = (1 << n);
    tmp = ~tmp;
    pMem[n / (sizeof(TELEM) * CHAR_BIT)] &= tmp;
}

int TBitField::GetBit(const int n) const // получить значение бита +
{
    if (n >= BitLen)
        throw - 1;
    if (n < 0)
        throw - 1;
    TELEM tmp = GetMemMask(n);
    if (tmp > 0)
        return 1;
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание +
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

int TBitField::operator==(const TBitField &bf) const // сравнение +
{
        if (bf.BitLen != BitLen)
            throw false;
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return false;
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение +
{
    if (bf.BitLen != BitLen)
        throw true;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return true;
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или" +
{
    for (int i = 0; i < MemLen; i++)
        pMem[i] |= bf.pMem[i];
    return (*this);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и" +
{
    for (int i = 0; i < MemLen; i++)
        pMem[i] &= bf.pMem[i];
    return (*this);
}

TBitField TBitField::operator~(void) // отрицание - ДОДЕЛАТЬ
{
    for (int i = 0; i < MemLen-1; i++)
        pMem[i] = ~pMem[i];
    /*TELEM masks[sizeof(TELEM) * CHAR_BIT];
    for (int i = MemLen * sizeof(TELEM) * CHAR_BIT, int j = 0; i < BitLen; i++, j++)
        masks[j] = GetMemMask(i);*/
    return (*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод 
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод - ПЕРЕДЕЛАТЬ
{
    TELEM tmp; //чтобы беспрепятственно портить переменную
    TELEM deg_of_two;//для того чтобы понимать, есть ли в двоичной записи на этом месте 1 или нет
    for (int i = 0; i < (bf.BitLen / (sizeof(TELEM) * CHAR_BIT)); i++)
    {
        tmp = bf.pMem[i];
        for (int j = 7; j > -1; j--)
        {
            deg_of_two = pow(2, j);
            if ((tmp / deg_of_two) > 0)
                ostr << 1;
            else
                ostr << 0;
            tmp %= deg_of_two;
        }
    }
    const int ExtraBitLen = bf.BitLen % (sizeof(TELEM) * CHAR_BIT);
    for (int i = 0; i < ExtraBitLen; i++)
    {
        tmp = bf.pMem[i];
        for (int j = 7; j > -1; j--)
        {
            deg_of_two = pow(2, j);
            if ((tmp / deg_of_two) > 0)
                ostr << 1;
            else
                ostr << 0;
            tmp %= deg_of_two;
        }
    }
    return ostr;
}
