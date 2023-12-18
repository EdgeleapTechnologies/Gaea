#pragma once

#include <Core/Types.h>
#include <Core/Memory.h>

namespace Edgeleap
{

//generic that never gets used
template<typename Return, typename... Parameters>
class Function;

//function specialization
template<typename Return, typename... Parameters>
class Function<Return(Parameters...)>
{
private:
    using LambdaCallHelper = Return (*)(const Function&, Parameters...);
    LambdaCallHelper m_call_helper = nullptr;
    void* m_function_type_storage = nullptr;
    size_t m_funtion_size = 0;

public:
    Function() = default;

    Function(Return (*function)(Parameters...))
    {
        //NOTE(Tiago):for now the way we will handle static funciton pointers by keeping the
        //function size variable at 0, despite the storage variable not being null. This
        //combination of flags will be used to denote that the pointer the storage variable
        //is directly a function pointer and not a lamba storage pointer.
        this->m_function_type_storage = (void*)function;
        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            using callable_function_type = Return (*)(Parameters...);
            callable_function_type callable = (callable_function_type)function.m_function_type_storage;
            return (*callable)(move(parameters)...);
        };
    }

    template<typename Callable>
    Function(const Callable& callable)
    {
        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = GlobalAllocate(this->m_funtion_size);
        memcpy(this->m_function_type_storage, &callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };
    }

    ~Function()
    {
        if(this->m_function_type_storage != nullptr && this->m_funtion_size != 0)
        {
            GlobalFree(this->m_function_type_storage);
            this->m_function_type_storage = nullptr;
        }
        this->m_call_helper = nullptr;
        this->m_funtion_size = 0;
    }

    Function(const Function& other)
    {
        if(this == &other) return;
        this->~Function();

        this->m_call_helper = other.m_call_helper;
        this->m_funtion_size = other.m_funtion_size;
        if(this->m_funtion_size != 0 && other.m_function_type_storage != nullptr)
        {
            this->m_function_type_storage = malloc(this->m_funtion_size);
            memcpy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);
        }
        else { this->m_function_type_storage = other.m_function_type_storage; }
    }

    Function& operator=(const Function& other)
    {
        if(this == &other) return *this;
        this->~Function();

        this->m_call_helper = other.m_call_helper;
        this->m_funtion_size = other.m_funtion_size;
        if(this->m_function_type_storage != 0 && other.m_function_type_storage != nullptr)
        {
            this->m_function_type_storage = malloc(this->m_funtion_size);
            memcpy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);
        }
        else { this->m_function_type_storage = other.m_function_type_storage; }

        return *this;
    }

    Function(Function&& other)
    {
        if(this == &other) return;
        this->~Function();

        this->m_funtion_size = other.m_funtion_size;
        other.m_funtion_size = 0;

        this->m_function_type_storage = other.m_function_type_storage;
        other.m_function_type_storage = nullptr;

        this->m_call_helper = other.m_call_helper;
        other.m_call_helper = nullptr;
    }

    Function& operator=(Function&& other)
    {
        if(this == &other) return *this;
        this->~Function();

        this->m_function_type_storage = other.m_function_type_storage;
        other.m_function_type_storage = nullptr;

        this->m_call_helper = other.m_call_helper;
        other.m_call_helper = nullptr;

        return *this;
    }

    template<typename Callable>
    Function& operator=(const Callable& callable)
    {
        if(this == &callable) return *this;
        this->~Function();

        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = GlobalAllocate(this->m_funtion_size);
        memcpy(this->m_function_type_storage, &callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };

        return *this;
    }

    Return operator()(Parameters... parameters)
    {
        if(this->m_function_type_storage == nullptr) { return Return(); }
        return this->m_call_helper(*this, move(parameters)...);
    }
};

}
