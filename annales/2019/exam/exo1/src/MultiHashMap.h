#pragma once

#include <forward_list>
#include <vector>
#include <mutex>
#include <atomic>

namespace pr
{

	template <typename K, typename V>
	class MHashMap
	{

	public:
		class Entry
		{
		public:
			const K key;
			V value;
			Entry(const K &k, const V &v) : key(k), value(v) {}
		};

	private:
		typedef std::vector<std::forward_list<Entry>> buckets_t;
		// stockage pour la table de buckets
		buckets_t buckets;
		// nombre total d'entrées dans la table
		std::atomic<size_t> sz;

		mutable vector<std::mutex> mtx_list;

	public:
		MHashMap(size_t size) : mtx_list(size), buckets(size), sz(0)
		{
			// le ctor buckets(size) => size cases, initialisées par défaut.
		}

		V *get(const K &key)
		{
			size_t h = std::hash<K>()(key);
			size_t target = h % buckets.size();
			std::unique_lock<std::mutex> lock(mtx_list[target]);
			for (Entry &ent : buckets[target])
			{
				if (ent.key == key)
				{
					return &ent.value;
				}
			}
			return nullptr;
		}

		bool put(const K &key, const V &value)
		{
			size_t h = std::hash<K>()(key);
			size_t target = h % buckets.size();
			std::unique_lock<std::mutex> lock(mtx_list[target]);
			for (Entry &ent : buckets[target])
			{
				if (ent.key == key)
				{
					ent.value = value;
					return true;
				}
			}
			sz++;
			buckets[target].emplace_front(key, value);
			return false;
		}

		size_t size() const { return sz; }
	};

} /* namespace pr */
