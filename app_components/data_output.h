// Вывод общей информации
void dataOutput(RowData &pdata) {
    std::cout << "Название: " << pdata.name << std::endl;
    if (pdata.mass != -274) std::cout << "Масса (Mj): " << pdata.mass << std::endl;
    if (pdata.radius != -274) std::cout << "Радиус (Rj): " << pdata.radius << std::endl;
    if (pdata.orbital_period != -274) std::cout << "Период (дней): " << pdata.orbital_period << std::endl;
    if (pdata.semi_major_axis != -274) std::cout << "Большая полуось (А.Е.): " << pdata.semi_major_axis << std::endl;
    if (pdata.eccentricity != -274) std::cout << "e орбиты: " << pdata.eccentricity << std::endl;
    if (pdata.inclination != -274) std::cout << "Угол наклона орбитальной плоскости: " << pdata.inclination << std::endl;
    if (pdata.angular_distance != -274) std::cout << "Угловое расстояние (сек.): " << pdata.angular_distance << std::endl;
    if (pdata.discovered != -274) std::cout << "Дата обнаружения: " << pdata.discovered << std::endl;
    std::cout << "Дата обновления: " << pdata.updated << std::endl;
    if (pdata.impact_parameter != -274) std::cout << "amin планеты от центра звезды (для транзитных планет) (Rзвезды, %): " << pdata.impact_parameter << std::endl;
    if (pdata.temp_calculated != -274) std::cout << "Tcalc (K): " << pdata.temp_calculated << std::endl;
    if (pdata.temp_measured != -274) std::cout << "Tmeas (K): " << pdata.temp_measured << std::endl;
    if (pdata.hot_point_lon != -274) std::cout << "Hot Point Longtitude:" << pdata.hot_point_lon << std::endl;
    if (pdata.geometric_albedo != -274) std::cout << "Альбедо: " << pdata.geometric_albedo << std::endl;
    if (pdata.surface_gravity != -274) std::cout << "g (м/с^2): " << pdata.surface_gravity << std::endl;
    std::cout << "Тип обнаружения: " << pdata.detection_type << std::endl;
    std::cout << "Тип измерения массы: " << pdata.mass_measurement_type << std::endl;
    std::cout << "Тип измерения радиуса: " << pdata.radius_measurement_type << std::endl;
    std::cout << "Альтернативные названия: " << pdata.alternate_names << std::endl;
    std::cout << "Молекулы: " << colTxt(pdata.molecules, 34).str() << std::endl;
    if (pdata.star_mass != -274) std::cout << "Масса звезды: " << pdata.star_mass << std::endl;
    if (pdata.star_radius != -274) std::cout << "Радиус звезды: " << pdata.star_radius << std::endl;
    if (pdata.star_teff != -274) std::cout << "Teff звезды (K): " << pdata.star_teff << std::endl;
}

int selected = 0; // Отслеживает выбранный пункт

// Просто getch()
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Очистка экрана после обновления выбора
void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

// Отображение указателя
void printOptions(const std::vector<std::string> &options, int selected) {
    clearScreen();
    for (size_t i = 0; i < options.size(); ++i) {
        if (i == selected) {
            std::cout << "> " << options[i] << std::endl;
        } else {
            std::cout << "  " << options[i] << std::endl;
        }
    }
}

// Вызов меню
void menuSelect() {
    int option = 6;        // кол-во опций меню
    bool selecting = true; // true, если мы что-то выбираем

    std::vector<std::string> options = {
        "[1] Просмотр данных о планете",
        "[2] Построение T-P профиля",
        "[3] Определитель типа",
        "[4] Восстановление .dat по умолчанию",
        "[T] UN",
        "[5] Выход"};

    printOptions(options, selected);

    char c;
    while (selecting) { // Так долго, как мы выбираем
        c = getch();
        switch (c) { // Проверка на последний введенный символ
        case KEY_UP:
            if (selected > 0) { // Без декремента, если первая опция
                --selected;
                printOptions(options, selected);
            }
            break;
        case KEY_DOWN:
            if (selected < options.size() - 1) { // Без инкремента, если последняя опция
                ++selected;
                printOptions(options, selected);
            }
            break;
        case KEY_ENTER:
            selecting = false;
            break;
        default:
            break;
        }
    }
    selected++;
}

// Вызов расчетных функций из Ч. II
void getPrepared(RowData pdata, int i) {
    if (i == 1) getName(pdata);
    if (i == 2) getName2(pdata);
    csvParser(pdata);
    getKeplerThird(pdata);
    getRGM(pdata);
    std::cout << colTxt("========================================", 30).str() << std::endl;
}

// Функции меню
void menuAction(RowData &pdata, float &bond_albedo) {
    switch (selected) {
    case 1:
        getPrepared(pdata, 1);
        dataOutput(pdata);
        break;
    case 2:
        getPrepared(pdata, 1);
        tpoutput(bond_albedo, pdata);
        break;
    case 3:
        getPrepared(pdata, 1);
        gadanieCall(pdata);
        gadanieResults(bond_albedo, pdata);
        break;
    case 4:
        pdata.name = "15 Sge b";
        getPrepared(pdata, 2);
        tpoutput(bond_albedo, pdata);
        pdata.name = "2MASS J0337-1758";
        getPrepared(pdata, 2);
        tpoutput(bond_albedo, pdata);
        pdata.name = "HAT-P-49 b";
        getPrepared(pdata, 2);
        tpoutput(bond_albedo, pdata);
        pdata.name = "HD 209458 b";
        getPrepared(pdata, 2);
        tpoutput(bond_albedo, pdata);
        break;
    default:
        exit(0);
    }
}
