#include "Functions.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#include <unistd.h>
#endif

namespace {
void clear_screen() {
#ifdef _WIN32
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (console == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(console, &info)) return;
    const DWORD cells = static_cast<DWORD>(info.dwSize.X) * info.dwSize.Y;
    const COORD origin{0, 0};
    DWORD written = 0;
    FillConsoleOutputCharacterW(console, L' ', cells, origin, &written);
    FillConsoleOutputAttribute(console, info.wAttributes, cells, origin, &written);
    SetConsoleCursorPosition(console, origin);
#else
    if (isatty(fileno(stdout))) std::cout << "\x1b[2J\x1b[H";
#endif
}

std::optional<std::string> ask(const std::string& prompt) {
    std::cout << prompt;
    std::string answer;
    if (!std::getline(std::cin, answer)) return std::nullopt;
    return answer;
}

std::optional<unsigned int> ask_number(const std::string& prompt, unsigned int min,
                                        unsigned int max = std::numeric_limits<unsigned int>::max(),
                                        bool allow_cancel = false) {
    while (auto answer = ask(prompt)) {
        if (allow_cancel && *answer == "0") return std::nullopt;
        try {
            std::size_t used = 0;
            const auto value = std::stoull(*answer, &used);
            if (used == answer->size() && value >= min && value <= max) {
                return static_cast<unsigned int>(value);
            }
        } catch (const std::exception&) {}
        std::cout << "Введите целое число от " << min << " до " << max;
        if (allow_cancel) std::cout << " или 0 для отмены";
        std::cout << ".\n";
    }
    return std::nullopt;
}

std::optional<std::string> ask_nonempty(const std::string& prompt, bool allow_cancel = false) {
    while (auto answer = ask(prompt)) {
        if (allow_cancel && *answer == "0") return std::nullopt;
        if (!answer->empty()) return answer;
        std::cout << "Поле не может быть пустым.\n";
    }
    return std::nullopt;
}

void pause_screen() {
    ask("\nНажмите Enter, чтобы вернуться в меню...");
}

void empty_hint() {
    std::cout << "База пуста. Загрузите sample.csv через пункт 1 или добавьте билет через пункт 5.\n";
}

bool confirm(const std::string& prompt) {
    const auto answer = ask(prompt);
    return answer && (*answer == "д" || *answer == "Д" ||
                      *answer == "y" || *answer == "Y" || *answer == "1");
}

std::optional<Ticket> enter_ticket() {
    std::cout << "На любом шаге введите 0, чтобы отменить ввод.\n";
    const auto number = ask_number("Номер поезда: ", 1,
        std::numeric_limits<unsigned int>::max(), true);
    if (!number) return std::nullopt;
    const auto price = ask_number("Цена билета (руб.): ", 1,
        std::numeric_limits<unsigned int>::max(), true);
    if (!price) return std::nullopt;
    const auto start = ask_nonempty("Пункт отправления: ", true);
    if (!start) return std::nullopt;
    const auto end = ask_nonempty("Пункт прибытия: ", true);
    if (!end) return std::nullopt;
    const auto seat = ask_nonempty("Тип места: ", true);
    if (!seat) return std::nullopt;
    Ticket ticket{*number, *price, *start, *end, *seat};
    std::string error;
    if (!valid_ticket(ticket, error)) {
        std::cout << error << '\n';
        return std::nullopt;
    }
    return ticket;
}

std::size_t display_width(const std::string& text) {
    std::size_t width = 0;
    for (unsigned char c : text) {
        if ((c & 0xC0) != 0x80) ++width;
    }
    return width;
}

std::string cell(std::string text, std::size_t width) {
    if (display_width(text) > width) {
        std::string clipped;
        std::size_t characters = 0;
        for (std::size_t i = 0; i < text.size() && characters < width - 1;) {
            const unsigned char first = static_cast<unsigned char>(text[i]);
            const std::size_t bytes = first < 0x80 ? 1 : first < 0xE0 ? 2 : first < 0xF0 ? 3 : 4;
            clipped.append(text, i, bytes);
            i += bytes;
            ++characters;
        }
        text = clipped + "…";
    }
    return text + std::string(width - display_width(text), ' ');
}

void table_border() {
    for (const auto width : {3, 7, 8, 16, 16, 9}) {
        std::cout << '+' << std::string(width + 2, '-');
    }
    std::cout << "+\n";
}

void table_row(const std::vector<std::string>& values) {
    const std::size_t widths[]{3, 7, 8, 16, 16, 9};
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << "| " << cell(values[i], widths[i]) << ' ';
    }
    std::cout << "|\n";
}

