int is_Dwarf = 0;
int is_EGP = 0;
int is_EIP = 0;
int is_ETP = 0;
int is_EEP = 0;
const int max_is_value = 8;

// 33-31-34-36-32

// Brown Dwarf
void isThisDwarf(RowData& pdata) {
    if (pdata.mass > 13.6) is_Dwarf = max_is_value; // для минимального порога поставлена масса, при которой начинается синтез дейтерия
    if (pdata.radius >= 0.7 && pdata.radius <= 1.4) is_Dwarf++;
    if (pdata.log_g >= 4.5) is_Dwarf++;
    if (temp_any > 800) is_Dwarf++;

    if (is_Dwarf > max_is_value) is_Dwarf = max_is_value;

    std::string bars(is_Dwarf, '|');
    std::string remainingBars((max_is_value - is_Dwarf), '|');
    std::cout << "[" << colTxt(bars, 33).str() << colTxt(remainingBars, 30).str() << "] D" << std::endl;
}

// Extrasolar Giant Planet
void isThisEGP(RowData& pdata) {
    // для минимального порога поставлена масса WASP-193 Ab, "самого легкого газового гиганта"
    // для максимального порога поставлена масса, при которой начинается синтез дейтерия
    if (pdata.mass > 0.138 && pdata.mass <= 13.2) is_EGP += 3;
    if (pdata.radius > 0.91) is_EGP += 3; // >10R⊕
    if (temp_any > 80) is_EGP += 2;
    if (pdata.log_g > 2.5) is_EGP += 2;

    // Maximum theoretical size limit assumed for a ~ 5 MJ mass object right after formation, however, for 'arbitrary initial conditions'.
    if (pdata.mass >= 4 && pdata.radius >= 7) is_EGP = max_is_value;

    std::string bars(is_EGP, '|');
    std::string remainingBars((max_is_value - is_EGP), '|');
    std::cout << "[" << colTxt(bars, 31).str() << colTxt(remainingBars, 30).str() << "] G" << std::endl;
}

// Extrasolar Ice Planet (Ice Giant)
void isThisEIP(RowData& pdata) {
    if (pdata.radius <= 0.55 && pdata.radius >= 0.31) is_EIP += 4; // 1.9 < R < 3.9 R⊕
    if (pdata.mass > 0.014 && pdata.mass <= 0.10) is_EIP += 4;

    std::string bars(is_EIP, '|');
    std::string remainingBars((max_is_value - is_EIP), '|');
    std::cout << "[" << colTxt(bars, 34).str() << colTxt(remainingBars, 30).str() << "] I" << std::endl;
}

// Extrasolar Transitional Planet
void isThisETP(RowData& pdata) {
    if (pdata.radius <= 0.36 && pdata.radius >= 0.173) is_ETP += 4; // 1.9 < R < 3.9 R⊕
    if (pdata.mass > 0.006 && pdata.mass <= 0.03) is_ETP += 4;

    std::string bars(is_ETP, '|');
    std::string remainingBars((max_is_value - is_ETP), '|');
    std::cout << "[" << colTxt(bars, 36).str() << colTxt(remainingBars, 30).str() << "] T" << std::endl;
}

// Extrasolar Earth Planet
void isThisEEP(RowData& pdata) {
    if (pdata.radius <= 0.173 && pdata.radius >= 0) is_EEP += 4; // < 1.9R⊕
    if (pdata.mass >= 0 && pdata.mass <= 0.01) is_EEP += 4;

    std::string bars(is_EEP, '|');
    std::string remainingBars((max_is_value - is_EEP), '|');
    std::cout << "[" << colTxt(bars, 32).str() << colTxt(remainingBars, 30).str() << "] E" << std::endl;
}

// Обработка значений EEP - определение мини- и супер-земель
void isThisAnyOfEarth(RowData& pdata) {
    int is_mEarth = 0;
    int is_sEarth = 0;

    if (pdata.mass <= 0.005) is_mEarth++;
    if (pdata.mass > 0.007) is_sEarth++;

    std::vector<int> values = {is_mEarth, is_sEarth};
    std::vector<std::string> names = {"Это мини-земля", "Это супер-земля"};
    int max_index = 0;
    for (int i = 1; i < values.size(); ++i) {
        if (values[i] > values[max_index]) {
            max_index = i;
        }
    }
    std::cout << names[max_index] << std::endl;
}

