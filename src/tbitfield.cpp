// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"///////ЗДЕСЬ ПИШЕМ КОД
//static - живут всю программу, как и константы, но обьявлять их можно в любом месте, а не только в начале. и инициализируются только 1 раз. с обьектами класса то же самое
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len=0)
{
    BitLen = len;
    MemLen = BitLen / sizeof(TELEM);
    if (BitLen % sizeof(TELEM) != 0)
        MemLen++;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];//т.к мы не выделяли паиять в новом объекте, потому что он новый!!!!!
    for (int i = 0; i < MemLen; i++)
    {
        this->pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()//деструктор
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int num = n / sizeof(TELEM);
    if (n % sizeof(TELEM) != 0)
        num++;
    return num;//?
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask;
    mask = 1 << (n % (sizeof(int) * 8));
    return (mask);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n);// на n-й позиции маски 1. значит, при битовом или в результате операции будет на n-м месте 1 в любом случае
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~(GetMemMask(n)));;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int bit;
    bit = (pMem[GetMemIndex(n)] & GetMemMask(n)) / GetMemMask(n);
    return bit;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (MemLen != 0)
    {
        delete[]pMem;
    }
    this->pMem = new TELEM[MemLen];
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    for (int i = 0; i < this->BitLen; i++)
    {
        this->pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int flag=1;
    if (bf.BitLen != this->BitLen)
        flag = 0;
    else
    {
        for (int i = 0; i < this->BitLen; i++)
            if (this->pMem[i] != bf.pMem[i])
                flag = 0;
    }
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int flag=0;
    if (bf.BitLen != this->BitLen)
        flag = 1;
    else
    {
        for (int i = 0; i < this->BitLen; i++)
            if (this->pMem[i] != bf.pMem[i])
                flag = 1;
    }
    return flag;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    /*TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < BitLen; i++)
    {
        if ((pMem[i] == 1) || (bf.pMem[i] == 1))
            res.pMem[i] = 1;
        else
            res.pMem[i] = 0;
    }
    return res;*/
    if (BitLen > bf.BitLen)
    {
        TBitField temp(BitLen);
        int r = MemLen - bf.MemLen;
        for (int i = 0; i < r; i++)
        {
            temp.pMem[i] = pMem[i];
        }
        for (int i = r; i < MemLen; i++)
        {
            temp.pMem[i] = pMem[i] | bf.pMem[i - r];
        }
        return temp;
    }
    else
    {
        TBitField temp(bf.BitLen);
        int sub = bf.MemLen - MemLen;

        for (int i = 0; i < sub; i++)
        {
            temp.pMem[i] = bf.pMem[i];
        }
        for (int i = sub; i < MemLen; i++)
        {
            temp.pMem[i] = bf.pMem[i] | pMem[i - sub];
        }
        return temp;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    /*TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < BitLen; i++)
    {
        if ((pMem[i] == 1) && (bf.pMem[i] == 1))
            res.pMem[i] = 1;
        else
            res.pMem[i] = 0;
    }
    return res;*/
    if (BitLen > bf.BitLen)
    {
        TBitField temp(BitLen);
        int sub = MemLen - bf.MemLen;

        for (int i = 0; i < sub; i++)
        {
            temp.pMem[i] = 0;
        }
        for (int i = sub; i < MemLen; i++)
        {
            temp.pMem[i] = pMem[i] & bf.pMem[i - sub];
        }
        return temp;
    }
    else
    {
        TBitField temp(bf.BitLen);
        int r = bf.MemLen - MemLen;

        for (int i = 0; i < r; i++)
        {
            temp.pMem[i] = 0;
        }
        for (int i = r; i < MemLen; i++)
        {
            temp.pMem[i] = bf.pMem[i] & pMem[i - r];
        }
        return temp;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i)) tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод, битфилд не константный!!!
{
    string bitstring;
    istr >> bitstring;
    TBitField tmp(bitstring.length());
    for (int i = 0; i < bitstring.length(); i++)
    {
        if (bitstring[i] == '1')
            tmp.SetBit(i);
    }
    bf = tmp;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    ostr << " { ";
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i) << " ";
    ostr << " } ";
    return ostr;
}
