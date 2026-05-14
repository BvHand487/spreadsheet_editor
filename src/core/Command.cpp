#include "Command.h"
#include "Table.h"


void QuitCommand::execute()
{
    running = false;
}

void PrintCommand::execute()
{
    std::ios_base::fmtflags original_flags = std::cout.flags();
    char original_fill = std::cout.fill();

    size_t width_array[5] = { 0 };

    for (size_t col = 0; col < 5; ++col)
    {
        auto columnCells = table.getColumn(col);
        size_t max_length = 0;
        for (size_t row = 0; row < 5; ++row)
        {
            if (!columnCells[row]) continue;

            const auto length = columnCells[row]->serialize().length();
            if (max_length < length) max_length = length;
        }

        width_array[col] = max_length;
    }

    for (size_t row = 0; row < 5; ++row)
    {
        for (size_t col = 0; col < 5; ++col)
        {
            const auto cell = table.getCell(row, col);

            size_t current_width = width_array[col] + 2;
            if (width_array[col] == 0) current_width = 3;

            std::cout << std::right << std::setfill(' ') << std::setw(current_width);

            if (cell) std::cout << cell->serialize();
            else std::cout << " ";

            if (col < 4) std::cout << "|";
        }

        std::cout << "\n";
    }

    std::cout.flags(original_flags);
    std::cout.fill(original_fill);
    std::cout << std::flush;
}
