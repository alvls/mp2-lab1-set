// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
using namespace std;
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);


//Нет требования сделать для любого типа Mem элемента, поэтому лучше вручную константы
//В .h нельзя добавлять, bit_width не работает т.к.с C++20 не собирается решение
#define TELEM_BITS 32 //sizeof(TELEM)*8 
#define TELEM_BYTES 4 //sizeof(TELEM)
#define TELEM_LOG 5 //bit_width(sizeof(TELEM)*8-1) ?

//#define MY_DEBUG

//Некоторые попытки в эффективность: в операциях над полями отсутствие итерирования по битам с частыми вызовами getBit/Set/ClrBit - копирование с помощью pMem, операции над целыми элементами-мемами, переиспользование маски

TBitField::TBitField(int len)
{
    //BitsInMem = numeric_limits<TELEM>::digits; // TELEM = unsigned int = 4 bytes
    
    BitLen = len;
    if (len > 0) {
        MemLen = GetMemIndex(BitLen - 1) + 1;
        pMem = new TELEM[MemLen];
        memset(pMem, 0, TELEM_BYTES * MemLen);
    }
    else if (len == 0){
        MemLen = 0;
        pMem = 0;
    }
    else {
        throw "Negative bitfield length";
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, TELEM_BYTES * MemLen);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
#ifdef MY_DEBUG
    //cout << "MEM " << ((n + TELEM_BITS) >> TELEM_LOG) - 1 << "< " << MemLen << endl;
#endif
    if ((n >= BitLen) || (n < 0)) 
        throw out_of_range("Accessing bit out of bitfield range");
    
        return (n >> TELEM_LOG); //bit_width(BitsInMem - 1) не работает;//(n + BitsInMem - 1) / BitsInMem - 1; //Деление с округлением вверх
}


TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
#ifdef MY_DEBUG
    //cout << "MASK " << (1 << (n & (TELEM_BITS - 1))) << endl;
#endif
    return 1<<(n&(TELEM_BITS-1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
#ifdef MY_DEBUG
    cout << "set " << n << " < " << BitLen<<endl;
    cout << "CHECK BEFORE " << GetBit(n) << " IN MEM " << pMem[GetMemIndex(n)] << endl;
#endif
    //if((n<BitLen)&&(n>=0))
        (pMem[GetMemIndex(n)]) |= GetMemMask(n);
#ifdef MY_DEBUG
    cout << "CHECK AFTER " << GetBit(n)<<" IN MEM "<< pMem[GetMemIndex(n)] << endl;
#endif
}

void TBitField::ClrBit(const int n) // очистить бит
{
#ifdef MY_DEBUG
    cout << "clr " << n << " < " << BitLen<<endl;
#endif
    //if ((n < BitLen)&&(n>=0))
        (pMem[GetMemIndex(n)]) &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    return !!((pMem[GetMemIndex(n)]) & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    memcpy(pMem, bf.pMem, TELEM_BYTES * MemLen);
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  return (BitLen != bf.BitLen) ? 0 : !memcmp(pMem, bf.pMem, TELEM_BYTES * MemLen); // сравнивает также остаточные биты, но они гарантированно нулевые
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField rf(0);
    const TBitField* sf;

    if (BitLen >= bf.BitLen) {
        rf = (*this);
        sf = &bf;
    }
    else {
        rf = (bf);
        sf = this;
    }
    //cout << "OR SMOL MEMLEN " << sf->MemLen << endl;
    cout << rf << endl << *sf << endl;
    for (int i = 0; i < sf->MemLen;i++) {
        //cout << "OR MEM NO " << i << endl;
        rf.pMem[i] |= (sf->pMem)[i];
    }
    return rf;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField rf(0);
    const TBitField* sf;

    if (BitLen >= bf.BitLen) {
        rf = (*this);
        sf = &bf;
    }
    else {
        rf = (bf);
        sf = this;
    }
    //cout << "AND SMOL MEMLEN " << sf->MemLen << endl;
    cout << rf << endl << *sf << endl;
    for (int i = 0; i < sf->MemLen;i++) {
        //cout << "AND MEM NO " << i << endl;
        rf.pMem[i] &= (sf->pMem)[i];
    }
    return rf;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField rf(*this);
    int fullMems = BitLen / TELEM_BITS;
    for (int i = 0; i < fullMems;i++) // целые мемы отрицает целиком
        rf.pMem[i] = ~(rf.pMem[i]);
    int rem =  TELEM_BITS * (fullMems+1) - BitLen; //остаточные нули которые нельзя отрицать для нормальной работы сравнения
    //TELEM* lastMem = rf.pMem + fullMems;
    if (rem) {
        TELEM mask = static_cast<TELEM>(-1) >> rem;
        rf.pMem[MemLen-1]^=mask;
    }
    //for (int i = BitLen - rem; i < BitLen;i++) { // неполные мемы отрицает только в нужной области
    //    TELEM mask = rf.GetMemMask(i); // маска имспользуется повторно
    //    if (*lastMem & mask)
    //        *lastMem &= ~mask;
    //    else
    //        *lastMem |= mask;
    //}
    return rf;
}

// ввод/вывод
// Непопулярные операции, не стоят оптимизации
istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        bool bit;
        istr >> bit;
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
