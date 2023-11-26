#pragma once

#include <Types.h>
#include <Memory.h>

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

    template<typename Callable>
    Function(const Callable& callable)
    {
        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = GlobalAllocate(this->m_funtion_size);
        memcpy(this->m_function_type_storage, &callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return
        {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };
    }

    ~Function()
    {
        if(this->m_function_type_storage != nullptr)
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
        this->m_function_type_storage = malloc(this->m_funtion_size);
        memcpy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);
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

    template<typename Callable>
    Function& operator=(const Callable& callable)
    {
        if(this == &callable) return *this;
        this->~Function();

        this->m_funtion_size = sizeof(callable);
        this->m_function_type_storage = GlobalAllocate(this->m_funtion_size);
        memcpy(this->m_function_type_storage, &callable, this->m_funtion_size);

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return
        {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters)...);
        };

        return *this;
    }

    Function& operator=(const Function& other)
    {
        if(this == &other) return *this;
        this->~Function();

        this->m_call_helper = other.m_call_helper;
        this->m_funtion_size = other.m_funtion_size;
        this->m_function_type_storage = malloc(this->m_funtion_size);
        memcpy(this->m_function_type_storage, other.m_function_type_storage, this->m_funtion_size);

        return *this;
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

    Return operator()(Parameters... parameters)
    {
        return this->m_call_helper(*this, move(parameters)...);
    }

};

}
