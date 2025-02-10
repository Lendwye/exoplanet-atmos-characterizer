// Для вывода сообщений об успехе, ошибке, акцентировании внимания на чём-либо
std::stringstream colTxt(const std::string &text, int textColor, int backgroundColor = -1) {
    std::stringstream ss;
    ss << "\x1b[";
    ss << textColor;
    if (backgroundColor != -1) {
        ss << ";" << backgroundColor;
    }
    ss << "m" << text << "\x1b[0m";
    return ss;

    // Цвета текста:
    // 30 - черный, 31 - красный, 32 - зеленый, 33 - желтый, 34 - синий, 35 - фиолетовый, 36 - голубой, 37 - белый (для UNIX-систем)
}