void print_tickets(const Tickets& tickets) {
    if (tickets.empty()) { empty_hint(); return; }
    table_border();
    table_row({"№", "Поезд", "Цена", "Откуда", "Куда", "Место"});
    table_border();
    for (std::size_t i = 0; i < tickets.size(); ++i) {
        const auto& t = tickets[i];
        table_row({std::to_string(i + 1), std::to_string(t.number), std::to_string(t.price),
                   t.start, t.end, t.seat});
    }
    table_border();
}

bool save(const Tickets& tickets, std::string& path, bool& changed) {
    const auto name = ask("Имя файла без .csv (Enter — " + path + ", 0 — отмена): ");
    if (!name || *name == "0") {
        std::cout << "Сохранение отменено.\n";
        return false;
    }
    std::string destination = name->empty() ? path : *name;
    std::string extension = destination.size() >= 4 ? destination.substr(destination.size() - 4) : "";
    for (char& c : extension) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    }
    if (extension != ".csv") destination += ".csv";
    std::cout << "Итоговый файл: " << destination << '\n';
    std::ifstream existing(destination, std::ios::binary);
    const bool exists = existing.good();
    existing.close();
    if (!confirm(exists ? "Файл уже существует. Перезаписать? (д/н): "
                        : "Сохранить данные в этот файл? (д/н): ")) {
        std::cout << "Сохранение отменено.\n";
        return false;
    }
    std::string error;
    if (!save_tickets(destination, tickets, error)) {
        std::cout << error << '\n';
        return false;
    }
    path = destination;
    changed = false;
    std::cout << "Сохранено: " << path << '\n';
    return true;
}

bool edit_ticket(Ticket& original) {
    std::cout << "Что изменить?\n"
              << "0 — отмена    1 — номер     2 — цена      3 — отправление\n"
              << "4 — прибытие  5 — место     6 — весь билет\n";
    const auto field = ask_number("Выбор: ", 0, 6);
    if (!field) return false;
    if (*field == 0) return false;
    if (*field == 6) {
        const auto replacement = enter_ticket();
        if (!replacement) return false;
        if (!confirm("Применить изменения билета? (д/н): ")) return false;
        original = *replacement;
        return true;
    }
    Ticket candidate = original;
    if (*field == 1 || *field == 2) {
        std::cout << "Введите 0 для отмены.\n";
        const auto value = ask_number(*field == 1 ? "Новый номер поезда: " : "Новая цена: ",
            1, std::numeric_limits<unsigned int>::max(), true);
        if (!value) return false;
        if (*field == 1) candidate.number = *value;
        else candidate.price = *value;
    } else {
        std::cout << "Введите 0 для отмены.\n";
        const auto value = ask_nonempty(
            *field == 3 ? "Новый пункт отправления: " :
            *field == 4 ? "Новый пункт прибытия: " : "Новый тип места: ", true);
        if (!value) return false;
        if (*field == 3) candidate.start = *value;
        else if (*field == 4) candidate.end = *value;
        else candidate.seat = *value;
    }
    std::string error;
    if (!valid_ticket(candidate, error)) {
        std::cout << error << " Изменение отменено.\n";
        return false;
    }
    if (!confirm("Применить изменения билета? (д/н): ")) return false;
    original = std::move(candidate);
    return true;
}

