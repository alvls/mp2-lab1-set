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
    if (len > 0)
    {
        this->BitLen = len;
        this->MemLen = ((int)((this->BitLen - 1) / ((sizeof(TELEM) * 8)))) + 1;
        this->pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
    else
    {

      throw ("tbitfield len error (<0)");
    }


}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

bool isIndexCorrect(int index, int bitlen) {
    if (index < 0 || index + 1 > bitlen) {
        return false;
    }
    return true;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (!isIndexCorrect(n,BitLen))
    {
        throw ("bit incorrect index error");

    }
    return (int)(n / (sizeof(TELEM) * 8));

}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (!isIndexCorrect(n, BitLen))
    {
        throw ("bit incorrect index error");
    }
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (!isIndexCorrect(n, BitLen))
    {
        throw ("bit incorrect index error");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (!isIndexCorrect(n, BitLen))
    {
        throw ("bit incorrect index error");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (!isIndexCorrect(n, BitLen))
    {
        throw ("bit incorrect index error");
    }
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf != this)
    {
        this->BitLen = bf.BitLen;
        this->MemLen = bf.MemLen;
        delete[] this->pMem;
        this->pMem = new TELEM[bf.MemLen];
        for (int i = 0; i < bf.MemLen; i++){
            this->pMem[i] = bf.pMem[i];
        }
        return *this;
    }
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bf.MemLen != MemLen || bf.BitLen != BitLen){
        return 0;
    }
    else {
        for (int i = 0; i < MemLen; i++){
            if (bf.pMem[i] != pMem[i]) {
                return 0;
            }
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (bf.BitLen != BitLen || bf.MemLen != MemLen) {
        return 1;
    }
    else {
        for (int i = 0; i < MemLen; i++){
            if (bf.pMem[i] != pMem[i]) {
                return 1;
            }
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (bf.BitLen >= BitLen){
        TBitField tempBitField(bf.BitLen);
        for (int i = 0; i < bf.MemLen; i++)
        {
            tempBitField.pMem[i] = this->pMem[i] | bf.pMem[i];
        }
        return tempBitField;
    }
    else
    {
        TBitField tempBitField(BitLen);
        for (int i = 0; i < MemLen; i++)
        {
            tempBitField.pMem[i] = this->pMem[i] | bf.pMem[i];
        }
        return tempBitField;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (bf.BitLen >= BitLen) {
        TBitField tempBitField(bf.BitLen);
        for (int i = 0; i < bf.MemLen; i++)
        {
            tempBitField.pMem[i] = this->pMem[i] & bf.pMem[i];
        }
        return tempBitField;
    }
    else
    {
        TBitField tempBitField(BitLen);
        for (int i = 0; i < MemLen; i++)
        {
            tempBitField.pMem[i] = this->pMem[i] & bf.pMem[i];
        }
        return tempBitField;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tempBitField(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0) {
            tempBitField.SetBit(i);
        }
        else {
            tempBitField.ClrBit(i);
        }

    }
    return tempBitField;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string tempString; 
    istr >> tempString;
    bf = TBitField(tempString.size());
    for (int i = 0; i < tempString.size(); i++)
    {
        //bf.ClrBit(i);
        if (tempString[i] == (const char) "1")
        {
            bf.SetBit(i);
        }
        

    }
    return istr;
     
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int tempInt;
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);;

    }
    return ostr;
}


