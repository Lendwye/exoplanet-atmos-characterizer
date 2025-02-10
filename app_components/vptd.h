// LASCIATE OGNE SPERANZA, VOI CH'ENTRATE   //
// - Данте Алигьери. Надпись на вратах Ада. //

double tpprofile(float &bond_albedo, float m, float m0, float t_int, float t_irr, float kappa_S, float kappa_0, float kappa_cia, float beta_S0, float beta_L0, float el1, float el3) {
    float albedo = bond_albedo;
    float kappa_L = kappa_0 + kappa_cia * m / m0; // при S > L - возникновение температурной инверсии
    float beta_S = kappa_S * m / beta_S0;         // параметр рассеяния
    float coeff1 = 0.25 * std::pow(t_int, 4);     // коэфф. перед первыми квадратными скобками
    float coeff2 = 0.125 * std::pow(t_irr, 4);    // коэфф. перед вторыми квадратными скобками
    float term1 =
        1.0 / el1 +
        m / (el3 * pow(beta_L0, 2)) *
            (kappa_0 + (kappa_cia * m) / (2 * m0));
    float term2 =
        1.0 / (2 * el1) +
        boost::math::expint(2, beta_S) *
            (kappa_S / (kappa_L * beta_S0) -
             (kappa_cia * m * beta_S0) / (el3 * kappa_S * m0 * pow(beta_L0, 2)));
    float term3 =
        kappa_0 * beta_S0 / (el3 * kappa_S * pow(beta_L0, 2)) *
        (1.0 / 3 - boost::math::expint(4, beta_S));
    float term4 =
        kappa_cia * pow(beta_S0, 2) / (el3 * m0 * pow(kappa_S, 2) * pow(beta_L0, 2)) *
        (0.5 - boost::math::expint(3, beta_S));
    float result = pow((coeff1 * term1 + coeff2 * (term2 + term3 + term4)), 0.25);
    return result;
}

void tpoutput(float &bond_albedo, RowData const &pdata) {
    float t_irr = pow(2, 0.5) * pdata.star_teff * pow((pdata.star_radius / (2 * pdata.semi_major_axis)), 0.5) * pow((1 - bond_albedo), 0.25);
    float g = pdata.surface_gravity * 1e2;                 // ускорение свободного падения в СГС
    float kappa_S0 = 0.85;                                 // коротковолновая прозрачность; ↗↗ - возникновение антипарникового эффекта
    float kappa_0 = 0.02;                                  // ИК и near-ИК прозрачность - влияет на kappa_L
    float kappa_cia = 0.9;                                 // CIA opacity normalization
    float beta_S0 = (1 - bond_albedo) / (1 + bond_albedo); // коротковолновый параметр рассеяния; ↗↗ - Ab ↘↘; весь TPP смещается на более низкие температуры из-за зависимости от (1 - Ab)
    float beta_L0 = 1.0;                                   // длинноволновый параметр рассеяния; ↘↘ - TPP смещается в более теплую область
    float el1 = 3.0 / 8.0;                                 // первый длинноволновый коэффициент Эддингтона
    float el3 = 1.0 / 3.0;                                 // второй длинноволновый коэффициент Эддингтона

    std::vector<double> logp; // логарифм p
    std::vector<double> pressure;
    std::vector<double> m;
    double bar2cgs = 1e6; // бар -> СГС единицы

    for (float p = -5.0; p <= 2.01; p += 0.01) {
        logp.push_back(p);
    } // добавление значения десятичного логарифма давления в массив logp
    for (double logp_val : logp) {
        pressure.push_back(std::pow(10.0, logp_val));
    } // преобразование lg в числовое значение и запись в pressure
    float p0 = *std::max_element(pressure.begin(), pressure.end());
    float m0 = p0 * bar2cgs / g;
    for (float p : pressure) {
        m.push_back(p * bar2cgs / g);
    } // масса атмосферной колонны
    std::vector<double> tp_prof(m.size());
    for (size_t i = 0; i < m.size(); ++i) {
        tp_prof[i] = tpprofile(bond_albedo, m[i], m0, 150, t_irr, kappa_S0, kappa_0, kappa_cia, beta_S0, beta_L0, el1, el3);
    }

    if (pdata.name == "15 Sge b" ||
        pdata.name == "HD 209458 b" ||
        pdata.name == "HAT-P-49 b" ||
        pdata.name == "2MASS J0337-1758") {
        std::ofstream latest("profiles/tpp_" + pdata.name + ".dat");
        for (size_t i = 0; i < m.size(); ++i) {
            latest << tp_prof[i] << std::setw(15) << pressure[i] << std::endl; // запись температуры/давления
        }
        latest.close();
    } else {
        std::ofstream latest("profiles/tpp_latest.dat");
        for (size_t i = 0; i < m.size(); ++i) {
            latest << tp_prof[i] << std::setw(15) << pressure[i] << std::endl; // запись температуры/давления
        }
        latest.close();
    }
}
