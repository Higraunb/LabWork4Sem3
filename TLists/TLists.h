#include <initializer_list>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <stdexcept>

using namespace std;

template <typename T>
class TLists;

template <typename T>
class TListsNode
{
private:
    T* elem;
    TListsNode<T>* next;
    TListsNode<T>* prev;
public:
    TListsNode() : elem(nullptr), next(this), prev(this) {}
    explicit TListsNode(const T& value) : elem(new T(value)), next(this), prev(this) {};
    TListsNode(const T& value, TListsNode<T>* nextNode, TListsNode<T>* prevNode)
        : elem(new T(value)), next(nextNode), prev(prevNode) {}
    TListsNode(const TListsNode<T>& other)
    {
        if(other.elem == nullptr)
            elem = nullptr;
        else
        {
            elem = new T(*(other.elem));
        }
        next = other.next;
        prev = other.prev;
    }
    TListsNode(TListsNode<T>&& other) noexcept : elem(other.elem), next(other.next), prev(other.prev)
    {
        other.elem = nullptr;
        other.next = nullptr;
        other.prev = nullptr;
    }
    ~TListsNode()
    {
        if(elem)
            delete elem;
    }
    TListsNode<T>& operator=(const TListsNode<T>& other)
    {
        if(this != &other)
        {
            if(elem != nullptr)
                delete elem;
            if(other.elem == nullptr)
                elem = nullptr;
            else
                elem = new T(*(other.elem));
            next = other.next;
            prev = other.prev;
        }
        return *this;
    }
    TListsNode<T>& operator=(TListsNode<T>&& other) noexcept
    {
        if(this != &other)
        {
            if(elem != nullptr)
                delete elem;
            elem = other.elem;
            next = other.next;
            prev = other.prev;
        }
        return *this;
    }

    T* GetElem() const {return elem;}
    TListsNode<T>* GetNext() const {return next;}
    TListsNode<T>* GetPrev() const {return prev;}

    void SetElem(const T& value)
    {
        if(elem != nullptr)
            delete elem;
        elem = new T(value);
    }
    void SetNext(TListsNode<T>* nextNode) {next = nextNode;}
    void SetPrev(TListsNode<T>* prevNode) {prev = prevNode;}    

};

template<typename T>
class TIterator
{
private:
    TListsNode<T> * current;
public:
    TIterator(TListsNode<T> * node = nullptr): current(node){}
    
    T& operator*() const { return *(current->GetElem()); }
    T* operator->() const { return current->GetElem(); }

    TIterator& operator++() { current = current->GetNext(); return *this; }
    TIterator& operator--() { current = current->GetPrev(); return *this; }

    TIterator operator++(int) { TIterator tmp = *this; current = current->GetNext(); return tmp; }
    TIterator operator--(int) { TIterator tmp = *this; current = current->GetPrev(); return tmp; }

    bool operator!=(const TIterator& other) const { return current != other.current; }
    bool operator==(const TIterator& other) const { return current == other.current; }

    TListsNode<T>* GetCurrent() const { return current; }
};

template<typename T>
class ConstTIterator
{
private:
    TListsNode<T> * current;
public:
    ConstTIterator(TListsNode<T> * node = nullptr): current(node){}
    
    const T& operator*() const { return *(current->GetElem()); }
    const T* operator->() const { return current->GetElem(); }

    ConstTIterator& operator++() { current = current->GetNext(); return *this; }
    ConstTIterator& operator--() { current = current->GetPrev(); return *this; }

    ConstTIterator operator++(int) { ConstTIterator tmp = *this; current = current->GetNext(); return tmp; }
    ConstTIterator operator--(int) { ConstTIterator tmp = *this; current = current->GetPrev(); return tmp; }

    bool operator!=(const ConstTIterator& other) const { return current != other.current; }
    bool operator==(const ConstTIterator& other) const { return current == other.current; }

    TListsNode<T>* GetCurrent() const { return current; }
};

template <typename T>
class TLists
{
private:
    TListsNode<T>* head;
    size_t size;

    void Incert(TListsNode<T>* node, TListsNode<T>* pos);
    T Remove(TListsNode<T>* node);

public:
    TLists();
    TLists(size_t size_);
    TLists(TLists<T>& other);
    TLists(TLists<T>&& other) noexcept;
    TLists (std::initializer_list<T> ilist);
    
    TIterator<T> begin() noexcept;
    ConstTIterator<T> begin() const noexcept;
    TIterator<T> end() noexcept;
    ConstTIterator<T> end() const noexcept;
    
