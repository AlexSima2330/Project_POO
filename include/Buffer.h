#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
private:
    int rows, cols;
    char** screen; // Apontador para um array bidimensional
    int cursorRow, cursorCol;

public:
    Buffer(int rows, int cols);
    Buffer(const Buffer &other);            // Construtor de cópia
    Buffer& operator=(const Buffer &other);
    ~Buffer();
    void clear();
    void printToConsole() const;
    void setCursor(int row, int col);
    void putChar(char c);
    void putString(const char *str);

    // Operadores <<
    friend Buffer& operator<<(Buffer &buffer, const char *str);
    friend Buffer& operator<<(Buffer &buffer, int num);
    friend Buffer& operator<<(Buffer &buffer, char c);
};

#endif
