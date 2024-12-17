#include "Buffer.h"
#include <iostream>
#include <cstdio>

Buffer::Buffer(int rows, int cols) : rows(rows), cols(cols), cursorRow(0), cursorCol(0) {
    screen = new char*[rows];
    for (int i = 0; i < rows; i++) {
        screen[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            screen[i][j] = ' ';
        }
    }
}

Buffer::Buffer(const Buffer &other) : rows(other.rows), cols(other.cols), cursorRow(other.cursorRow), cursorCol(other.cursorCol) {
    // Alocar nova memória
    screen = new char*[rows];
    for (int i = 0; i < rows; i++) {
        screen[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            screen[i][j] = other.screen[i][j]; // Copiar cada caracter
        }
    }
}

Buffer& Buffer::operator=(const Buffer &other) {
    if (this == &other) // Previne auto-atribuição
        return *this;

    // Libertar memória atual
    for (int i = 0; i < rows; i++) {
        delete[] screen[i];
    }
    delete[] screen;

    // Copiar dimensões e cursor
    rows = other.rows;
    cols = other.cols;
    cursorRow = other.cursorRow;
    cursorCol = other.cursorCol;

    // Alocar nova memória e copiar dados
    screen = new char*[rows];
    for (int i = 0; i < rows; i++) {
        screen[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            screen[i][j] = other.screen[i][j];
        }
    }

    return *this;
}

Buffer::~Buffer() {
    for (int i = 0; i < rows; i++) {
        delete[] screen[i];
    }
    delete[] screen;
}

void Buffer::clear() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            screen[i][j] = ' ';
        }
    }
    cursorRow = 0;
    cursorCol = 0;
}

void Buffer::printToConsole() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << screen[i][j];
        }
        std::cout << "\n";
    }
}

void Buffer::setCursor(int row, int col) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        cursorRow = row;
        cursorCol = col;
    }
}

void Buffer::putChar(char c) {
    if (cursorRow >= 0 && cursorRow < rows && cursorCol >= 0 && cursorCol < cols) {
        screen[cursorRow][cursorCol] = c;
        cursorCol++;
        if (cursorCol >= cols) {
            cursorCol = 0;
            cursorRow++;
        }
    }
}

void Buffer::putString(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putChar(str[i]);
    }
}

// Operador << para C-strings
Buffer& operator<<(Buffer &buffer, const char *str) {
    buffer.putString(str);
    return buffer;
}

// Operador << para inteiros
Buffer& operator<<(Buffer &buffer, int num) {
    char tmp[32];
    std::snprintf(tmp, sizeof(tmp), "%d", num);
    buffer.putString(tmp);
    return buffer;
}

// Operador << para caracteres
Buffer& operator<<(Buffer &buffer, char c) {
    buffer.putChar(c);
    return buffer;
}
