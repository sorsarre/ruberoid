#include <ruberoid/data_source/memory_byte_source.hpp>
#include <ruberoid/runtime/program_parser.hpp>
#include <ruberoid/runtime/execution.hpp>

#include <iostream>
#include <map>
#include <typeinfo>
#include <typeindex>

template<typename T>
void print_specific(const std::any& value)
{
    std::cout << std::any_cast<T>(value);
}

template<>
void print_specific<uint8_t>(const std::any& value)
{
    std::cout << static_cast<uint32_t>(std::any_cast<uint8_t>(value));
}

template<>
void print_specific<int8_t>(const std::any& value)
{
    std::cout << static_cast<uint32_t>(std::any_cast<uint8_t>(value));
}

void print_arithmetic(const std::any& value)
{
    using printer_fun_type = std::function<void(const std::any&)>;
    using printer_map_type = std::unordered_map<std::type_index, printer_fun_type>;

    static printer_map_type printers = {
            {typeid(uint8_t), print_specific<uint8_t>},
            {typeid(uint16_t), print_specific<uint16_t>},
            {typeid(uint32_t), print_specific<uint32_t>},
            {typeid(uint64_t), print_specific<uint64_t>},
            {typeid(int8_t), print_specific<int8_t>},
            {typeid(int16_t), print_specific<int16_t>},
            {typeid(int32_t), print_specific<int32_t>},
            {typeid(int64_t), print_specific<int64_t>}
    };

    auto iter = printers.find(value.type());
    if (iter != printers.end()) {
        iter->second(value);
    } else {
        std::cout << "[unknown]";
    }
}

void print(rb::runtime::context::ptr ctx)
{
    std::cout << "[CONTEXT BEGIN]" << std::endl;
    std::cout << "type: " << ctx->type() << std::endl;
    for (const auto& pair: *ctx) {
        std::cout << "field: " << pair.first << " ";
        rb::runtime::value::ptr value = pair.second->get_value();
        if (value && *value) {
            if (value->context()) {
                std::cout << "context";
                std::cout << std::endl;
                print(value->get<rb::runtime::context::ptr>());
            }

            if (value->array()) {
                std::cout << "array";
            }

            if (value->arithmetic()) {
                std::cout << "arithmetic ";
                print_arithmetic(value->get());
            }

            if (value->string()) {
                std::cout << "string";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "[CONTEXT END]" << std::endl;
}

rb::runtime::execution_context play()
{
    rb::runtime::raw_program raw = {
            { "ctxc", "BinaryTestEntry" },
            { "slotf", "version" },
            { "brdu8", "version", "8" },
            { "slotd" },
            { "bskp", "4" },
            { "slotf", "coeff" },
            { "brdu16", "coeff", "12" },
            { "slotd" },
            { "ctxf" }
    };

    uint8_t data[] = { 0x12, 0x56, 0x89 };
    rb::common::bitreader br;
    br.set_data(data, sizeof(data));

    auto program = rb::runtime::program_parser::parse(raw);

    rb::runtime::execution_context ctx;
    rb::runtime::execution exec;
    exec.exec(program, br, ctx);

    for (auto context: ctx._context_stack.state()) {
        print(context.ctx);
    }

    return ctx;
}

int main(int, char**)
{
    std::cout << "Start playing..." << std::endl;
    play();
    std::cout << "Done playing..." << std::endl;
}