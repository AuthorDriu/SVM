#ifndef VM_ERROR_H
#define VM_ERROR_H

#include <stdexcept>
#include <string>

class VMRuntimeError : public std::runtime_error
{
public:
    VMRuntimeError():std::runtime_error("ERROR WAS OCCERED WHILE EXECUTING THE PROGRAM"){}
    VMRuntimeError(std::string msg):std::runtime_error(msg.c_str()){}
};

class VMLoadError : public std::runtime_error
{
public:
    VMLoadError():std::runtime_error("ERROR WAS OCCURED WHILE LOADING THE PROGRAM\n"){}
    VMLoadError(std::string msg):std::runtime_error(msg.c_str()){}
};

#endif