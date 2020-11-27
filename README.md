# Intel 8080
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![C/C++ CI](https://github.com/temaweb/8080/workflows/C/C++%20CI/badge.svg)

Эмулятор процессора [Intel 8080](https://en.wikipedia.org/wiki/Intel_8080). Аналог в СССР: [КР580ВМ80А](https://en.wikipedia.org/wiki/KR580VM80A)

* Дата анонса: апрель 1974 года
* Тактовая частота: 2 МГц (позже 2,5 МГц, 3,1 МГц и 4 МГц)
* Разрядность регистров: 8 бит
* Разрядность шины данных: 8 бит
* Разрядность шины адреса: 16 бит
* Объём адресуемой памяти: 64 Кбайт
* Поддерживаемые технологии: 80 инструкций

## API

Эмулятор позволяет подключать объекты, реализующие шаблон IO<T> для обмена данными с памятью или устройствами. 
    
```cpp
template<class T>
class IO
{
public:
    virtual uint8_t read(T address) const = 0;
    virtual void write(T address, uint8_t data) = 0;
};
```

Шаблон может быть реализован как `uint16_t` для обмена данным с RAM или как `uint8_t` для обработки данных от инструкций `IN` и `OUT`. Обратите внимание, что во втором случае индекс устройства будет `uint8_t`

```cpp
void connect (std::shared_ptr<IO<uint16_t>> bus);
void connect (std::shared_ptr<IO<uint8_t>>  io);
```

Пример реализации RAM

```cpp
class Ram : public IO<uint16_t>
{
private:
    
    // 64 KB
    std::array<uint8_t, 64 * 1024> ram {};
    
public:
    virtual uint8_t read(uint16_t address) const override final {
        return ram[address];
    }
    
    virtual void write(uint16_t address, uint8_t data) override final {
        ram[address] = data;
    }
};
```

Подключение

```cpp
auto ram = std::make_shared<Ram>();
cpu -> connect(ram);
```

### Установка и получение программного счетчика

```cpp
void setCounter(uint16_t counter);
uint16_t getCounter();
```

Метод `setCounter(uint16_t counter)` устанавливает программный счетчик на адрес `counter`. После этого процессор начнет обрабатывать операции начиная с этого адреса:

```cpp
cpu -> setCounter(0xF800);

while (cpu -> getCounter() > 0)
{
    // Читает команды с 0xF800 пока программный счетчик не
    // встретит команду NOP (0x00)
    
    cpu -> clock();  
}
```

### Запуск

Запуск и сброс осуществляются при помощи методов `clock()` и `reset()` 

```cpp
void clock();
void reset();
```

Каждая инструкция выполняется за определенное количество циклов (_clock cycles_). Один вызов `clock()` эмулирует один такт. Например, для выполнения операции `MOV R,R` необходимо вызвать `clock()` 5 раз. Причем, операция будет выполнена в первый вызов, а последующие нужны для эмуляции нужного числа тактов.

### Недокументированные операции

Эмулятор обрабатывает недокументированные операции

```asm
0xCB: JMP  00 00
0xD9: RET
0xDD: CALL 00 00
0xED: CALL 00 00
0xFD: CALL 00 00
```

### Нереализованные операции

В эмуляторе отсутствует поддержка установки флага прерывания и поддержка спящего режима.

```asm
0x03: DI   
0xFB: EI
0x76: HLT
```

## Отладка

Если собрать приложение с ключем `ASMLOG`, то оно будет логировать операции

```
0x3A0A CD E1 32 $0E  CALL A:57 B:00 C:04 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FF3
0x32E1 0E 02          MVI A:57 B:00 C:02 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FF3
0x32E3 CD BD 20 $F5  CALL A:57 B:00 C:02 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FF1
0x20BD F5            PUSH A:57 B:00 C:02 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FEF
0x20BE 0E 02          MVI A:57 B:00 C:02 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FEF
0x20C0 CD 05 00 $D3  CALL A:57 B:00 C:02 D:00 E:57 H:00 L:00 $00 $00 $D3  S=0  Z=0  AC=1  P=0  C=0  0x2FED
```

Последнии колонка — указатель стека.

## Компиляция и запуск

По-умолчанию, код собирается с ключем `LOGTEST` для чтения результатов прохождения тестов. Для сборки потребуется компилятор с поддержкой C++ 14.

```shell
$ make run
g++ -std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST Example.cpp
g++ -std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST Cpu.cpp
g++ -std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST Status.cpp
g++ -std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST Asmlog.cpp
g++ -lstdc++ Example.o Cpu.o Status.o Asmlog.o -o cpu
```

## Диагностика

После запуска, приложение выполняет несколько тестов для проверки работоспособности процессора. 
Модифицированный 8080EXM.com это тренажер (exerciser) Яна Бартоломеева и Александра Демина, который выполняет циклически избыточный код ([CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)) и генерирует некоторое значение на основе выполнения группы операций процессора. Полученное значение сравнивается со значением, полученным на реальном процессоре. 

[8080/8085 CPU Exerciser](https://altairclone.com/downloads/cpu_tests/8080_8085%20CPU%20Exerciser.pdf)
    
### 8080

8080/8085 CPU Diagnostic, version 1.0, by Microcosm Associates (Kelly Smith)

    MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC VERSION 1.0  (C) 1980

    CPU IS OPERATIONAL
    
### CPUTEST

Тест процессора SuperSoft Associates из пакета Diagnostic II
    
    DIAGNOSTICS II V1.2 - CPU TEST
    COPYRIGHT (C) 1981 - SUPERSOFT ASSOCIATES

    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    CPU IS 8080/8085
    BEGIN TIMING TEST
    END TIMING TEST
    CPU TESTS OK
    
### 8080PRE

Предварительный тест для тренажера 8080EXM (Ian Bartholomew and Frank Cringles)

    8080 Preliminary tests complete
    
### 8080EXM

    8080 instruction exerciser

    dad <b,d,h,sp>................  PASS! crc is:14474ba6
    aluop nn......................  PASS! crc is:9e922f9e
    aluop <b,c,d,e,h,l,m,a>.......  PASS! crc is:cf762c86
    <daa,cma,stc,cmc>.............  PASS! crc is:bb3f030c
    <inr,dcr> a...................  PASS! crc is:adb6460e
    <inr,dcr> b...................  PASS! crc is:83ed1345
    <inx,dcx> b...................  PASS! crc is:f79287cd
    <inr,dcr> c...................  PASS! crc is:e5f6721b
    <inr,dcr> d...................  PASS! crc is:15b5579a
    <inx,dcx> d...................  PASS! crc is:7f4e2501
    <inr,dcr> e...................  PASS! crc is:cf2ab396
    <inr,dcr> h...................  PASS! crc is:12b2952c
    <inx,dcx> h...................  PASS! crc is:9f2b23c0
    <inr,dcr> l...................  PASS! crc is:ff57d356
    <inr,dcr> m...................  PASS! crc is:92e963bd
    <inx,dcx> sp..................  PASS! crc is:d5702fab
    lhld nnnn.....................  PASS! crc is:a9c3d5cb
    shld nnnn.....................  PASS! crc is:e8864f26
    lxi <b,d,h,sp>,nnnn...........  PASS! crc is:fcf46e12
    ldax <b,d>....................  PASS! crc is:2b821d5f
    mvi <b,c,d,e,h,l,m,a>,nn......  PASS! crc is:eaa72044
    mov <bcdehla>,<bcdehla>.......  PASS! crc is:10b58cee
    sta nnnn / lda nnnn...........  PASS! crc is:ed57af72
    <rlc,rrc,ral,rar>.............  PASS! crc is:e0d89235
    stax <b,d>....................  PASS! crc is:2b0471e9

    Tests complete
