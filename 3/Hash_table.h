#pragma once

#include <vector>
#include <forward_list>
#include <functional>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <iterator> // Pour std::forward_iterator_tag

namespace pr
{

    template <typename K, typename V>
    class Hash_table
    {
    private:
        struct Entry
        {
            const K key;
            V value;

            Entry(const K &key_, const V &value_) : key(key_), value(value_) {}
        };

        std::vector<std::forward_list<Entry>> buckets;
        size_t cur_size;
        std::hash<K> hash_func;

        size_t get_bucket_index(const K &key) const
        {
            size_t hash_value = hash_func(key);
            return hash_value % buckets.size();
        }

        // O(n), où n est le nb d'elem ds la table
        void grow()
        {
            size_t new_nb_buckets = buckets.size() * 2;
            std::vector<std::forward_list<Entry>> new_buckets(new_nb_buckets);

            for (const auto &bucket : buckets)
            {
                for (const auto &entry : bucket)
                {
                    size_t new_index = hash_func(entry.key) % new_nb_buckets;
                    new_buckets[new_index].emplace_front(entry.key, entry.value);
                }
            }

            buckets = std::move(new_buckets);
        }

    public:
        // explicit pour éviter les conversions implicites comme on a un seul paramètre
        explicit Hash_table(size_t num_buckets = 100)
            : buckets(num_buckets), cur_size(0) {}

        V *get(const K &key)
        {
            size_t index = get_bucket_index(key);
            // on pourrait avoir une collision, càd plusieurs clés qui ont le même hash
            // cela est invvitable quand on a + d'entrées de clés que de buckets
            // UPDATE : avec grow() ce problème n'est plus d'actualité ds notre implé
            for (auto &entry : buckets[index])
            {
                if (entry.key == key)
                    return &(entry.value);
            }
            return nullptr;
        }

        bool put(const K &key, const V &value)
        {

            if (static_cast<double>(cur_size) / buckets.size() >= 0.8)
                grow();

            size_t index = get_bucket_index(key);
            for (auto &entry : buckets[index])
            {
                if (entry.key == key)
                {
                    entry.value = value;
                    return true;
                }
            }

            // si la clé n'existe pas, on la rajoute
            buckets[index].emplace_front(key, value);
            cur_size++;
            return false;
        }

        size_t size() const
        {
            return cur_size;
        }

        bool empty() const
        {
            return cur_size == 0;
        }

        std::vector<std::pair<K, V>> get_entries_vec() const
        {
            std::vector<std::pair<K, V>> entries;
            // reserve de la mémoire pour éviter les réallocations quand on ajoute des éléments
            entries.reserve(cur_size);

            for (const auto &bucket : buckets)
            {
                for (const auto &entry : bucket)
                    entries.emplace_back(entry.key, entry.value);
            }

            return entries;
        }

        class Iterator
        {
        public:
            // necessary type aliases for iterator_traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = Entry;
            using difference_type = std::ptrdiff_t;
            using pointer = Entry *;
            using reference = Entry &;

            Iterator() : outer_it_(), outer_end_(), inner_it_() {}

            // typename signifie que le type est dépendant du template
            Iterator(typename std::vector<std::forward_list<Entry>>::iterator outer_it,
                     typename std::vector<std::forward_list<Entry>>::iterator outer_end)
                : outer_it_(outer_it), outer_end_(outer_end)
            {
                if (outer_it_ != outer_end_)
                {
                    inner_it_ = outer_it_->begin();
                    advance_to_next_valid();
                }
            }

            reference operator*() const
            {
                return *inner_it_;
            }

            pointer operator->() const
            {
                // on prend le pointer-like object, le déréférence et on retourne l'adresse de l'objet
                return &(*inner_it_);
            }

            Iterator &operator++()
            {
                ++inner_it_;
                advance_to_next_valid();
                return *this;
            }

            // int est un dummy parameter pour différencier avec le version préfixe
            // on va incrémenter l'itérateur et retourner l'ancienne valeur
            Iterator operator++(int)
            {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator!=(const Iterator &other) const
            {
                return outer_it_ != other.outer_it_ ||
                       (outer_it_ != outer_end_ && other.outer_it_ != other.outer_end_ && inner_it_ != other.inner_it_);
            }

            bool operator==(const Iterator &other) const
            {
                return !(*this != other);
            }

        private:
            typename std::vector<std::forward_list<Entry>>::iterator outer_it_;
            typename std::vector<std::forward_list<Entry>>::iterator outer_end_;
            typename std::forward_list<Entry>::iterator inner_it_;

            void advance_to_next_valid()
            {
                while (outer_it_ != outer_end_ && inner_it_ == outer_it_->end())
                {
                    ++outer_it_;
                    if (outer_it_ != outer_end_)
                        inner_it_ = outer_it_->begin();
                }
            }
        };

        class ConstIterator
        {
        public:
            // necessary type aliases for iterator_traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = const Entry;
            using difference_type = std::ptrdiff_t;
            using pointer = const Entry *;
            using reference = const Entry &;

            ConstIterator() : outer_it_(), outer_end_(), inner_it_() {}

            ConstIterator(typename std::vector<std::forward_list<Entry>>::const_iterator outer_it,
                          typename std::vector<std::forward_list<Entry>>::const_iterator outer_end)
                : outer_it_(outer_it), outer_end_(outer_end)
            {
                if (outer_it_ != outer_end_)
                {
                    inner_it_ = outer_it_->begin();
                    advance_to_next_valid();
                }
            }

            reference operator*() const
            {
                return *inner_it_;
            }

            pointer operator->() const
            {
                return &(*inner_it_);
            }

            ConstIterator &operator++()
            {
                ++inner_it_;
                advance_to_next_valid();
                return *this;
            }

            ConstIterator operator++(int)
            {
                ConstIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator!=(const ConstIterator &other) const
            {
                return outer_it_ != other.outer_it_ ||
                       (outer_it_ != outer_end_ && other.outer_it_ != other.outer_end_ && inner_it_ != other.inner_it_);
            }

            bool operator==(const ConstIterator &other) const
            {
                return !(*this != other);
            }

        private:
            typename std::vector<std::forward_list<Entry>>::const_iterator outer_it_;
            typename std::vector<std::forward_list<Entry>>::const_iterator outer_end_;
            typename std::forward_list<Entry>::const_iterator inner_it_;

            void advance_to_next_valid()
            {
                while (outer_it_ != outer_end_ && inner_it_ == outer_it_->end())
                {
                    ++outer_it_;
                    if (outer_it_ != outer_end_)
                        inner_it_ = outer_it_->begin();
                }
            }
        };

        Iterator begin()
        {
            return Iterator(buckets.begin(), buckets.end());
        }

        Iterator end()
        {
            return Iterator(buckets.end(), buckets.end());
        }

        ConstIterator begin() const
        {
            return ConstIterator(buckets.begin(), buckets.end());
        }

        ConstIterator end() const
        {
            return ConstIterator(buckets.end(), buckets.end());
        }

        ConstIterator cbegin() const { return begin(); }
        ConstIterator cend() const { return end(); }
    };

    template <typename Iterator>
    size_t count(Iterator begin, Iterator end)
    {
        size_t cnt = 0;
        for (; begin != end; ++begin)
            ++cnt;

        return cnt;
    }

    template <typename Iterator, typename T>
    size_t count_if_equal(Iterator begin, Iterator end, const T &val)
    {
        size_t cnt = 0;
        for (; begin != end; ++begin)
        {
            if (begin->key == val)
                ++cnt;
        }
        return cnt;
    }

} // fin namespace pr
