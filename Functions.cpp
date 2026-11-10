#include "Functions.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {
bool parse_number(const std::string& text, unsigned int& number) {
    if (text.empty() || !std::all_of(text.begin(), text.end(),
            [](unsigned char c) { return std::isdigit(c) != 0; })) return false;
    try {
        const auto value = std::stoull(text);
        if (value > std::numeric_limits<unsigned int>::max()) return false;
        number = static_cast<unsigned int>(value);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parse_row(const std::string& line, std::vector<std::string>& fields) {
    fields.clear();
    std::string field;
    bool quoted = false;
    bool closed_quote = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (quoted) {
            if (c == '"' && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else if (c == '"') {
                quoted = false;
                closed_quote = true;
            } else {
                field += c;
            }
        } else if (c == ',') {
            fields.push_back(field);
            field.clear();
            closed_quote = false;
        } else if (c == '"' && field.empty() && !closed_quote) {
            quoted = true;
        } else if (c == '"' || closed_quote) {
            return false;
        } else {
            field += c;
        }
    }
    if (quoted) return false;
    fields.push_back(field);
    return true;
}

std::string csv_field(const std::string& value) {
    if (value.find_first_of(",\"\r\n") == std::string::npos) return value;
    std::string result = "\"";
    for (char c : value) {
        result += c;
        if (c == '"') result += '"';
    }
    return result + '"';
}

std::string ascii_lower(std::string value) {
    for (char& c : value) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    }
    return value;
}
}

bool valid_ticket(const Ticket& ticket, std::string& error) {
    if (ticket.number == 0 || ticket.price == 0) {
        error = "Номер поезда и цена должны быть больше нуля.";
    } else if (ticket.start.empty() || ticket.end.empty() || ticket.seat.empty()) {
        error = "Текстовые поля не могут быть пустыми.";
    } else if (ticket.start == ticket.end) {
        error = "Пункт отправления и прибытия не могут совпадать.";
    } else if (ticket.start.find_first_of("\r\n") != std::string::npos ||
               ticket.end.find_first_of("\r\n") != std::string::npos ||
               ticket.seat.find_first_of("\r\n") != std::string::npos) {
        error = "Поля не могут содержать перевод строки.";
    } else {
        error.clear();
        return true;
    }
    return false;
}

bool load_tickets(const std::string& path, Tickets& tickets, std::string& error) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        error = "Не удалось открыть файл: " + path;
        return false;
    }
    Tickets loaded;
    std::string line;
    std::size_t line_number = 0;
    while (std::getline(file, line)) {
        ++line_number;
        if (line_number == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line.erase(0, 3);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> fields;
        Ticket ticket;
        if (!parse_row(line, fields) || fields.size() != 5 ||
            !parse_number(fields[0], ticket.number) ||
            !parse_number(fields[1], ticket.price)) {
            error = "Неверный формат CSV в строке " + std::to_string(line_number) + ".";
            return false;
        }
        ticket.start = fields[2];
        ticket.end = fields[3];
        ticket.seat = fields[4];
        if (!valid_ticket(ticket, error)) {
            error = "Строка " + std::to_string(line_number) + ": " + error;
            return false;
        }
        loaded.push_back(std::move(ticket));
    }
    if (file.bad()) {
        error = "Ошибка чтения файла: " + path;
        return false;
    }
    tickets = std::move(loaded);
    error.clear();
    return true;
}

bool save_tickets(const std::string& path, const Tickets& tickets, std::string& error) {
    for (const auto& ticket : tickets) {
        if (!valid_ticket(ticket, error)) return false;
    }
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file) {
        error = "Не удалось создать файл: " + path;
        return false;
    }
    file << "\xEF\xBB\xBF";
    for (const auto& ticket : tickets) {
        file << ticket.number << ',' << ticket.price << ','
             << csv_field(ticket.start) << ',' << csv_field(ticket.end) << ','
             << csv_field(ticket.seat) << '\n';
    }
    file.close();
    if (!file) {
        error = "Ошибка записи файла: " + path;
        return false;
    }
    error.clear();
    return true;
}

std::vector<std::size_t> search_tickets(const Tickets& tickets, const std::string& place) {
    std::vector<std::size_t> matches;
    const auto query = ascii_lower(place);
    for (std::size_t i = 0; i < tickets.size(); ++i) {
        if (ascii_lower(tickets[i].start).find(query) != std::string::npos ||
            ascii_lower(tickets[i].end).find(query) != std::string::npos) matches.push_back(i);
    }
    return matches;
}

void sort_tickets(Tickets& tickets, int field, bool ascending) {
    if (field < 1 || field > 5) throw std::invalid_argument("Неверное поле сортировки");
    std::stable_sort(tickets.begin(), tickets.end(),
        [field, ascending](const Ticket& a, const Ticket& b) {
            bool less = false;
            bool greater = false;
            switch (field) {
                case 1: less = a.number < b.number; greater = a.number > b.number; break;
                case 2: less = a.price < b.price; greater = a.price > b.price; break;
                case 3: less = a.start < b.start; greater = a.start > b.start; break;
                case 4: less = a.end < b.end; greater = a.end > b.end; break;
                case 5: less = a.seat < b.seat; greater = a.seat > b.seat; break;
            }
            return ascending ? less : greater;
        });
}

double average_price(const Tickets& tickets) {
    if (tickets.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& ticket : tickets) sum += ticket.price;
    return sum / static_cast<double>(tickets.size());
}

std::size_t cheapest_ticket(const Tickets& tickets) {
    if (tickets.empty()) throw std::invalid_argument("Список билетов пуст");
    return static_cast<std::size_t>(std::min_element(tickets.begin(), tickets.end(),
        [](const Ticket& a, const Ticket& b) { return a.price < b.price; }) - tickets.begin());
}

std::size_t dearest_ticket(const Tickets& tickets) {
    if (tickets.empty()) throw std::invalid_argument("Список билетов пуст");
    return static_cast<std::size_t>(std::max_element(tickets.begin(), tickets.end(),
        [](const Ticket& a, const Ticket& b) { return a.price < b.price; }) - tickets.begin());
}
