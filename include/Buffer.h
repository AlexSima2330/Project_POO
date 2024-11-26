#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vector>
#include <string>

class Buffer {
private:
    int rows, cols;
    std::vector<std::string> screen;
    int cursorRow, cursorCol;

public:
    Buffer(int rows, int cols);
    void clear();
    void printToConsole() const;
    void setCursor(int row, int col);
    void putChar(char c);
    void putString(const std::string &str);
    friend Buffer& operator<<(Buffer &buffer, const std::string &str);
};

#endif
