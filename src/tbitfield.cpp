#include "tbitfield.h"


static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

// Конструктор
TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("Negative length");
    }

    BitLen = len;
    MemLen = (BitLen + (8 * sizeof(TELEM) - 1)) / (8 * sizeof(TELEM));  // Округление вверх
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));  // Обнуляем память
}

// Конструктор копирования
TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));  // Копируем данные
}

// Деструктор
TBitField::~TBitField() {
    delete[] pMem;
}

// Вспомогательные методы
int TBitField::GetMemIndex(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Out of bounds");
    }
    return n / (8 * sizeof(TELEM));  // Номер элемента массива pMem
}

TELEM TBitField::GetMemMask(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Out of bounds");
    }
    return 1 << (n % (8 * sizeof(TELEM)));  // Маска для бита
}

// Доступ к битам битового поля
int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;  // Установка бита
}

void TBitField::ClrBit(const int n) {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;  // Очистка бита
}

int TBitField::GetBit(const int n) const {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) ? 1 : 0;  // Возвращает 1, если бит установлен
}

// Битовые операции
TBitField& TBitField::operator=(const TBitField& bf) {
    if (this != &bf) {
        delete[] pMem;  // Освобождаем старую память
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));  // Копируем данные
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) return 0;  // Разные длины - не равны
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;  // Если хотя бы один элемент не равен
    }
    return 1;  // Все элементы равны
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);  // Не равно - это отрицание равенства
}

TBitField TBitField::operator|(const TBitField& bf) {
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;  // Выбираем максимальную длину
    TBitField temp(len);

    for (int i = 0; i < temp.MemLen; i++) {
        if (i < MemLen && i < bf.MemLen) {
            temp.pMem[i] = pMem[i] | bf.pMem[i];  // Объединяем элементы
        }
        else if (i < MemLen) {
            temp.pMem[i] = pMem[i];  // Копируем оставшиеся элементы
        }
        else {
            temp.pMem[i] = bf.pMem[i];  // Копируем оставшиеся элементы
        }
    }

    return temp;
}

TBitField TBitField::operator&(const TBitField& bf) {
    int maxSize = std::max(BitLen, bf.BitLen);
    TBitField result(maxSize);

    for (int i = 0; i < std::min(BitLen, bf.BitLen); i++) {
        if (GetBit(i) && bf.GetBit(i)) {
            result.SetBit(i);  // Устанавливаем бит, если оба бита установлены
        }
    }

    return result;
}

TBitField TBitField::operator~(void) {
    TBitField result(BitLen);

    for (int i = 0; i < BitLen; i++) {
        if (!GetBit(i)) {
            result.SetBit(i);  // Инвертируем биты
        }
    }

    return result;
}

// Ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        int bit;
        istr >> bit;
        if (bit == 1) {
            bf.SetBit(i);  // Устанавливаем бит
        }
        else {
            bf.ClrBit(i);  // Очищаем бит
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);  // Выводим значения битов
    }
    return ostr;
}