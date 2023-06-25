#include <fstream>

int main(void)
{
    std::fstream f("simple_test", std::ios::out);
    if (!f.is_open())
    {
        return 1;
    }

    unsigned char program[] = " olleH\0      "; // 10
    program[0] = (unsigned char)0b00010001; // PUSH_STR
    program[7] = (unsigned char)0b00001110; // OUT_STR
    program[8] = (unsigned char)0b11111111; // END
    program[9] = (unsigned char)0;
    program[10] = (unsigned char)0;
    program[11] = (unsigned char)0;
    program[12] = (unsigned char)0;
    program[13] = (unsigned char)0b10000000;

    for (int i = 0; i < 18; ++i)
    {
        f << program[i];
    }
    f.close();

    return 0;
}