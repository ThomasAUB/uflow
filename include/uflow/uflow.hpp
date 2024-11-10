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

#include "ulink.hpp"

namespace uflow {

    template<typename ... args_t>
    struct INode : ulink::Node<INode<args_t...>> {
        virtual bool operator()(args_t&... a) = 0;
    };

    template<typename ... args_t>
    struct BasicNode final : INode<args_t...> {

        BasicNode(bool(*f)(args_t& ... args)) : mF(f) {}

        bool operator()(args_t&... a) override {
            return mF(a...);
        }

    private:
        bool(*mF)(args_t& ... args);
    };

    template<typename ... args_t>
    struct Flow : INode<args_t...> {

        using node_t = INode<args_t...>;

        template<typename ... nodes_t>
        Flow(nodes_t&... nodes) {
            append(nodes...);
        }

        void append(node_t& node) {
            mNodes.push_back(node);
        }

        template<typename ... nodes_t>
        void append(nodes_t&... nodes) {
            (mNodes.push_back(nodes), ...);
        }

        void insert(std::size_t idx, node_t& node) {
            std::size_t counter = 0;
            for (auto& n : mNodes) {
                if (counter++ == idx) {
                    mNodes.insert_before(&n, node);
                }
            }
        }

        void operator()(args_t&&... args) {
            for (auto& n : mNodes) {
                if (!n(args...)) {
                    return;
                }
            }
        }

        bool operator()(args_t&... args) override {
            for (auto& n : mNodes) {
                if (!n(args...)) {
                    break;
                }
            }
            return true;
        }

        auto& operator >> (node_t& node) {
            append(node);
            return *this;
        }

    private:
        ulink::List<node_t> mNodes;
    };


    template<std::size_t count, typename ... args_t>
    struct Fork : INode<args_t...> {

        Flow<args_t...>& operator[](std::size_t i) {
            return mSubFlows[i];
        }

        template<std::size_t i>
        Flow<args_t...>& get() {
            static_assert(i < count, "Invalid fork index");
            return mSubFlows[i];
        }

    private:

        bool operator()(args_t&... args) override {

            for (std::size_t i = 0; i < count; i++) {
                // copy the arguments : should this be on the stack ?
                mArgsCopy = std::make_tuple(args...);

                // process sub flows with the arg copy
                callFlow(mSubFlows[i], std::make_index_sequence<sizeof...(args_t)>{});
            }

            return true;
        }

        template<std::size_t ... Is>
        void callFlow(Flow<args_t...>& f, const std::index_sequence<Is...>&) {
            f(std::get<Is>(mArgsCopy)...);
        }

        std::tuple<args_t...> mArgsCopy;
        Flow<args_t...> mSubFlows[count];
    };

    template<std::size_t count, typename ... args_t>
    struct Switch : INode<args_t...> {

        Flow<args_t...>& operator[](std::size_t i) {
            return mSubFlows[i];
        }

        template<std::size_t i>
        Flow<args_t...>& get() {
            static_assert(i < count, "Invalid fork index");
            return mSubFlows[i];
        }

        void set(std::size_t i) {
            mIndex = i;
        }

        bool operator()(args_t&... args) override {
            if (mIndex >= count) { return false; }
            return mSubFlows[mIndex](args...);
        }

    private:
        std::size_t mIndex = count;
        Flow<args_t...> mSubFlows[count];
    };

}