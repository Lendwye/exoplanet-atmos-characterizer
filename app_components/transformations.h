
// Перевод из астрономических значений в СИ
void fromSourceToSI(RowData &pdata) {
    pdata.orbital_period = pdata.orbital_period * sec_per_day;
    pdata.semi_major_axis = pdata.semi_major_axis * au_to_meters;
    pdata.mass = pdata.mass * jupiter_mass;
    pdata.radius = pdata.radius * jupiter_radius_m;
    pdata.star_mass = pdata.star_mass * sun_mass;
}

// Перевод из СИ в астрономические значений
void fromSIToSource(RowData &pdata) {
    pdata.orbital_period = pdata.orbital_period / sec_per_day;
    pdata.semi_major_axis = pdata.semi_major_axis / au_to_meters;
    pdata.mass = pdata.mass / jupiter_mass;
    pdata.radius = pdata.radius / jupiter_radius_m;
    pdata.star_mass = pdata.star_mass / sun_mass;
}

// Получение T, a, Mj, Ms
void getKeplerThird(RowData &pdata) {
    int unknown_count = 0; // количество неизвестных
    if (pdata.orbital_period <= 0) {
        unknown_count++;
        std::cout << "KT: T - ? | ";
    }
    if (pdata.semi_major_axis <= 0) {
        unknown_count++;
        std::cout << "KT: a - ? | ";
    }
    if (pdata.mass <= 0) {
        unknown_count++;
        std::cout << "KT: pM - ? | ";
    }
    if (pdata.star_mass <= 0) {
        unknown_count++;
        std::cout << "KT: sM - ? | ";
    }
    if (unknown_count == 0) {
        std::cout << colTxt("KT - ✓", 32).str() << std::endl;
    } else if (unknown_count == 1) {
        const double G = 6.67430e-11; // гравитационная постоянная
        // Преобразуем входные данные в СИ
        fromSourceToSI(pdata);
        if (pdata.orbital_period <= 0) {
            // Вычисляем период T
            double squared_period = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * (pdata.star_mass + pdata.mass));
            pdata.orbital_period = sqrt(squared_period); // Извлекаем квадратный корень
            fromSIToSource(pdata);                       // Преобразуем результат в нужные единицы
            std::cout << "KT: T = " << pdata.orbital_period << " дней" << std::endl;
        } else if (pdata.semi_major_axis <= 0) {
            // Вычисляем большую полуось a
            pdata.semi_major_axis = cbrt((G * (pdata.star_mass + pdata.mass) * pdata.orbital_period * pdata.orbital_period) / (4 * M_PI * M_PI));
            fromSIToSource(pdata);
            std::cout << "KT: a = " << pdata.semi_major_axis << " а.е." << std::endl;
        } else if (pdata.mass <= 0) {
            // Вычисляем массу планеты
            pdata.mass = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * pdata.orbital_period * pdata.orbital_period) - pdata.star_mass;
            fromSIToSource(pdata);
            std::cout << "KT: M = " << pdata.mass << " Mj" << std::endl;
        } else if (pdata.star_mass <= 0) {
            // Вычисляем массу звезды
            pdata.star_mass = (4 * M_PI * M_PI * pow(pdata.semi_major_axis, 3)) / (G * pdata.orbital_period * pdata.orbital_period) - pdata.mass;
            fromSIToSource(pdata);
            std::cout << "KT: SM = " << pdata.star_mass << " Msun" << std::endl;
        }
    } else {
        std::cout << colTxt("KT - ×", 31).str() << std::endl;
    }
}

// Получение Rj, surface_gravity, Mj
void getRGM(RowData &pdata) {
    int unknown_count = 0; // количество неизвестных

    // Проверяем, какие параметры неизвестны
    if (pdata.radius < 0) {
        unknown_count++;
        std::cout << "RGM: R - ? | ";
    }
    if (pdata.log_g < 0) {
        unknown_count++;
        std::cout << "RGM: g - ? | ";
    }
    if (pdata.mass < 0) {
        unknown_count++;
        std::cout << "RGM: M - ? | ";
    }

    if (unknown_count == 0) {
        std::cout << colTxt("RGM - ✓", 32).str() << std::endl;
    } else if (unknown_count == 1) {
        const double G = 6.67430e-11; // гравитационная постоянная

        // Преобразуем входные данные в СИ
        fromSourceToSI(pdata);

        if (pdata.log_g < 0) {
            // Вычисляем ускорение свободного падения g
            pdata.surface_gravity = (G * pdata.mass) / pow(pdata.radius, 2);
            std::cout << "RGM: g = " << pdata.surface_gravity << " м/с^2" << std::endl;
        } else if (pdata.radius < 0) {
            // Вычисляем радиус R
            pdata.surface_gravity = pow(10, pdata.log_g); // log_g = log10(g)
            pdata.radius = std::sqrt(G * pdata.mass / pdata.surface_gravity);
            std::cout << "RGM: R = " << pdata.radius / jupiter_radius_m << " Rj" << std::endl;
        } else if (pdata.mass < 0) {
            // Вычисляем массу M
            pdata.surface_gravity = pow(10, pdata.log_g); // log_g = log10(g)
            pdata.mass = (pdata.surface_gravity * pow(pdata.radius, 2)) / G;
            std::cout << "RGM: M = " << pdata.mass / jupiter_mass << " Mj" << std::endl;
        }

        // Преобразуем результаты в нужные единицы
        fromSIToSource(pdata);
    } else {
        std::cout << colTxt("RGM - ×", 31).str() << std::endl;
    }
}

// Избавление от pdata.temp_any
void getTempAny(RowData &pdata) {
    if (pdata.temp_calculated > -274)
        temp_any = pdata.temp_calculated;
    if (pdata.temp_measured > -274)
        temp_any = pdata.temp_measured; // бОльший приоритет измеренной температуре, нежели посчитанной по T_eff
    else std::cout << "Нет температуры\n";
}

void getBondAlbedo(RowData &pdata, float &bond_albedo) {
    bond_albedo = pdata.geometric_albedo * 0.743009549402362;
    // q = 0.816019098804724;
    // A = pq; q -- фазовый интеграл (средний на основе данных о телах СС)
}

void catchErrors() {
    // тут должен
}