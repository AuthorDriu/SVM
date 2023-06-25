#include "include/vitrual_machine.hpp"
#include <iostream>
#include <fstream>
#include <curses.h>

//#define DEBUG_LOG

void debug_log(std::string message)
{
#ifdef DEBUG_LOG
    std::cout << "\033[33mDEBUG_LOG\033[0m: " << message;
#endif
}

bool isEndOfCode(const unsigned int &p, const unsigned char *sc)
{
    unsigned char op = sc[p];
    if ((int)op == HALT) return 1;
    else return 0;
}

int getInt(unsigned int &p, const unsigned char *content)
{
    debug_log(std::string("picking... "));
    int res = 0;
    res += (int)content[++p] << 24;
#ifdef DEBUG_LOG
    std::cout << res << ' ';
#endif
    res += (int)content[++p] << 16;
#ifdef DEBUG_LOG
    std::cout << res << ' ';
#endif
    res += (int)content[++p] << 8;
#ifdef DEBUG_LOG
    std::cout << res << ' ';
#endif
    res += (int)content[++p];
#ifdef DEBUG_LOG
    std::cout << res << '\n';
#endif

    debug_log(std::string("picked ") + std::to_string(res) + std::string("\n"));
    return res;
}

short argumentRequired(const unsigned int &p, const unsigned char *content)
{
    debug_log("verifying\n");
    unsigned char op = content[p];
    if (
        op == ADD || op == SUB || op == MUL || op == DIV || op == POP ||
        op == RET || op == INP_I || op == INP_C || op == OUT_C || op == OUT_I ||
        op == NEWL || op == NEWL || op == SPACE
    ) return 0;
    else if (
        op == JUMP || op == JUMP_IF_ZERO || op == JUMP_IF_NEG || op == CALL ||
        op == PUSH || op == WRITE || op == LOAD
    ) return 1;
    else throw VMRuntimeError(std::string("ERROR OCCURED WHILE LOADING THE PROGRAM\nUnknown operator on ") + std::to_string(p) + std::string(" byte ") + std::to_string((int)op) + std::string("\n"));
}

VirtualMachine::VirtualMachine()
{
    memory = new int[VIRTUAL_MACHINE_MEMORY_SIZE];
    
    SP = DATA_STACK_START;
    IP = CODE_SEGMENT_START;
    RP = RET_STACK_START;
}

VirtualMachine::~VirtualMachine()
{
    if (memory != nullptr)
    {
        delete [] memory;
        memory = nullptr;
    }
}

void VirtualMachine::clear()
{
    for (short i = 0; i < VIRTUAL_MACHINE_MEMORY_SIZE; ++i)
        memory[i] = 0;
    
    SP = DATA_STACK_START;
    IP = CODE_SEGMENT_START;
    RP = RET_STACK_START;
}

void VirtualMachine::loadProgram(char* source_filename)
{
    debug_log(std::string("start loading\n"));

    unsigned char* sc; // source content
    unsigned int size; // source size

    std::fstream source(source_filename, std::ios::in | std::ios::binary);
    if ( !source.is_open() )
    {
        throw VMRuntimeError(std::string("Cannot open the file <") + std::string(source_filename) + std::string(">\n"));
    }

    // Узнаю размер файла
    //source.seekg(std::ios::end);
    //size = source.tellg();
    //source.seekg(std::ios::beg);

    // Считывание файла в буфер
    sc = new unsigned char[CODE_SEGMENT_START - CODE_SEGMENT_END];
    source >> sc;

    debug_log(std::string("source size ") + std::to_string(size) + std::string("\n"));
    debug_log(std::string());
#ifdef DEBUG_LOG
    for (int i = 0; i < size; ++i)
        std::cout << std::hex << (int)sc[i] << ' ';
    std::cout << '\n';
#endif

    // Закрытие файла
    source.close();

    // запись файла в память ВМ
    unsigned int p = 0; // указатель на первый байт
    unsigned int command = 0;
    while (true)
    {
        if (isEndOfCode(p, sc))
        {
            debug_log(std::string("end of loading\n"));
            break;
        }

        debug_log(std::string("\033[31mloading\033[0m ") + std::to_string(command) + std::string("\n"));
        memory[IP] = (int)sc[p];
        debug_log(std::string("command "));
#ifdef DEBUG_LOG
        std::cout << std::hex << (int)sc[p] << '\n';
#endif
        if (argumentRequired(p, sc))
        {
            moveIP(1);
            memory[IP] = getInt(p, sc);
        }
        moveIP(1);
        debug_log(std::string("\033[32mloaded\033[0m [") + std::to_string(p) + std::string("]\n"));
        ++p;
        ++command;
#ifdef DEBUG_LOG
        std::cout << '\n';
#endif
    }
    memory[IP] = (int)PUSH;
    moveIP(1);
    memory[IP] = 0;
    moveIP(1);
    memory[IP] = (int)HALT;
    debug_log("loaded the program\n");
}

