#include <iostream>
#include "dictionary/dictionary.hpp"

int main() {
    Dictionary<std::string, int> npc;
    npc.put("health", 10);
    npc.put("armor", 20);
    npc.put("ammo", 5);
    npc.print();
/*    auto it = npc.iterator();
    for (; it.hasNext(); it.next())
        std::cout << it.key() << " " << it.get() << std::endl;
    std::cout << it.key() << " " << it.get() << std::endl;*/
    return 0;
}