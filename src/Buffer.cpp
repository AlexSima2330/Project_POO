#include "Buffer.h"

Buffer::Buffer(int rows, int cols) : rows(rows), cols(cols), cursorRow(0), cursorCol(0) {
    screen.resize(rows, std::string(cols, ' '));
}

void Buffer::clear() {
    for (auto &row : screen)
        row.assign(cols, ' ');
}

void Buffer::printToConsole() const {
    for (const auto &row : screen)
        std::cout << row << std::endl;
}

void Buffer::setCursor(int row, int col) {
    cursorRow = row;
    cursorCol = col;
}

void Buffer::putChar(char c) {
    if (cursorRow >= 0 && cursorRow < rows && cursorCol >= 0 && cursorCol < cols) {
        screen[cursorRow][cursorCol] = c;
        cursorCol++;
    }
}

void Buffer::putString(const std::string &str) {
    for (char c : str) {
        putChar(c);
    }
}

Buffer& operator<<(Buffer &buffer, const std::string &str) {
    buffer.putString(str);
    return buffer;
}