int VirtualMachine::getArg()
{
    moveIP(1);
    return memory[IP];
}

int VirtualMachine::start()
{
    IP = CODE_SEGMENT_START;
    SP = DATA_STACK_START;
    RP = RET_STACK_START;

    int x, y;
    unsigned char op;
    char c;
    debug_log(std::string("start execution:\n"));
    while (memory[IP] != (int)HALT)
    {
        debug_log("next: IP " + std::to_string(IP) + std::string("\tSP " + std::to_string(SP) + std::string("\n")));

        op = (unsigned char)memory[IP];
        switch (op)
        {
            
            case ADD:
                x = dataPop();
                y = dataPop();
                dataPush(x + y);
            break;

            case SUB:
                x = dataPop();
                y = dataPop();
                dataPush(y - x);
            break;

            case MUL:
                x = dataPop();
                y = dataPop();
                dataPush(x * y);
            break;

            case DIV:
                x = dataPop();
                y = dataPop();
                dataPush(y / x);
            break;

            case JUMP:
                x = getArg();
                moveIP(x - 1);
            break;

            case JUMP_IF_ZERO:
                x = dataPop();
                y = getArg();
                if (x == 0)
                {
                    moveIP(y - 1);
                }
            break;
            
            case JUMP_IF_NEG:
                x = dataPop();
                y = getArg();
                if (x < 0)
                {
                    moveIP(y - 1);
                }
                    
            break;

            case CALL:
                x = getArg();
                retPush(IP);
                moveIP(x - 2);
            break;

            case RET:
                x = retPop();
                IP = x;
            break;

            case INP_I:
                std::cin >> x;
                dataPush(x);
            break;

            case OUT_I:
                x = dataPop();
                std::cout << x;
            break;

            case INP_C:
                std::cin >> c;
                dataPush((int)c);
            break;

            case OUT_C:
                x = dataPop();
                std::cout << (char)x;
            break;

            case NEWL:
                dataPush((int)'\n');
            break;

            case ENDL:
                dataPush(0);
            break;

            case SPACE:
                dataPush(32);
            break;

            case PUSH:
                x = getArg();
                dataPush(x);
            break;

            case POP:
                dataPop();
            break;

            case WRITE:
                x = dataPop();
                y = getArg();
                write(y, x);
            break;

            case LOAD:
                x = getArg();
                dataPush(read(x));
            break;

            default:
                throw VMRuntimeError();
            break;
        }
        moveIP(1);
    }
    moveIP(-1);
    return memory[IP];
}

void VirtualMachine::moveSP(int step)
{
    SP += step;
    if (DATA_STACK_END < SP || SP < DATA_STACK_START)
        throw VMRuntimeError(std::string("Data stack overflow [") + std::to_string(step) + std::string("]\n"));
}

void VirtualMachine::moveIP(int step)
{
    debug_log(std::string("IP move ") + std::to_string(step) + std::string("\n"));
    IP -= step;
    if (CODE_SEGMENT_END > IP || IP > CODE_SEGMENT_START + 1)
        throw VMRuntimeError(std::string("Code pointer is out [") + std::to_string(step) + std::string("]\n"));
}

void VirtualMachine::moveRP(int step)
{
    RP += step;
    if (RET_STACK_END < RP || RP < RET_STACK_START)
        throw VMRuntimeError(std::string("Returt stack overflow [") + std::to_string(step) + std::string("]\n"));
}

void VirtualMachine::write(int address, int value)
{
    if (DATA_SEGMENT_END < address || address < DATA_SEGMENT_START)
        throw VMRuntimeError(std::string("Data segment overflow [") + std::to_string(address) + std::string("]\n"));
    memory[address] = value;
}

int VirtualMachine::read(int address)
{
    if (DATA_SEGMENT_END < address || address < DATA_SEGMENT_START)
        throw VMRuntimeError(std::string("Data segment overflow [") + std::to_string(address) + std::string("]\n"));
    return memory[address];
}

void VirtualMachine::dataPush(int value)
{
    moveSP(1);
    memory[SP] = value;
}

int VirtualMachine::dataPop()
{
    int x = memory[SP];
    moveSP(-1);
    return x;
}

void VirtualMachine::retPush(int pos)
{
    moveRP(1);
    memory[RP] = pos;
}

int VirtualMachine::retPop()
{
    int x = memory[RP];
    moveRP(-1);
    return x;
}