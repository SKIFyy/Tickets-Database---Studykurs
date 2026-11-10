#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct Ticket {
    unsigned int number{};
    unsigned int price{};
    std::string start;
    std::string end;
    std::string seat;
};

using Tickets = std::vector<Ticket>;

bool valid_ticket(const Ticket& ticket, std::string& error);
bool load_tickets(const std::string& path, Tickets& tickets, std::string& error);
bool save_tickets(const std::string& path, const Tickets& tickets, std::string& error);
std::vector<std::size_t> search_tickets(const Tickets& tickets, const std::string& place);
void sort_tickets(Tickets& tickets, int field, bool ascending);
double average_price(const Tickets& tickets);
std::size_t cheapest_ticket(const Tickets& tickets);
std::size_t dearest_ticket(const Tickets& tickets);

