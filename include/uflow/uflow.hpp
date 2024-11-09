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
    struct Flow : INode<args_t...> {

        template<typename ... nodes_t>
        Flow(nodes_t&... nodes) {
            append(nodes...);
        }

        void append(INode<args_t...>& node) {
            mNodes.push_back(node);
        }

        template<typename ... nodes_t>
        void append(nodes_t&... nodes) {
            (mNodes.push_back(nodes), ...);
        }

        void insert(std::size_t idx, INode<args_t...>& node) {
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
                    return false;
                }
            }
            return true;
        }

    private:
        ulink::List<INode<args_t...>> mNodes;
    };

}