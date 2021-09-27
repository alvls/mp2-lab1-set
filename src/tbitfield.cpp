// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
static const int FAKE_INT = -1;
// Fake variables used as пlaceholders in test
TBitField::TBitField(int len)
{
    if (len < 0)
        throw - 1;
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * CHAR_BIT) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
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
    if (n > BitLen - 1 || n < 0)
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
    if (n < 0 || n>BitLen)
        throw - 1;
    TELEM tmp;
    tmp = (1 << n);
    pMem[n / (sizeof(TELEM) * CHAR_BIT)] |= tmp;
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
            return 0;
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение +
{
    if (bf.BitLen != BitLen)
        return 1;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxlen;
    int memlen;
    if (BitLen > bf.BitLen)
    {
        maxlen = BitLen;
        memlen = bf.MemLen;
    }
    else
    {
        maxlen = bf.BitLen;
        memlen = MemLen;
    }
    TBitField out(maxlen);
    for (int i = 0; i < memlen; i++)
    {
        out.pMem[i] = bf.pMem[i] | pMem[i];
    }
    return out;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxlen;
    int memlen;
    if (BitLen > bf.BitLen)
    {
        maxlen = BitLen;
        memlen = bf.MemLen;
    }
    else
    {
        maxlen = bf.BitLen;
        memlen = MemLen;
    }
    TBitField out(maxlen);
    for (int i = 0; i < memlen; i++) 
    {
        out.pMem[i] = bf.pMem[i] & pMem[i];
    }
    return out;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField out(*this);
    for (int i = 0; i < BitLen; i++)
        if (out.GetBit(i) > 0)
            out.ClrBit(i);
        else
            out.SetBit(i);
    return out;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод 
{
    int max;
    for (max = 0; _getch() != '\n'; max++)
        ;
    int* tmp;
    tmp = new int[max];
    for (int i = 0; i < max; i++)
        tmp[i] = _getch();
    delete[] bf.pMem;
    bf.BitLen = max;
    bf.MemLen = (bf.BitLen - 1) / (sizeof(TELEM) * CHAR_BIT) + 1;
    bf.pMem = new TELEM[bf.MemLen];
    for (int i = 0; i < max; i++)
        if (tmp[i] > '0')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    TELEM tmp; //чтобы беспрепятственно портить переменную
    TELEM deg_of_two;//для того чтобы понимать, есть ли в двоичной записи на этом месте 1 или нет
    int out;
    for (int i = 0; i < bf.BitLen; i++)
    {
        out = bf.GetBit(i);
        ostr << out;
    }
    ostr << '\n';
    ////Если бы у меня не было волшебного GetBit()
    //for (int i = 0; i < (bf.BitLen / (sizeof(TELEM) * CHAR_BIT)); i++)
    //{
    //    tmp = bf.pMem[i];
    //    for (int j = 7; j > -1; j--)
    //    {
    //        deg_of_two = pow(2, j);
    //        if ((tmp / deg_of_two) > 0)
    //            ostr << 1;
    //        else
    //            ostr << 0;
    //        tmp %= deg_of_two;
    //    }
    //}
    //const int ExtraBitLen = bf.BitLen % (sizeof(TELEM) * CHAR_BIT);
    //for (int i = 0; i < ExtraBitLen; i++)
    //{
    //    tmp = bf.pMem[i];
    //    for (int j = 7; j > -1; j--)
    //    {
    //        deg_of_two = pow(2, j);
    //        if ((tmp / deg_of_two) > 0)
    //            ostr << 1;
    //        else
    //            ostr << 0;
    //        tmp %= deg_of_two;
    //    }
    //}
    return ostr;
}
