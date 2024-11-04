#pragma once
#include <string> // size_t
#include <cassert>

namespace pr
{
    /**
     * An optionally-bounded queue based on
     * linked nodes.
     * This queue orders elements FIFO (first-in-first-out).
     * The <em>head</em> of the queue is that element that has been on the
     * queue the longest time.
     * The <em>tail</em> of the queue is that element that has been on the
     * queue the shortest time. New elements
     * are inserted at the tail of the queue, and the queue retrieval
     * operations obtain elements at the head of the queue.
     *  Linked nodes are
     * dynamically created upon each insertion unless this would bring the
     * queue above capacity.
     *
     * @param <E> the type of elements held in this queue
     */
    template <typename E>
    class LinkedQueue
    {
        /**
         * Linked list node class/struct
         */
        struct Node
        {
            /** always nullptr for the head of the list */
            E *item;

            /**
             * One of:
             * - the real successor Node
             * - nullptr, meaning there is no successor (this is the last node)
             */
            Node *next;

            Node(E *x) : item(x), next(nullptr) {}
        };

        /** The capacity bound, or Integer.MAX_VALUE if none */
        const size_t capacity;
        /** Current number of elements */
        size_t count;
        /**
         * Head of linked list.
         * Invariant: head->item == nullptr
         */
        Node *head;
        /**
         * Tail of linked list.
         * Invariant: last->next == nullptr
         */
        Node *last;

        /**
         * Links node at end of queue.
         *
         * @param node the node
         */
        void enqueue(Node *node)
        {
            assert(last->next == nullptr);

            // TODO Question 1
            if (size() == 0)
            {
                last = node;
                head->next = last;
            }
            else
            {
                last->next = node;
                last = node;
            }
        }

        /**
         * Removes a node from head of queue.
         * @return the node
         */
        E *dequeue() // if called, queue not empty
        {
            assert(head->item == nullptr);

            // TODO Question 1
            Node *to_destroy = head;
            head = head->next;
            delete (to_destroy);
            E *itm = head->item;
            head->item = nullptr;

            // cas où on dequeue la last et que last devient == head
            // semble OK

            return itm;
        }

    public:
        /**
         * Creates a queue with the given (fixed) capacity.
         * @param capacity the capacity of this queue
         */
        LinkedQueue(size_t capacity) : capacity(capacity), count(0)
        {
            if (capacity <= 0)
                throw "IllegalArgumentException()";

            // TODO Question 1
            head = new Node(nullptr);
            last = head;
        }

        ~LinkedQueue()
        {
            // TODO Question 1
            while (take())
                continue;

            delete (head);
            head = nullptr;
            last = nullptr;
        }
        /**
         * Returns the number of elements in this queue.
         */
        size_t size() const
        {
            return count;
        }

        /**
         * Returns the number of additional elements that this queue can ideally
         * (in the absence of memory or resource constraints) accept without
         * blocking. This is always equal to the initial capacity of this queue
         * less the current size of this queue.
         */
        size_t remainingCapacity() const
        {
            return capacity - count;
        }

        /**
         * Inserts the specified element at the tail of this queue
         *
         * // TODO Question 2
         * or return false if full
         *
         */
        bool put(E *e)
        {
            if (e == nullptr)
                throw "NullPointerInserted()";

            if (remainingCapacity() == 0)
                return false;

            Node *node = new Node(e);
            enqueue(node);
            count++;
            return true;
        }

        /**
         * Takes and returns the element at the head of the queue
         *
         * // TODO Question 2
         * or return nullptr if empty.
         *
         */
        E *take()
        {
            if (size() == 0)
                return nullptr;

            E *x = dequeue();
            count--;
            return x;
        }

    }; // end class

} // namespace
