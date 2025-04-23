#ifndef MYHASHTABLE_H
#define MYHASHTABLE_H

#include <vector>
#include <optional>
#include <QDebug>

template<typename K, typename V>
class MyHashtable {
public:
    MyHashtable(size_t size = 16)
        : table(size), capacity(size), count(0) {}

    size_t findSlot(const K& key) const {
        size_t index = hash(key);
        size_t start = index;

        while (table[index].has_value() && table[index]->first != key) {
            index = (index + 1) % capacity;
            if (index == start) break;
        }
        return index;
    }

    void insert(const K& key, const V& value) {
        if ((float)count / capacity >= 0.7f) {
            rehash(capacity * 2);
        }

        size_t index = findSlot(key);
        if (!table[index].has_value()) {
            count++;
        }
        table[index] = std::make_pair(key, value);
    }

    std::optional<V> get(const K& key) const {
        size_t index = findSlot(key);
        if (table[index].has_value() && table[index]->first == key)
            return table[index]->second;
        return std::nullopt;
    }

    bool remove(const K& key) {
        size_t index = findSlot(key);
        if (table[index].has_value() && table[index]->first == key) {
            table[index].reset();
            count--;
            return true;
        }
        return false;
    }

    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].has_value()) {
                qDebug() << "ALO[" << i << "] " << table[i]->first << ": " << table[i]->second;
            }
        }
    }

private:
    std::vector<std::optional<std::pair<K, V>>> table;
    size_t capacity;
    size_t count;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    void rehash(size_t newCapacity) {
        std::vector<std::optional<std::pair<K, V>>> oldTable = std::move(table);
        table = std::vector<std::optional<std::pair<K, V>>>(newCapacity);
        capacity = newCapacity;
        count = 0;

        for (auto& item : oldTable) {
            if (item.has_value()) {
                insert(item->first, item->second);
            }
        }
    }
};

#endif // MYHASHTABLE_H
