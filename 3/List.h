#pragma once

#include <cstddef>
#include <stdexcept>

namespace pr
{
    template <typename T>
    class List
    {
    private:
        struct Node
        {
            T data;
            Node *next;

            Node(const T &value, Node *nextNode = nullptr)
                : data(value), next(nextNode) {}
        };

        Node *head;

    public:
        List() : head(nullptr) {}

        ~List()
        {
            Node *cur = head;
            while (cur)
            {
                Node *tmp = cur->next;
                delete cur;
                cur = tmp;
            }
        }

        List(const List &other) : head(nullptr)
        {
            if (!other.head)
                head = nullptr;
            else
            {
                head = new Node(other.head->data);
                Node *curNxt = other.head->next;
                Node *curPrev = head;

                while (curNxt)
                {
                    curPrev->next = new Node(curNxt->data);
                    curPrev = curPrev->next;
                    curNxt = curNxt->next;
                }
            }
        }

        List &operator=(const List &other)
        {
            if (this == &other)
                return *this;

            Node *cur = head;
            while (cur)
            {
                Node *tmp = cur->next;
                delete cur;
                cur = tmp;
            }

            if (!other.head)
                head = nullptr;
            else
            {
                head = new Node(other.head->data);
                Node *curNxt = other.head->next;
                Node *curPrev = head;

                while (curNxt)
                {
                    curPrev->next = new Node(curNxt->data);
                    curNxt = curNxt->next;
                    curPrev = curPrev->next;
                }
            }
            return *this;
        }
        // O(1)
        void push_front(const T &val)
        {
            Node *newNode = new Node(val, head);
            head = newNode;
        }
        // O(n)
        void push_back(const T &val)
        {
            Node *newNode = new Node(val, nullptr);
            if (!head)
            {
                head = newNode;
                return;
            }

            Node *cur = head;
            while (cur->next)
                cur = cur->next;

            cur->next = newNode;
        }

        T &operator[](size_t index)
        {
            Node *cur = head;
            size_t curIndex = 0;

            while (cur)
            {
                if (curIndex == index)
                    return cur->data;

                cur = cur->next;
                curIndex++;
            }

            throw std::out_of_range("Index hors limites");
        }

        size_t size() const
        {
            size_t count = 0;
            Node *cur = head;

            while (cur)
            {
                count++;
                cur = cur->next;
            }

            return count;
        }

        bool empty() const
        {
            return head == nullptr;
        }

        class Iterator
        {
        private:
            Node *cur;

        public:
            Iterator(Node *node) : cur(node) {}

            T &operator*() const
            {
                if (!cur)
                    throw std::out_of_range("Déréférencement 'un itérateur invalide");
                return cur->data;
            }

            bool operator!=(const Iterator &other) const
            {
                return cur != other.cur;
            }

            Iterator &operator++()
            {
                if (cur)
                    cur = cur->next;
                return *this;
            }
        };

        Iterator begin()
        {
            return Iterator(head);
        }

        Iterator end()
        {
            return Iterator(nullptr);
        }

        class ConstIterator
        {
        private:
            const Node *cur;

        public:
            ConstIterator(const Node *node) : cur(node) {};

            const T &operator*() const
            {
                if (!cur)
                    throw std::out_of_range("Déréférencement d'un itérateur invalide");
                return cur->data;
            }

            bool operator!=(const ConstIterator &other) const
            {
                return cur != other.cur;
            }

            ConstIterator &operator++()
            {
                if (cur)
                    cur = cur->next;
                return *this;
            }
        };

        ConstIterator begin() const
        {
            return ConstIterator(head);
        }

        ConstIterator end() const
        {
            return ConstIterator(nullptr);
        }
    };

} // fin namespace pr