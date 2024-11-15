/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MIT License                                                                     *
 *                                                                                 *
 * Copyright (c) 2024 Thomas AUBERT                                                *
 *                                                                                 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy    *
 * of this software and associated documentation files (the "Software"), to deal   *
 * in the Software without restriction, including without limitation the rights    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is           *
 * furnished to do so, subject to the following conditions:                        *
 *                                                                                 *
 * The above copyright notice and this permission notice shall be included in all  *
 * copies or substantial portions of the Software.                                 *
 *                                                                                 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 * SOFTWARE.                                                                       *
 *                                                                                 *
 * github : https://github.com/ThomasAUB/uflow                                   *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#pragma once

#include <tuple>
#include <cstddef>
#include <type_traits>

namespace uflow {

    template<typename ... args_t> struct Flow;

    template<typename ... args_t>
    struct INode {

        virtual bool operator () (args_t... a) = 0;

        auto& operator >> (INode<args_t...>& n) {
            next = &n;
            return n;
        }

    private:
        friend struct Flow<args_t...>;
        INode<args_t...>* next = nullptr;
    };


    template<typename ... args_t>
    struct Flow {

        bool operator () (args_t... args) {
            auto* n = first;
            while (n) {
                if (!(*n)(args...)) {
                    return false;
                }
                n = n->next;
            }
            return true;
        }

        template<typename ... targs_t>
        typename std::enable_if_t<
            (std::is_convertible_v<
                targs_t,
                std::remove_reference_t<args_t>
            > && ...),
            bool
        >
            operator () (targs_t&&... args) {
            auto* n = first;
            while (n) {
                if (!(*n)(args...)) {
                    return false;
                }
                n = n->next;
            }
            return true;
        }

        auto& operator >> (INode<args_t...>& node) {
            first = &node;
            return node;
        }

    private:
        INode<args_t...>* first = nullptr;
    };

    template<typename std::size_t count, typename ... args_t>
    struct Switch : INode<args_t...> {

        bool operator () (args_t... args) override {
            return mFlows[mSelect](args...);
        }

        auto& operator [] (std::size_t i) {
            return mFlows[i];
        }

        void select(std::size_t i) {
            if (i >= count) { return; }
            mSelect = i;
        }

    private:

        auto& operator >> (INode<args_t...>& n);

        Flow<args_t...> mFlows[count];
        std::size_t mSelect = 0;
    };


    template<std::size_t count, typename ... args_t>
    struct Fork : INode<args_t...> {

        auto& operator [] (std::size_t i) {
            return mFlows[i];
        }

    private:

        bool operator () (args_t... args) override {

            for (std::size_t i = 0; i < count; i++) {

                // copy the arguments so their values don't interfere on the fork branches
                std::tuple<std::decay_t<args_t>...> argcopy(args...);

                // process sub flows with the arg copy
                std::apply(
                    [&] (auto&& ... a) {
                        mFlows[i](a...);
                    },
                    argcopy
                );
            }
            return true;
        }

        Flow<args_t...> mFlows[count];
    };
}