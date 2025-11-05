#include "gtest.h"
#include "../TLists/TLists.h"
#include <string>

// Тест конструктора по умолчанию
TEST(TListsTest, DefaultConstructor) {
    TLists<int> list;
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.GetSize(), 0);
}

// Тест конструктора с размером
TEST(TListsTest, SizeConstructor) {
    TLists<int> list(5);
    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(list.GetSize(), 5);
}

// Тесты для PushBack и PushFront
TEST(TListsTest, PushOperations) {
    TLists<int> list;
    
    // Тест PushBack
    list.PushBack(1);
    EXPECT_EQ(list.GetSize(), 1);
    EXPECT_EQ(*list.begin(), 1);

    list.PushBack(2);
    EXPECT_EQ(list.GetSize(), 2);
    
    // Тест PushFront
    list.PushFront(0);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(*list.begin(), 0);
}

// Тесты для PopBack и PopFront
TEST(TListsTest, PopOperations) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    // Тест PopFront
    EXPECT_EQ(list.PopFront(), 1);
    EXPECT_EQ(list.GetSize(), 2);

    // Тест PopBack
    EXPECT_EQ(list.PopBack(), 3);
    EXPECT_EQ(list.GetSize(), 1);
}

// Тест на исключения при попытке удаления из пустого списка
TEST(TListsTest, PopFromEmptyList) {
    TLists<int> list;
    EXPECT_THROW(list.PopFront(), std::out_of_range);
    EXPECT_THROW(list.PopBack(), std::out_of_range);
}

// Тест оператора индексации
TEST(TListsTest, IndexOperator) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
    EXPECT_THROW(list[3], std::out_of_range);
}

// Тест итератора
TEST(TListsTest, Iterator) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

// Тест SetSize
TEST(TListsTest, SetSize) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    // Уменьшение размера
    list.SetSize(2);
    EXPECT_EQ(list.GetSize(), 2);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_THROW(list[2], std::out_of_range);
}

// Тест операторов сравнения
TEST(TListsTest, ComparisonOperators) {
    TLists<int> list1;
    TLists<int> list2;

    list1.PushBack(1);
    list1.PushBack(2);
    list2.PushBack(1);
    list2.PushBack(2);

    EXPECT_TRUE(list1 == list2);
    list2.PushBack(3);
    EXPECT_TRUE(list1 != list2);
}

// Тест сохранения и загрузки из файла
TEST(TListsTest, FileOperations) {
    TLists<int> list1;
    list1.PushBack(1);
    list1.PushBack(2);
    list1.PushBack(3);

    // Сохранение в файл
    list1.SaveToFile("test_list.bin");

    // Загрузка из файла
    TLists<int> list2;
    list2.LoadFromFile("test_list.bin");

    EXPECT_TRUE(list1 == list2);
}

// Тест поиска элементов
TEST(TListsTest, FindElements) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(2);
    list.PushBack(3);

    EXPECT_EQ(list.FindCountInputElem(2), 2);
    auto positions = list.FindInputElem(2);
    EXPECT_EQ(positions.size(), 2);
    EXPECT_EQ(positions[0], 1);
    EXPECT_EQ(positions[1], 2);
}

// Тест вставки после элемента
TEST(TListsTest, InsertAfter) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(3);

    auto it = list.begin();
    list.IncertAfter(it, 2);

    int expected[] = {1, 2, 3};
    int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

// Тест удаления элемента
TEST(TListsTest, RemoveElement) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    auto it = list.begin();
    ++it; // перемещаемся к элементу 2
    EXPECT_EQ(list.RemoveElem(it), 2);
    EXPECT_EQ(list.GetSize(), 2);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 3);
}

// Тест лямбда-функции
TEST(TListsTest, LambdaFunction) {
    TLists<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    int sum = 0;
    list.LamdaFunc([&sum](const int& value) { sum += value; });
    EXPECT_EQ(sum, 6);
}
