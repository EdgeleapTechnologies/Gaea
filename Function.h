#include "Types.h"

#include <stdlib.h>

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
    LambdaCallHelper m_call_helper;
    void* m_function_type_storage;

public:
    template<typename Callable>
    Function(const Callable& callable)
    {
        this->m_function_type_storage = malloc(sizeof(Callable));
        memcpy(this->m_function_type_storage, &callable, sizeof(Callable));

        this->m_call_helper = [](const Function& function, Parameters... parameters) -> Return
        {
            const Callable* callable_function = (const Callable*)function.m_function_type_storage;
            return (*callable_function)(move(parameters...));
        };
    }

    Return operator()(Parameters... parameters)
    {
        return this->m_call_helper(*this, move(parameters...));
    }

};

}