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
    Allocator* m_allocator = nullptr;
    size_t m_funtion_size = 0;

public:
    Function() = default;

    Function(Return (*function)(Parameters...), Allocator* allocator = &SystemAllocator::Instance())
    {
        //NOTE(Tiago):for now the way we will handle static funciton pointers by keeping the
        //function size variable at 0, despite the storage variable not being null. This
        //combination of flags will be used to denote that the pointer the storage variable
        //is directly a function pointer and not a lamba storage pointer.
        this->m_allocator = allocator;
        this->m_function_type_storage = (void*)function;
        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            using callable_function_type = Return (*)(Parameters...);
            callable_function_type callable = (callable_function_type)function.m_function_type_storage;
            return (*callable)(move(parameters)...);
        };
    }

    Function& operator=(Return (*function)(Parameters...))
    {
        this->~Function();

        this->m_function_type_storage = (void*)function;
        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            using callable_function_type = Return (*)(Parameters...);
            callable_function_type callable = (callable_function_type)function.m_function_type_storage;
            return (*callable)(move(parameters)...);
        };

        return *this;
    }

    template<typename Callable>
    Function(const Callable& callable, Allocator* allocator = &SystemAllocator::Instance())
    {
        this->m_allocator = allocator;
        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = this->m_allocator->Allocate(this->m_funtion_size);
        MemCopy((void*)this->m_function_type_storage, (void*)&callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };
    }

    template<typename Callable>
    Function& operator=(const Callable& callable)
    {
        if(this == &callable) return *this;
        this->~Function();

        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = this->m_allocator->Allocate(this->m_funtion_size);
        MemCopy(this->m_function_type_storage, &callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };

        return *this;
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

        this->m_allocator = other.m_allocator;
        this->m_call_helper = other.m_call_helper;
        this->m_funtion_size = other.m_funtion_size;
        if(this->m_funtion_size != 0 && other.m_function_type_storage != nullptr)
        {
            this->m_function_type_storage = this->m_allocator->Allocate(this->m_funtion_size);
            MemCopy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);
        }
        else { this->m_function_type_storage = other.m_function_type_storage; }
    }

    Function& operator=(const Function& other)
    {
        if(this == &other) return *this;
        this->~Function();

        this->m_allocator = other.m_allocator;
        this->m_call_helper = other.m_call_helper;
        this->m_funtion_size = other.m_funtion_size;
        if(this->m_function_type_storage != 0 && other.m_function_type_storage != nullptr)
        {
            this->m_function_type_storage = this->m_allocator->Allocate(this->m_funtion_size);
            MemCopy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);
        }
        else { this->m_function_type_storage = other.m_function_type_storage; }

        return *this;
    }

    Function(Function&& other)
    {
        if(this == &other) return;
        this->~Function();

        this->m_allocator = other.m_allocator;
        other.m_allocator = nullptr;

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

        this->m_allocator = other.m_allocator;
        other.m_allocator = nullptr;

        this->m_funtion_size = other.m_funtion_size;
        other.m_funtion_size = 0;

        this->m_function_type_storage = other.m_function_type_storage;
        other.m_function_type_storage = nullptr;

        this->m_call_helper = other.m_call_helper;
        other.m_call_helper = nullptr;

        return *this;
    }

    Return operator()(Parameters... parameters)
    {
        if(this->m_function_type_storage == nullptr) { return Return(); }
        return this->m_call_helper(*this, move(parameters)...);
    }
};

}
