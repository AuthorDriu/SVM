/*
    указатели (типы):
        стек данных, блок данных, блок кода - unsigned short (65_536)
        стек возврата - unsigned char (256) 
*/

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <string>
#include "error.hpp"
#include "instructions.hpp"

// Определение размеров памяти в ячейках
#define DATA_STACK_MEMORY_SIZE          65536
#define DATA_SEGMENT_MEMORY_SIZE        65536
#define RET_STACK_MEMORY_SIZE           256
#define CODE_SEGMENT_MEMORY_SIZE        65536
#define VIRTUAL_MACHINE_MEMORY_SIZE     196864

//  Начало/конец сегментов
#define CODE_SEGMENT_START              196863
#define CODE_SEGMENT_END                131327
#define DATA_SEGMENT_START              0
#define DATA_SEGMENT_END                65535
#define DATA_STACK_END                  131327
#define DATA_STACK_START                65791
#define RET_STACK_END                   65791
#define RET_STACK_START                 65596

#define INPUT_BUFFER_SIZE               1028

class VirtualMachine
{
public:
    VirtualMachine();
    ~VirtualMachine();

    void loadProgram(char*);
    void clear();

    int start();

private:
    int *memory;
    
    unsigned int SP; // стековый указателб
    unsigned int IP; // Командный указатель
    unsigned int RP; // Возвратный указатель

    void moveSP(int);
    void moveIP(int);
    void moveRP(int);

    void write(int, int); // pointer, data
    int read(int); // pointer

    void dataPush(int);
    int dataPop();

    void retPush(int);
    int retPop();

    int getArg();
};

#endif