    TLists<T>& operator = (const TLists<T>& other);
    TLists<T>& operator = (TLists<T>&& other);

    bool operator == (const TLists<T>& other);
    bool operator != (const TLists<T>& other);

    T& operator[](size_t index);
    void IncertAfter(TIterator<T> node, const T& value);
    T RemoveElem(TIterator<T> node);
    
    void PushBack(const T& value);
    void PushFront(const T& value);
    T PopFront();
    T PopBack();

    bool isEmpty() const;
    bool isFull() const;
    void SetSize(size_t newSize);

    size_t GetSize() const ;

    size_t FindCountInputElem(const T& val);
    vector<size_t> FindInputElem(const T& val);
    template<typename Func>
    void LamdaFunc(Func lambda);
    
    void SaveToFile(const string& filename) const;
    void LoadFromFile(const string& filename);
    
    template<typename O>
    friend ostream& operator << (ostream & out, const TLists<O>& other);
    template<typename O>
    friend istream& operator >>(istream & inp, const TLists<O>& other);
    ~TLists();
};

template <typename T>
inline TLists<T>::TLists()
{
    head = new TListsNode<T>();
    head->SetNext(head);
    head->SetPrev(head);
    size = 0;
}

template <typename T>
inline TLists<T>::TLists(size_t size_) 
{
    size = size_;
    head = new TListsNode<T>();
    head->SetNext(head);
    head->SetPrev(head);
}

template <typename T>
inline TLists<T>::TLists(TLists<T> &other)
{
    if(this != &other)
    {
        size = other.size;
        head = new TListsNode(other.head);
    }
}

template <typename T>
inline TLists<T>::TLists(TLists<T> &&other) noexcept
{
    if(this != &other)
    {
        size = other.size;
        head = new TListsNode(other.head);
    }
}

template <typename T>
inline TLists<T>::TLists(std::initializer_list<T> ilist)
{
  head = new TListsNode<T>();
  head->SetNext(head);
  head->SetPrev(head);
  size = 0;

  for (const auto& item : ilist) {
    PushBack(item);
  }
}

template <typename T>
inline TIterator<T> TLists<T>::begin() noexcept
{
    return TIterator<T>(head->GetNext());
}

template <typename T>
inline ConstTIterator<T> TLists<T>::begin() const noexcept
{
    return ConstTIterator<T>(head->GetNext());
}

template <typename T>
inline TIterator<T> TLists<T>::end() noexcept
{
    return TIterator<T>(head);
}

template <typename T>
inline ConstTIterator<T> TLists<T>::end() const noexcept
{
    return ConstTIterator<T>(head);
}

template <typename T>
inline TLists<T> &TLists<T>::operator=(const TLists<T> &other)
{  
    if(this != &other)
    {
        if(head)
            delete[] head;
        head = new TListsNode(other.head);
        size = other.size;
    }
    return *this;
}

template <typename T>
inline TLists<T> &TLists<T>::operator=(TLists<T> &&other)
{
    if(this != &other)
    {
        head = new TListsNode(other.head);
        size = other.size;
        other.head = nullptr;
        other.size = 0;
    }
    return *this;
}

template <typename T>
inline bool TLists<T>::operator==(const TLists<T> &other)
{
    if(other.size == size)
    {
        auto it1 = begin();
        auto it2 = other.begin();
        while(it1 != end() && it2 != other.end())
        {
            if(*it1 != *it2)
                return false;
            ++it1;
            ++it2;
        }
        return true;
    }
    return false;
}

template <typename T>
inline bool TLists<T>::operator!=(const TLists<T> &other)
{
    return !(*this == other);
}

template <typename T>
inline void TLists<T>::Incert(TListsNode<T> *node, TListsNode<T>* pos)
{
    node->SetNext(pos);
    node->SetPrev(pos->GetPrev());
    pos->GetPrev()->SetNext(node);
    pos->SetPrev(node);
    size++;
}

template <typename T>
inline T TLists<T>::Remove(TListsNode<T> *node)
{
    T value = *(node->GetElem());
    node->GetPrev()->SetNext(node->GetNext());
    node->GetNext()->SetPrev(node->GetPrev());
    delete node;
    size--;
    return value;
}

template <typename T>
inline T &TLists<T>::operator[](size_t index)
{
    if(index >= size)
        throw std::out_of_range("Index out of range");
    TListsNode<T>* current = head->GetNext();
    for(size_t i = 0; i < index; i++)   
        current = current->GetNext();
    return *(current->GetElem());
}

template <typename T>
inline void TLists<T>::IncertAfter(TIterator<T> node, const T &value)
{
    Incert(new TListsNode<T>(value), node.GetCurrent()->GetNext());
}