void menu(std::size_t count) {
    std::cout << "БАЗА ЖЕЛЕЗНОДОРОЖНЫХ БИЛЕТОВ  |  записей: " << count << "\n\n"
              << "0 — выход\n1 — загрузить CSV\n2 — показать билеты\n"
              << "3 — изменить билет\n4 — удалить билет\n5 — добавить билет\n"
              << "6 — сортировать\n7 — сохранить CSV\n8 — найти по пункту\n"
              << "9 — самый дорогой и дешёвый\n10 — средняя цена\n\n";
    if (count == 0) empty_hint();
}
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    Tickets tickets;
    std::string save_path = "tickets.csv";
    std::string error;
    bool changed = false;
    while (true) {
        clear_screen();
        menu(tickets.size());
        const auto action = ask_number("Действие: ", 0, 10);
        if (!action) return 0;
        clear_screen();
        if (*action == 0) {
            if (changed) {
                if (confirm("Есть несохранённые изменения. Сохранить? (д/н): ")) {
                    if (!save(tickets, save_path, changed)) { pause_screen(); continue; }
                }
            }
            return 0;
        }
        if (*action == 1) {
            if (changed) {
                if (!confirm("Несохранённые изменения будут потеряны. Продолжить? (д/н): ")) {
                    std::cout << "Загрузка отменена.\n";
                    pause_screen();
                    continue;
                }
            }
            const auto name = ask_nonempty("Путь к CSV-файлу (например, sample.csv; 0 — отмена): ", true);
            if (!name) {
                std::cout << "Загрузка отменена.\n";
            } else if (load_tickets(*name, tickets, error)) {
                changed = false;
                std::cout << "Загружено билетов: " << tickets.size() << "\n\n";
                print_tickets(tickets);
            } else {
                std::cout << error << '\n';
            }
        } else if (*action == 2) {
            print_tickets(tickets);
        } else if (*action == 3) {
            if (tickets.empty()) { empty_hint(); }
            else {
                print_tickets(tickets);
                const auto index = ask_number("Номер записи (0 — отмена): ", 1,
                    static_cast<unsigned int>(tickets.size()), true);
                if (!index) {
                    std::cout << "Изменение отменено.\n";
                } else if (edit_ticket(tickets[*index - 1])) {
                    changed = true;
                    std::cout << "Билет изменён.\n\n";
                    print_tickets(tickets);
                } else {
                    std::cout << "Изменение отменено.\n";
                }
            }
        } else if (*action == 4) {
            if (tickets.empty()) { empty_hint(); }
            else {
                print_tickets(tickets);
                const auto index = ask_number("Номер записи (0 — отмена): ", 1,
                    static_cast<unsigned int>(tickets.size()), true);
                if (index && confirm("Удалить выбранный билет? (д/н): ")) {
                    tickets.erase(tickets.begin() + (*index - 1));
                    changed = true;
                    std::cout << "Билет удалён.\n\n";
                    print_tickets(tickets);
                } else {
                    std::cout << "Удаление отменено.\n";
                }
            }
        } else if (*action == 5) {
            const auto ticket = enter_ticket();
            if (ticket && confirm("Добавить этот билет? (д/н): ")) {
                tickets.push_back(*ticket);
                changed = true;
                std::cout << "Билет добавлен.\n\n";
                print_tickets(tickets);
            } else {
                std::cout << "Добавление отменено.\n";
            }
        } else if (*action == 6) {
            if (tickets.empty()) { empty_hint(); }
            else {
                std::cout << "1 — номер поезда, 2 — цена, 3 — отправление, 4 — прибытие, 5 — место\n";
                const auto field = ask_number("Поле (0 — отмена): ", 1, 5, true);
                if (!field) {
                    std::cout << "Сортировка отменена.\n";
                } else {
                    const auto order = ask_number("1 — по возрастанию, 2 — по убыванию, 0 — отмена: ", 1, 2, true);
                    if (!order) {
                        std::cout << "Сортировка отменена.\n";
                    } else {
                        sort_tickets(tickets, static_cast<int>(*field), *order == 1);
                        changed = true;
                        print_tickets(tickets);
                    }
                }
            }
        } else if (*action == 7) {
            if (tickets.empty() && !changed) empty_hint();
            else save(tickets, save_path, changed);
        } else if (*action == 8) {
            if (tickets.empty()) { empty_hint(); }
            else {
                const auto place = ask_nonempty("Пункт маршрута (0 — отмена): ", true);
                if (!place) {
                    std::cout << "Поиск отменён.\n";
                } else {
                    const auto matches = search_tickets(tickets, *place);
                    std::cout << "Найдено: " << matches.size() << '\n';
                    if (!matches.empty()) {
                        Tickets found;
                        for (auto index : matches) found.push_back(tickets[index]);
                        print_tickets(found);
                    }
                }
            }
        } else if (*action == 9) {
            if (tickets.empty()) { empty_hint(); }
            else {
                std::cout << "Самый дешёвый:\n";
                print_tickets({tickets[cheapest_ticket(tickets)]});
                std::cout << "\nСамый дорогой:\n";
                print_tickets({tickets[dearest_ticket(tickets)]});
            }
        } else {
            if (tickets.empty()) { empty_hint(); }
            else {
                std::cout << "Средняя цена: " << std::fixed << std::setprecision(2)
                          << average_price(tickets) << " руб.\n";
            }
        }
        pause_screen();
    }
}
