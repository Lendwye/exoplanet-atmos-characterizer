int g_row_number = -1;

void getName(RowData &pdata) {
    std::cout << colTxt("Введите название планеты: ", 33).str();
    std::getline(std::cin >> std::ws, pdata.name); // ws - на случай лишних пробелов перед вводом, pdata.name - название планеты
    for (char &c : pdata.name) {
        c = std::tolower(c);
    }

    rapidcsv::Document doc("catalogue.csv");
    auto names = doc.GetColumn<std::string>(1); // получение второго столбца (с названиями) в виде вектора строк
    for (size_t i = 0; i < names.size(); ++i) {
        for (char &c : names[i]) {
            c = std::tolower(c);
        }
        if (names[i] == pdata.name) {
            g_row_number = i; // при совпадении названия с pdata.name - сохранение номера строки
            return;
        }
    }
    std::cerr << "Планета не найдена." << std::endl;
    exit(0);
}

void getName2(RowData &pdata) {
    std::cout << colTxt("Моделирование: ", 33).str() << pdata.name << std::endl;
    rapidcsv::Document doc("catalogue.csv");
    auto names = doc.GetColumn<std::string>(1); // получение второго столбца (с названиями) в виде вектора строк
    for (size_t i = 0; i < names.size(); ++i) {
        if (names[i] == pdata.name) {
            g_row_number = i; // при совпадении названия с pdata.name - сохранение номера строки
            return;
        }
    }
}

void csvParser(RowData &pdata) {
    rapidcsv::Document doc("catalogue.csv");
    auto row = doc.GetRow<std::string>(g_row_number); // получение строки g_row_number (i) в виде вектора строк

    /*
     * Все переменные из header-строки указаны в файле Planetpdata.h (в виде struct) и в catalogue_def.txt (для более легкого определения номеров столбцов)
     * Если строка variable, полученная из ячейки, не пустая - строка с помощью std::stof преобразовывается в float variable
     * Если строка variable, полученная из ячейки, пустая - float variable приравнивается к -274 (маркер для функций из ЧII, что значение неизвестно; отладка std::stof)
     * Число -274 выбрано как ближайшее, округляя вниз, целое число после абсолютного нуля, чтобы не создавать конфликтов с float temp_calculated
     *
     */

    pdata.mass = (row[3].empty()) ? -274 : std::stof(row[3]);
    pdata.mass_sini = (row[6].empty()) ? -274 : std::stof(row[6]);
    pdata.radius = (row[9].empty()) ? -274 : std::stof(row[9]);
    pdata.orbital_period = (row[12].empty()) ? -274 : std::stof(row[12]);
    pdata.semi_major_axis = (row[15].empty()) ? -274 : std::stof(row[15]);
    pdata.eccentricity = (row[18].empty()) ? -274 : std::stof(row[18]);
    pdata.inclination = (row[21].empty()) ? -274 : std::stof(row[21]);
    pdata.angular_distance = (row[24].empty()) ? -274 : std::stof(row[24]);
    pdata.discovered = (row[25].empty()) ? -274 : std::stoi(row[25]); // Обработка int
    pdata.updated = row[26];
    pdata.impact_parameter = (row[45].empty()) ? -274 : std::stof(row[45]);
    pdata.temp_calculated = (row[54].empty()) ? -274 : std::stof(row[54]);
    pdata.temp_measured = (row[57].empty()) ? -274 : std::stof(row[57]);
    pdata.hot_point_lon = (row[58].empty()) ? -274 : std::stof(row[58]);
    pdata.geometric_albedo = (row[59].empty()) ? -274 : std::stof(row[59]);
    pdata.log_g = (row[62].empty()) ? -274 : std::stof(row[62]);
    pdata.detection_type = row[64];
    pdata.mass_measurement_type = row[65];
    pdata.radius_measurement_type = row[66];
    pdata.alternate_names = row[67];
    pdata.molecules = row[68];
    pdata.star_mass = (row[83].empty()) ? -274 : std::stof(row[83]);
    pdata.star_radius = (row[86].empty()) ? -274 : std::stof(row[86]);
    pdata.star_teff = (row[93].empty()) ? -274 : std::stof(row[93]);
    // pdata.star_sp_type = row[89];
}
