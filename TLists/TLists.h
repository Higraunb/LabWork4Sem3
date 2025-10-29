#include <initializer_list>
#include <ostream>
using namespace std;

template<class T>
class TLists
{
private:
    T* data;
    size_t* right;
    size_t* left;

    size_t head;
    size_t tail;
    size_t capacity;
    size_t size;
    int64_t free_list;
    size_t GetHead() const;
    size_t GetSize() const;
    size_t GetCapacity() const;
    size_t GetTail() const;
    size_t GetFreeList() const;
public:
    TLists();
    TLists(size_t capacity_);
    TLists(const TLists<T>& other);
    TLists(TLists<T>&& other);
    TLists(initializer_list<T> other);
    ~TLists();
    class TIterator 
    {
    private:
        TLists<T>* list;
        size_t current;

    public:
        TIterator() : list(nullptr), current(0) {}
        TIterator(TLists<T>* l, size_t pos) : list(l), current(pos) {}

        T& operator*() { return list->data[current]; }
        T* operator->() { return &(list->data[current]); }

        TIterator& operator++() {
            current = list->right[current];
            return *this;
        }

        TIterator operator++(int) {
            TIterator tmp = *this;
            current = list->right[current];
            return tmp;
        }

        TIterator& operator--() {
            current = list->left[current];
            return *this;
        }

        TIterator operator--(int) {
            TIterator tmp = *this;
            current = list->left[current];
            return tmp;
        }

        bool operator==(const TIterator& other) const {
            return list == other.list && current == other.current;
        }

        bool operator!=(const TIterator& other) const {
            return !(*this == other);
        }
        size_t getCurrent() const { return current; }
    };
    TIterator begin() { return TIterator(this, head); }
    TIterator end() { return TIterator(this, tail); }

    TLists<T> operator+(const TLists<T>& other);
    TLists<T>& operator=(const TLists<T>& other);
    TLists<T>& operator=(TLists<T>&& other);
    bool operator == (const TLists<T>& other);
    bool operator != (const TLists<T>& other);
    
    TIterator Insert(TIterator pos, const size_t& value);

    void push_back(const T& value);
    void push_front(const T& value);

    bool isEmpty() const;
    bool isFull() const;
    template <class O>
    friend ostream& operator<<(ostream& os, const TLists<O>& list);
};

template <class T>
inline size_t TLists<T>::GetHead() const
{
    return head;
}

template <class T>
inline size_t TLists<T>::GetSize() const
{
    return size;
}

template <class T>
inline size_t TLists<T>::GetCapacity() const
{
    return capacity;
}

template <class T>
inline size_t TLists<T>::GetTail() const
{
    return tail;
}

template <class T>
inline size_t TLists<T>::GetFreeList() const
{
    return free_list;
}

template <class T>
inline TLists<T>::TLists()
{
    data = nullptr;
    right = nullptr;
    left = nullptr;
    capacity = 0;
    head = 0;
    size = 0;
    tail = 0;
    free_list = 0;
}

template <class T>
inline TLists<T>::TLists(size_t capacity_)
{
    capacity = capacity_;
    data = new T[capacity]{};
    right = new size_t[capacity]{};
    left = new size_t[capacity]{};
    head = 0;
    size = 0;
    tail = capacity - 1;
    free_list = 0;
}

template <class T>
inline TLists<T>::TLists(const TLists<T> &other)
{
    if(other.capacity != 0)
    {
        capacity =  other.capacity;
        head = other.head;
        size = other.size;
        tail = other.tail;
        free_list = other.free_list;
        data = new T[capacity]{};
        right = new size_t[capacity]{};
        left = new size_t[capacity]{};
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
            right[i] = other.right[i];
            left[i] = other.left[i];
        }
    }
    else
    {
        data = nullptr;
        right = nullptr;
        left = nullptr;
        capacity = 0;
        head = 0;
        size = 0;
        tail = 0;
        free_list = 0;    
    }
}

template <class T>
inline TLists<T>::TLists(TLists<T> &&other)
{
     if(other.capacity != 0)
    {
        capacity =  other.capacity;
        head = other.head;
        size = other.size;
        tail = other.tail;
        free_list = other.free_list;
        data = new T[capacity]{};
        right = new size_t[capacity]{};
        left = new size_t[capacity]{};
        data = other.data;
        right = other.right;
        left = other.left;
    }
    else
    {
        data = nullptr;
        right = nullptr;
        left = nullptr;
        capacity = 0;
        head = 0;
        size = 0;
        tail = 0;
        free_list = 0;    
    }
    other.data = nullptr;
    other.right = nullptr;
    other.left = nullptr;
    other.capacity = 0;
    other.free_list = 0;
    other.head = 0;
    other.tail = 0; 
}

