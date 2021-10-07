// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0)
    {
        throw "Negative value of the variable";
    }
    BitLen = len;
    //1 варинат
    MemLen = BitLen / (sizeof(TELEM) * 8) + 1; //Почему то, если тут поставить дефайн константу, то всё рушится.
    //2 вариант
    //MemLen = BitLen / NUM_OF_BIT_IN_MEM;
    //MemLen++;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }        
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << n % (sizeof(TELEM) * 8);;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n >= BitLen) || (n < 0)) 
    { 
        throw "Bad value of the variable"; 
    }
    int index = GetMemIndex(n);
    pMem[index] = (GetMemMask(n) | pMem[index]);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n >= BitLen) || (n < 0)) 
    {
        throw "Bad value of the variable";
    }
    int index = GetMemIndex(n);
    pMem[index] = ((~GetMemMask(n)) & pMem[index]);//~ - инвертирование бита
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n >= BitLen) || (n < 0)) 
    { 
        throw "Bad value of the variable"; 
    }
    if (pMem[GetMemIndex(n)] & GetMemMask(n))// Битовая операция И, если 1 И 1, то истина, иначе ложь!
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение // Но здесь же можно тип Bool
{
    if (MemLen != bf.MemLen)
    {
        return 0;
    }
    else
    {
        int j = 0;
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                j++;
            }
        }
        if (j == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (MemLen != bf.MemLen)
    {
        return 1;
    }
    else
    {
        int j = 0;
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                j++;
                break;
            }
        }
        if (j == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int k,z,y;
    if (BitLen > bf.BitLen)
    {
        y = 0;
        k = BitLen;
        z = bf.BitLen;
    }
    else 
    {
        y = 1;
        k = bf.BitLen;
        z = BitLen;
    }
    TBitField temp(k);
    for (int i = 0; i <= GetMemIndex(z); i++) //где наименьшее, там битовое или, а после того, как наименьшая длина закончится, будем использовать наибольшую 
    {
        temp.pMem[i] = bf.pMem[i] | pMem[i];
    }
    for (int i = (GetMemIndex(z) + 1); i < temp.MemLen; i++)
    {
        if (y == 1)
        {
            temp.pMem[i] = bf.pMem[i];
        }
        else
        {
            temp.pMem[i] = pMem[i];
        }
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int k, z;
    if (BitLen > bf.BitLen) {
        k = BitLen;
        z = bf.BitLen;
    }
    else
    {
        k = bf.BitLen;
        z = BitLen;
    }
    TBitField temp(z); //Создаём с меньшей длинной, так как в остальных будут нули просто
    for (int i = 0; i <= GetMemIndex(z); i++)
    {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание //Все элементы за исклюяением последнего мы можем инвертировать с помощью ~, а последний нужно рассмотреть, ведь он может быть не весь заполнен, поэтому его прогоним по циклу
{
    TBitField temp(*this);
    for (int i = 0; i < (temp.MemLen - 1); i++) //Почеум MemLen - 1;
    {
        temp.pMem[i] = ~(temp.pMem[i]);
    }
    int i = (temp.MemLen - 1) * (sizeof(TELEM) * 8);
    while (i < temp.BitLen)
    {
        if (temp.GetBit(i) == 1)
        {
            temp.ClrBit(i);
        } 
        else
        {
            temp.SetBit(i);
        }
        i++;
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int k;
    int j;
    cout << "Enter lenght of the Bitlen" << endl;
    cin >> k;
    TBitField A(k);
    cout << "Enter the BitField" << endl;
    for (int i = 0; i < A.GetLength(); i++)
    {
        cin >> j;
        if (j == 1) // нули не нужно, так как они изначально уже там
        {
            A.SetBit(i);
        }
    }
    bf = A;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i) == 0)
        {
            ostr << 0;
        }
        else
        {
            ostr << 1;
        }
    }
    ostr << endl;
    return ostr;
}
