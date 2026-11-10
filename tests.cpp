#include "Functions.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

int main() {
    Tickets tickets{{235, 3450, "Москва", "Казань", "купе"},
                    {536, 1340, "Санкт-Петербург", "Москва", "плацкарт"},
                    {567, 1520, "Казань", "Самара", "сидячее"}};
    std::string error;
    assert(valid_ticket(tickets[0], error));
    assert(!valid_ticket({1, 10, "Москва", "Москва", "купе"}, error));
    assert(cheapest_ticket(tickets) == 1);
    assert(dearest_ticket(tickets) == 0);
    assert(average_price(tickets) == (3450.0 + 1340.0 + 1520.0) / 3.0);
    assert(search_tickets(tickets, "Казань").size() == 2);
    sort_tickets(tickets, 2, true);
    assert(tickets[0].price == 1340 && tickets[2].price == 3450);
    sort_tickets(tickets, 1, false);
    assert(tickets[0].number == 567);

    const std::string path = "ticket_tests_tmp.csv";
    tickets.push_back({42, 100, "New,York", "Boston", "seat \"A\""});
    assert(save_tickets(path, tickets, error));
    { std::ifstream file(path, std::ios::binary);
      std::string bom(3, '\0');
      file.read(&bom[0], 3);
      assert(bom == "\xEF\xBB\xBF"); }
    Tickets restored;
    assert(load_tickets(path, restored, error));
    assert(restored.size() == tickets.size());
    assert(restored.back().start == "New,York");
    assert(restored.back().seat == "seat \"A\"");

    { std::ofstream file(path, std::ios::trunc); file << "1,2,only-three\n"; }
    assert(!load_tickets(path, restored, error));
    assert(restored.size() == tickets.size());  // Ошибка не стирает текущие данные.
    { std::ofstream file(path, std::ios::trunc); file << "1,2,Alpha,Beta,seat\n"; }
    assert(load_tickets(path, restored, error));  // Старые файлы без BOM тоже читаются.
    assert(restored.size() == 1);
    std::remove(path.c_str());
}