// Обработка значений EGP - определение класса газового гиганта по классификации Сударского
void isThisTheSudarskyPlanet(float &bond_albedo, RowData& pdata) {
    float is_sudarsky = 0;
    int is_classI = 0;
    int is_classII = 0;
    int is_classIII = 0;
    int is_classIV = 0;
    int is_classV = 0;

    if (pdata.mass_sini >= 0.17 || pdata.mass >= 0.17 && pdata.mass < 50.0) is_sudarsky++;
    if (bond_albedo > 0.54 && bond_albedo < 0.61) is_classI++;
    if (temp_any < 195) is_classI++;

    if (bond_albedo >= 0.81) is_classII++;
    if (temp_any >= 195 && temp_any < 250) is_classII++;

    if (bond_albedo >= 0.09 && bond_albedo < 0.20) is_classIII++;
    if (temp_any >= 350 && temp_any < 800) is_classIII++;

    if (bond_albedo < 0.09 && bond_albedo > 0) is_classIV++;
    if (temp_any >= 800 && temp_any < 1050) is_classIV++;

    if (bond_albedo > 0.50 && bond_albedo < 0.60) is_classV++;
    if (temp_any >= 1050) is_classV++;

    if (is_classI > 0) is_sudarsky = is_sudarsky + is_classI / 3.0;
    if (is_classII > 0) is_sudarsky = is_sudarsky + is_classII / 3.0;
    if (is_classIII > 0) is_sudarsky = is_sudarsky + is_classIII / 3.0;
    if (is_classIV > 0) is_sudarsky = is_sudarsky + is_classIV / 3.0;
    if (is_classV > 0) is_sudarsky = is_sudarsky + is_classV / 3.0;

    std::vector<int> is_class = {is_classI, is_classII, is_classIII, is_classIV, is_classV};
    std::vector<int> ints = {2, 4, 6, 8, 10};
    int max_index = 0;
    for (int i = 1; i < is_class.size(); ++i) {
        if (is_class[i] > is_class[max_index]) max_index = i;
    }
    if (is_class[max_index] == is_classI) std::cout << "[" << colTxt("██", 34).str() << colTxt("||||||||", 30).str() << "] Class I" << std::endl;
    if (is_class[max_index] == is_classII) std::cout << "[" << colTxt("||", 30).str() << colTxt("██", 36).str() << colTxt("||||||", 30).str() << "] Class II" << std::endl;
    if (is_class[max_index] == is_classIII) std::cout << "[" << colTxt("||||", 30).str() << colTxt("██", 33).str() << colTxt("||||", 30).str() << "] Class III" << std::endl;
    if (is_class[max_index] == is_classIV) std::cout << "[" << colTxt("||||||", 30).str() << colTxt("██", 33).str() << colTxt("||", 30).str() << "] Class IV" << std::endl;
    if (is_class[max_index] == is_classV) std::cout << "[" << colTxt("||||||||", 30).str() << colTxt("██", 31).str() << "] Class V" << std::endl;
}

// Вызов всех функций для main()
void gadanieCall(RowData& pdata) {
    isThisDwarf(pdata);
    isThisEGP(pdata);
    isThisEIP(pdata);
    isThisETP(pdata);
    isThisEEP(pdata);
}

// Подведение итогов функций is_any
void gadanieResults(float &bond_albedo, RowData& pdata) {
    std::vector<int> values = {is_Dwarf, is_EGP, is_EIP, is_ETP, is_EEP};
    std::vector<std::string> names = {"Это КК", "Это EGP", "Это EIP", "Это ETP", "Это EEP"};

    // Находим максимальное значение
    int max_index = 0;
    for (int i = 1; i < values.size(); ++i) {
        if (values[i] > values[max_index]) {
            max_index = i;
        }
    }

    // Проверяем, есть ли другие элементы с таким же максимальным значением
    bool is_ambiguous = false;
    for (int i = 0; i < values.size(); ++i) {
        if (i != max_index && values[i] == values[max_index]) {
            is_ambiguous = true;
            break;
        }
    }

    if (is_ambiguous) {
        std::cout << "Неопределенный тип\n" << std::endl;
    } else {
        std::cout << names[max_index] << std::endl; // Выводим максимальное значение
        if (values[max_index] == is_EGP) isThisTheSudarskyPlanet(bond_albedo, pdata);
        if (values[max_index] == is_EEP) isThisAnyOfEarth(pdata);
    }
}