#ifndef __AUTO_MAP_H
#define __AUTO_MAP_H

#include <memory>
#include <vector>
#include <iostream>
#include <boost/assert.hpp>

template <typename _KeyType, typename _ValType>
class auto_map
{
public:
    typedef _KeyType KeyType;
    typedef _ValType T;

    class auto_lvalue
    {
    public:

        auto_lvalue (T * & p) : _p (p) {}
        operator T * () const { return _p; }
        T * operator-> () const { return _p; }
        auto_lvalue & operator= (std::unique_ptr<T> ap)
        {
            delete _p;
            _p = ap.release ();
            return *this;
        }

    private:
        T * & _p;
    };

    class mappings
    {
        friend class auto_map;
    public:
        //mappings(KeyType k, std::unique_ptr<T> v) : key(k), val(v) { }
        mappings(KeyType k, T* v) : key(k), val(v) { }
        ~mappings() {
            if (val != nullptr) {
                delete val;
            }
        }

        KeyType key;
/*
        const T* getValue() {
            return val;
        }
*/
        std::unique_ptr<T> getValue() {
            std::unique_ptr<T> res(val);
            val = nullptr;
            return res;
            //return std::unique_ptr<T>(val);
        }

    protected:
        T *val;
    };

    typedef typename std::vector<mappings*>::iterator iterator;
    typedef typename std::vector<mappings*>::const_iterator const_iterator;
    typedef mappings* iterator_value;

public:
    auto_map() : _data() {}
    ~auto_map()
    {
    }

    auto_map& operator=(const auto_map& x)
    {
        _data = x._data;
        return *this;
    }

#if 0
    T const operator [] (KeyType i) const
    {
        iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                return *((*it).val->get());
            }
            ++it;
        }
        throw "Key not found";
    }

#else

    T const * operator [] (KeyType i) const
    {
        iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                return (*it).val;
            }
            /*if ((*it)->first == i) {
                return (*it)->second->get();
            }
            */
            ++it;
        }
        throw "Key not found";
    }
#endif

    auto_lvalue operator [] (KeyType i)
    {
        iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                return auto_lvalue((*it)->val);
            }
            ++it;
        }
        it = _data.insert(it, new mappings(i, new T()));
        return auto_lvalue ((*it)->val);
    }

    auto_lvalue operator [] (iterator i)
    {
        BOOST_ASSERT( i < _data.end() );
        return auto_lvalue((*i)->val);
    }

    auto_lvalue operator [] (const_iterator i) const
    {
        BOOST_ASSERT( i < _data.end() );
        return auto_lvalue((*i)->val);
    }

    std::unique_ptr<T> get(KeyType i)
    {
        iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                T *res = (*it)->val;
                (*it)->val = nullptr;
                _data.erase(it);
                return std::unique_ptr<T>(res);
            }
            ++it;
        }
        return std::unique_ptr<T>(new T());
    }

    iterator find(KeyType i)
    {
        iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                return it;
            }
            ++it;
        }
        return it;
    }

    const_iterator find(KeyType i) const
    {
        const_iterator it = _data.begin();
        while (it != _data.end()) {
            if ((*it)->key == i) {
                return it;
            }
            ++it;
        }
        return it;
    }


    iterator begin()
    {
        return _data.begin();
    }

    iterator end()
    {
        return _data.end();
    }

    const_iterator begin() const
    {
        return _data.cbegin();
    }

    const_iterator end() const
    {
        return _data.cend();
    }

    bool empty() const
    {
        return _data.empty();
    }

    size_t size() const
    {
        return _data.size();
    }

private:
    std::vector<mappings*> _data;
};

#endif  // __AUTO_MAP_H
