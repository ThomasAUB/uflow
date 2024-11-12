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

        template<typename node_t>
        auto& operator>>(node_t& n) {
            mNextNode = &n;
            return n;
        }

    private:
        friend struct Flow<args_t...>;
        INode<args_t...>* mNextNode = nullptr;
    };


    template<typename ... args_t>
    struct Flow {

        bool operator()(args_t&&... args) {
            INode<args_t...>* n = mFirst;
            while (n) {
                if (!(*n)(args...)) {
                    return false;
                }
                n = n->mNextNode;
            }
            return true;
        }

        auto& operator >> (INode<args_t...>& node) {
            mFirst = &node;
            return node;
        }

    private:
        INode<args_t...>* mFirst = nullptr;
    };

    template<typename std::size_t count, typename ... args_t>
    struct Switch : INode<args_t...> {

        bool operator()(args_t... args) override {
            return mFlows[mSelect](args...);
        }

        auto& operator[](std::size_t i) {
            return mFlows[i];
        }

        void select(std::size_t i) {
            if (i >= count) { return; }
            mSelect = i;
        }

    private:
        Flow<args_t...> mFlows[count];
        std::size_t mSelect = 0;
    };


    template<std::size_t count, typename ... args_t>
    struct Fork : INode<args_t...> {

        auto& operator[](std::size_t i) {
            return mFlows[i];
        }

    private:

        bool operator()(args_t&... args) override {

            std::tuple<args_t...> argcopy;

            for (std::size_t i = 0; i < count; i++) {
                // copy the arguments : should this be on the stack ?
                argcopy = std::make_tuple(args...);
                // process sub flows with the arg copy
                callFlow(mFlows[i], argcopy, std::make_index_sequence<sizeof...(args_t)>{});
            }
            return true;
        }

        template<std::size_t ... Is>
        void callFlow(Flow<args_t...>& f, std::tuple<args_t...>& args, const std::index_sequence<Is...>&) {
            f(std::get<Is>(args)...);
        }

        Flow<args_t...> mFlows[count];

    };
}