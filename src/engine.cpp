#pragma once
#include "engine.h"

namespace enact {
    template <typename IntegerT>
    IntegerT Engine::get_integer_constant(std::size_t offset) const 
        requires std::is_integral_v<IntegerT>
    {
        return *static_cast<IntegerT*>(get_constants() + offset);
    }

    const std::uint8_t* Engine::get_string_constant(std::size_t offset) const {
        return (const std::uint8_t*)(get_constants() + offset);
    }

    template <typename IntegerT=uint64_t>
    IntegerT Engine::get_heap_item(std::size_t offset) const
        requires std::is_integral_v<IntegerT>
    {
        return *static_cast<IntegerT*>(get_heap_data() + offset);
    }

    void Engine::extend_heap(std::size_t factor) {
        if (auto temp = std::realloc(get_heap_data(), get_heap_size() + factor); 
            temp
        ) {
            runtime_data.heap.data = static_cast<decltype(get_heap_data())>(
                temp
            );
            get_heap_size() += factor;
        }
        else {
            throw std::bad_alloc();
        }
    }

    void Engine::free_heap() {
        if (get_heap_data() != NULL) {
            std::free(get_heap_data());
            runtime_data.heap.data = NULL;
        }
    }

    void Engine::initialize_code(const std::uint8_t* code) {
        runtime_data.code.data = code;
        get_code_offset() = 0;
    }

    void Engine::initialize_constants(const uint8_t* constants) {
        runtime_data.constants = constants;
    }

    void Engine::initialize_heap(std::size_t heap_size=10) {
        runtime_data.heap.data = static_cast<decltype(get_heap_data())>(
            std::malloc(heap_size*sizeof(std::uint8_t))
        );
        get_heap_size() = heap_size;
    }

    void Engine::reset_stack() {
        get_stack().clear();
    }

    std::uint64_t Engine::pop_from_stack() {
        const std::uint64_t temp = get_stack().back();
        get_stack().pop_back();
        return temp;
    }

    void Engine::push_onto_stack(std::uint64_t value) {
        get_stack().push_back(value);
    }

    std::size_t Engine::get_stack_size() const {
        return get_stack().size();
    }

    std::uint64_t& Engine::get_stack_top() {
        return get_stack().back();
    }

    const std::uint64_t& Engine::get_stack_top() const {
        return get_stack().back();
    }

    template <typename T>
    T Engine::read_code() 
        requires requires {std::is_integral_v<T>; std::is_unsigned_v<T>; } 
    {
        const auto data = *static_cast<T*>(get_code() + get_code_offset());
        increment_code_offset(sizeof(T));
        return data;
    }

    const std::uint8_t* Engine::get_constants() const {
        return runtime_data.constants;
    }

    std::uint8_t* Engine::get_heap_data() {
        return runtime_data.heap.data;
    }

    std::size_t& Engine::get_heap_size() {
        return runtime_data.heap.size;
    }

    std::size_t& Engine::get_code_offset() {
        return runtime_data.code.offset;
    }

    void Engine::increment_code_offset(std::size_t amount) {
        get_code_offset() += amount;
    }

    std::vector<uint64_t>& Engine::get_stack() {
        return runtime_data.stack;
    }

    const std::uint8_t* Engine::get_heap_data() const {
        return runtime_data.heap.data;
    }

    const std::size_t& Engine::get_heap_size() const {
        return runtime_data.heap.size;
    }

    const std::uint8_t* Engine::get_code() const {
        return runtime_data.code.data;
    }

    const std::size_t& Engine::get_code_offset() const {
        return runtime_data.code.offset;
    }

    const std::vector<uint64_t>& Engine::get_stack() const {
        return runtime_data.stack;
    }

    int Engine::run(EngineInitializer initializer) {
        const auto initialize_all = [&]() {
            initialize_constants(initializer.constants);
            initialize_code(initializer.code);
            initialize_heap(initializer.heap_size);
        };

        initialize_all();

        // ...
        assert(0);
        return 0;
    }
}

namespace enact {
    Engine get_engine() {
        return Engine();
    }
}