template <typename T>
inline T TLists<T>::RemoveElem(TIterator<T> node)
{
    return Remove(node.GetCurrent());
}

template <typename T>
inline void TLists<T>::PushBack(const T &value)
{
    Incert(new TListsNode<T>(value), head);
}

template <typename T>
inline void TLists<T>::PushFront(const T &value)
{
    Incert(new TListsNode<T>(value), head->GetNext());
}

template <typename T>
inline T TLists<T>::PopFront()
{
    if(isEmpty())
        throw std::out_of_range("List is empty");
    return Remove(head->GetNext());
}

template <typename T>
inline T TLists<T>::PopBack()
{
    if(isEmpty())
        throw std::out_of_range("List is empty");
    return Remove(head->GetPrev());
}

template <typename T>
inline bool TLists<T>::isFull() const
{
    return false;
}

template <typename T>
inline size_t TLists<T>::GetSize() const
{
    return size;
}

template <typename T>
inline size_t TLists<T>::FindCountInputElem(const T &val)
{
    size_t cout = 0;
    for (auto it = begin(); it != end(); ++it)
    {
        if(val == *it)
            cout++;
    }
    return cout;
}

template <typename T>
inline vector<size_t> TLists<T>::FindInputElem(const T &val)
{
    size_t i = 0;
    vector<size_t> res;
    for (auto it = begin(); it != end(); ++it)
    {
        if(val == *it)
            res.push_back(i);
        i++;
    }
    return vector<size_t>(res);
}

template <typename T>
inline bool TLists<T>::isEmpty() const
{
    return size == 0;
}

template <typename T>
inline void TLists<T>::SetSize(size_t newSize)
{
    if (newSize < size)
    {
        TListsNode<T>* current = head->GetNext();
        size_t currentSize = 0;
        
        while (currentSize < newSize)
        {
            current = current->GetNext();
            currentSize++;
        }
        
        TListsNode<T>* last = head->GetPrev();
        while (current != head)
        {
            TListsNode<T>* next = current->GetNext();
            delete current;
            current = next;
        }
        
        last = head->GetPrev();
        last->SetNext(head);
        head->SetPrev(last);
    }
    size = newSize;
}

template <typename T>
template<typename Func>
inline void TLists<T>::LamdaFunc(Func lambda)
{
    if (isEmpty())
        return;
        
    TListsNode<T>* current = head->GetNext();
    while (current != head)
    {
        lambda(*(current->GetElem()));
        current = current->GetNext();
    }
}

template <typename T>
inline void TLists<T>::SaveToFile(const string& filename) const
{
    ofstream outFile(filename, ios::binary);
    if (!outFile)
        throw runtime_error("Could not open file for writing: " + filename);

    // Записываем размер списка
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

    // Записываем каждый элемент
    TListsNode<T>* current = head->GetNext();
    while (current != head)
    {
        T element = *(current->GetElem());
        outFile.write(reinterpret_cast<const char*>(&element), sizeof(T));
        current = current->GetNext();
    }
    
    outFile.close();
    if (!outFile.good())
        throw runtime_error("Error occurred while writing to file: " + filename);
}

template <typename T>
inline void TLists<T>::LoadFromFile(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile)
        throw runtime_error("Could not open file for reading: " + filename);

    // Очищаем текущий список
    while (!isEmpty())
        PopFront();

    // Читаем размер списка
    size_t fileSize;
    inFile.read(reinterpret_cast<char*>(&fileSize), sizeof(size_t));

    // Читаем элементы
    for (size_t i = 0; i < fileSize; ++i)
    {
        T element;
        inFile.read(reinterpret_cast<char*>(&element), sizeof(T));
        if (!inFile.good())
            throw runtime_error("Error occurred while reading from file: " + filename);
        PushBack(element);
    }

    inFile.close();
}

template <typename T>
inline TLists<T>::~TLists()
{
    if(head != nullptr)
        delete head;
}

template <typename O>
inline ostream &operator<<(ostream &out, const TLists<O> &other)
{
    if(!other.isEmpty())
    {
        out << "elements : "<< endl;
        for (auto it = other.begin(); it != other.end(); ++it)
            out << *it << " ";
        out << endl << "size = " << other.GetSize();
    }
    else
        out << "list is empty";
    return out;
}

template <typename O>
inline istream &operator>>(istream &inp, const TLists<O> &other)
{
    O val = O();
    cout << "Enter element";
    inp >> val;
    other.PushBack(val);
    return inp;
}
