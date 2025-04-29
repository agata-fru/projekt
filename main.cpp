#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <future>
#include <thread>

using namespace std;

class SamochodStack {
private:
    stack<double> zaladunek;
    double ladownosc;
    double obecny_zaladunek;

public:
    SamochodStack(double lad) : ladownosc(lad), obecny_zaladunek(0.0) {}

    bool MoznaZaladowac(double waga) const {
        return (obecny_zaladunek + waga) <= ladownosc;
    }

    void push(double waga) {
        zaladunek.push(waga);
        obecny_zaladunek += waga;
    }

    void print() const {
        stack<double> temp = zaladunek;
        cout << "/----|";
        while (!temp.empty()) {
            printf(" %.1f", temp.top());
            temp.pop();
            cout << " ";
        }
        cout << "\n|----|------------------------\n";
        cout << " o  o  o  o  o  o  o  o  o  o\n";
    }

    double pokazLadownosc() const { return ladownosc; }
    double pokazObecnyZaladunek() const { return obecny_zaladunek; }
};

double losowaWaga(double min = 0.2, double max = 10.0) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

double losowaLadownosc(double min = 10.0, double max = 20.0) {
    double raw = min + static_cast<double>(rand()) / RAND_MAX * (max - min);
    return static_cast<int>(raw);
}

queue<double> generujTasmociag(int liczbaPrzedmiotow, vector<double>& wagiPrzedmiotow) {
    srand(static_cast<unsigned>(time(0)) + std::hash<std::thread::id>{}(std::this_thread::get_id()));

    queue<double> tasmociag;
    for (int i = 0; i < liczbaPrzedmiotow; ++i) {
        double waga = losowaWaga();
        tasmociag.push(waga);
        wagiPrzedmiotow.push_back(waga);
    }
    return tasmociag;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int liczbaPrzedmiotow;
    cout << "Podaj liczbe przedmiotow na tasmociagu: ";
    cin >> liczbaPrzedmiotow;

    if (liczbaPrzedmiotow <= 0) {
        cout << "Liczba przedmiotow musi byc dodatnia!" << endl;
        return 1;
    }

    vector<double> wagiPrzedmiotow;
    vector<SamochodStack> samochody;
    vector<double> ladownosciSamochodow;

    future<queue<double>> przyszlyTasmociag = async(launch::async, generujTasmociag, liczbaPrzedmiotow, ref(wagiPrzedmiotow));

    queue<double> tasmociag = przyszlyTasmociag.get();

    SamochodStack obecneAuto(losowaLadownosc());
    ladownosciSamochodow.push_back(obecneAuto.pokazLadownosc());
    samochody.push_back(obecneAuto);

    while (!tasmociag.empty()) {
        double item = tasmociag.front();

        if (samochody.back().MoznaZaladowac(item)) {
            samochody.back().push(item);
            tasmociag.pop();
        }
        else {
            SamochodStack nowySamochod(losowaLadownosc());

            if (nowySamochod.MoznaZaladowac(item)) {
                nowySamochod.push(item);
                tasmociag.pop();
                ladownosciSamochodow.push_back(nowySamochod.pokazLadownosc());
                samochody.push_back(nowySamochod);
            }
        }
    }

    cout << "\nPrzedmioty na tasmociagu:\n\n";
    for (const auto& waga : wagiPrzedmiotow) {
        cout << fixed << setprecision(1) << waga << "  ";
    }

    cout << "\n";
    int dlugoscLinii = liczbaPrzedmiotow * 5;
    for (int i = 0; i < dlugoscLinii; ++i) {
        cout << "-";
    }
    cout << ">";
    cout << "\n\n";

    cout << "Pojemnosci ciezarowek:\n\n";
    for (size_t i = 0; i < ladownosciSamochodow.size(); ++i) {
        cout << "n" << i + 1 << "=" << static_cast<int>(ladownosciSamochodow[i]) << " ";
    }
    cout << "\n\n";

    cout << "Zapakowane ciezarowki:\n\n";
    for (size_t i = 0; i < samochody.size(); ++i) {
        cout << "n" << i + 1 << " = " << static_cast<int>(samochody[i].pokazLadownosc()) << endl;
        samochody[i].print();
        cout << endl;
    }

    return 0;
}

