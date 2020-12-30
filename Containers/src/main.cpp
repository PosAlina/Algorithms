#include <iostream>

#include "dynamic_array/dynamic_array.hpp"
#include "linked_list/linked_list.hpp"

int main() {
    std::cout << "Dynamic array:" << std::endl;
    Array<int> a;
    for (int i = 0; i < 10; ++i) a.insert(i + 1);
    a.size();
    for (int i = 0; i < a.size(); ++i) a[i] *= 2;
    auto it = a.iterator();
    for (; it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << it.get() << std::endl;

    std::cout <<"Linked list:" << std::endl;
    List<int> l;
    for (int i = 0; i < 10; ++i) l.insertHead(i + 1);

    auto it2 = l.iterator();
    for (; it2.hasNext(); it2.next())
        std::cout << it2.get() << " ";
    std::cout << it2.get() << std::endl;
    return 0;
}