template <class T>
inline TLists<T>::TLists(initializer_list<T> other)
{
    capacity = other.size();
    data = new T[capacity]{};
    right = new size_t[capacity]{};
    left = new size_t[capacity]{};
    
    size = capacity;
    head = 0;
    tail = size - 1;
    
    size_t i = 0;
    for (const T& item : other)
    {
        data[i] = item;
        if (i > 0)
            left[i] = i - 1;
        if (i < size - 1)
            right[i] = i + 1;
        i++;
    }
    
    if (size > 0)
    {
        left[0] = size - 1;
        right[size - 1] = 0;
    }
}

template <class T>
inline TLists<T>::~TLists()
{
    delete[] data;
    delete[] right;
    delete[] left;
}

template <class T>
inline TLists<T> &TLists<T>::operator=(const TLists<T> &other)
{
    if (this != &other)
    {
        delete[] data;
        delete[] right;
        delete[] left;

        if(other.capacity != 0)
        {
            capacity =  other.capacity;
            head = other.head;
            size = other.size;
            tail = other.tail;
            free_list = other.free_list;
            data = new T[capacity]{};
            right = new size_t[capacity]{};
            left = new size_t[capacity]{};
            for (size_t i = 0; i < size; i++)
            {
                data[i] = other.data[i];
                right[i] = other.right[i];
                left[i] = other.left[i];
            }
        }
        else
        {
            data = nullptr;
            right = nullptr;
            left = nullptr;
            capacity = 0;
            head = 0;
            size = 0;
            tail = 0;
            free_list = 0;    
        }
    }
    return *this;
}

template <class T>
inline TLists<T> &TLists<T>::operator=(TLists<T> &&other)
{
    if (this != &other)
    {
        if(other.capacity != 0)
        {
            capacity =  other.capacity;
            head = other.head;
            size = other.size;
            tail = other.tail;
            free_list = other.free_list;
            data = other.data;
            right = other.right;    
            left = other.left;
        }
        else
        {
            data = nullptr;
            right = nullptr;
            left = nullptr;
            capacity = 0;
            head = 0;
            size = 0;
            tail = 0;
            free_list = 0;    
        }
        other.data = nullptr;
        other.right = nullptr;
        other.left = nullptr;
        other.capacity = 0;
        other.free_list = 0;    
        other.head = 0;
        other.tail = 0;
    }
    return *this;
}

template <class T>
inline TIterator TLists<T>::Insert(TIterator pos, const size_t& value)
{
    if (isFull())
        throw std::out_of_range("List is full");
    
    size_t new_index = free_list;
    free_list = right[free_list];
    size_t position = pos.getCurrent();
    if(position > size)
        throw std::out_of_range("Invalid position");
    data[new_index] = value;
    right[new_index] = right[position];
    left[new_index] = left[position];
    right[left[position]] = new_index;
    left[right[position]] = new_index;
    if(position == tail)
        tail = new_index;
    size++;
    return TIterator(this, new_index);
}

template <class T>
inline void TLists<T>::push_back(const T &value)
{
    Insert(size, value);
}

template <class T>
inline void TLists<T>::push_front(const T &value)
{
    Insert(value, 0);
}

template <class T>
inline bool TLists<T>::isEmpty() const
{
    return size == 0;
}

template <class T>
inline bool TLists<T>::isFull() const
{
    return capacity == size;
}

template <class T>
inline bool TLists<T>::operator==(const TLists<T> &other)
{
    if(other.size != size)
    {
        return false;
    }
    size_t current = head;
    size_t other_current = other.head;
    if (size == 0)
        return true;

    for (size_t i = 0; i < size; i++)
    {
        if (data[current] != other.data[other_current])
            return false;
        current = right[current];
        other_current = other.right[other_current];
    }
    return true;
}

template <class T>
inline bool TLists<T>::operator!=(const TLists<T> &other)
{
    return !(other == *this);
}

template <class O>
inline ostream& operator<<(ostream &os, const TLists<O> &list)
{
    if (list.size == 0) {
        os << "{}";
        return os;
    }

    os << "{";
    size_t current = list.head;
    for (size_t i = 0; i < list.size; i++) {
        os << list.data[current];
        if (i < list.size - 1)
            os << ", ";
        current = list.right[current];
    }
    os << "}";
    return os;
}
