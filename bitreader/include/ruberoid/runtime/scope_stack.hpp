#pragma once
#include <vector>
#include <algorithm>
#include <cassert>
#include "ruberoid/runtime/context.hpp"
#include "ruberoid/runtime/value.hpp"

namespace rb::runtime
{
    struct scope
    {
        context::ptr ctx;
        bool is_scope;
    };

    class scope_stack
    {
    public:
        //----------------------------------------------------------------------
        void push_context(context::ptr ctx)
        {
            _state.push_back({ctx, false});
        }

        //----------------------------------------------------------------------
        void push_scope(context::ptr ctx)
        {
            _state.push_back({ctx, true});
        }

        //----------------------------------------------------------------------
        value::ptr resolve(const std::string& name)
        {
            auto iter = find_scope([&](scope& s) -> bool {
                return s.ctx->has(name);
            });

            if (iter == _state.rend()) {
                return nullptr;
            }

            return iter->ctx->get(name);
        }

        //----------------------------------------------------------------------
        value::ptr resolve(const std::string& name, const std::string& type)
        {
            auto iter = find_scope([&](scope& s) -> bool {
                return s.ctx->has(name) && s.ctx->type() == type;
            });

            if (iter == _state.rend()) {
                return nullptr;
            }

            return iter->ctx->get(name);
        }

        //----------------------------------------------------------------------
        void pop_scope()
        {
            assert(!_state.empty() && _state.back().is_scope && "No scope in the stack!");
            _state.pop_back();
        }

        //----------------------------------------------------------------------
        void pop_context()
        {
            assert(!_state.empty() && "Stack is empty! Can't pop!");

            auto iter = find_scope([&](scope& s) -> bool {
                return !s.is_scope;
            });

            assert((iter != _state.rend()) && "Context scope not found!");
            _state.erase(iter.base()-1, _state.end());
        }

        //----------------------------------------------------------------------
        context::ptr current_scope()
        {
            if (_state.empty() || !_state.back().is_scope)
            {
                push_scope(std::make_shared<context>(""));
            }

            return _state.back().ctx;
            //assert(!_state.empty() && "Stack is empty! No current scope!");
        }

        //----------------------------------------------------------------------
        context::ptr current_context()
        {
            auto iter = find_scope([&](scope& s) -> bool {
                return !s.is_scope;
            });

            assert((iter != _state.rend()) && "No context found in stack!");
            return iter->ctx;
        }
    private:
        using scope_container = std::vector<scope>;
        using scope_iterator = scope_container::iterator;
        using scope_reverse_iterator = scope_container::reverse_iterator;

        template<typename Fn>
        scope_reverse_iterator find_scope(Fn f)
        {
            return std::find_if(_state.rbegin(), _state.rend(), f);
        }

        std::vector<scope> _state;
    };
}