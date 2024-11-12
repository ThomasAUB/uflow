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


namespace uflow {

    template<typename ... args_t>
    struct Flow;

    template<typename ... args_t>
    struct INode {
        virtual bool operator()(args_t... a) = 0;
    private:
        friend struct Flow<args_t...>;
        INode<args_t...>* mNextNode = nullptr;
    };


    template<typename ... args_t>
    struct Flow : INode<args_t...> {

        using node_t = INode<args_t...>;

        bool operator()(args_t... args) override {
            node_t* n = mFirst;
            while (n) {
                if (!(*n)(args...)) {
                    return false;
                }
                n = n->mNextNode;
            }
            return true;
        }

        auto& operator >> (node_t& node) {
            if (!mFirst) {
                mFirst = &node;
            }
            else {
                node_t* n = mFirst;
                while (n->mNextNode) {
                    n = n->mNextNode;
                }
                n->mNextNode = &node;
            }
            return *this;
        }

    private:
        node_t* mFirst = nullptr;
    };


}