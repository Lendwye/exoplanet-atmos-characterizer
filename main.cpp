#include "csv.h"
#include <algorithm>
#include <boost/math/special_functions/expint.hpp> // Boost library for expn
#include <cctype>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "PlanetData.h"
#include "app_components/constants.h"
#include "app_components/utils.h"
#include "app_components/classification.h"
#include "app_components/csv_parsing.h"
#include "app_components/vptd.h"
#include "app_components/transformations.h"
#include "app_components/data_output.h"



using namespace std;

int main() {
    do {
        clearScreen();
        RowData pdata; // Импорт данных из PlanetData.h
        float bond_albedo;
        getTempAny(pdata);
        getBondAlbedo(pdata, bond_albedo);
        menuSelect();
        menuAction(pdata, bond_albedo);
    } while (cin.get());
}
