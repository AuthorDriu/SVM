#include "vm/include/vitrual_machine.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 1)
    {
        std::cout << "\033[31mYou should to point at source file\033[0m\n";
        exit(1);
    }

    VirtualMachine vm;
    
    int ern;
    for (int i = 1; i < argc; ++i)
    {
        vm.loadProgram(argv[i]);
        
        try{
            if(ern = vm.start())
            {
                std::cout << argv[i] << " finished with code " << ern << '\n';
            }
            else
            {
                std::cout << argv[i] << " program successfuly finished " << ern << '\n';
            }
        }
        catch (VMRuntimeError *err)
        {
            std::cout << err->what() << '\n';
        }
    }

    return 0;
}