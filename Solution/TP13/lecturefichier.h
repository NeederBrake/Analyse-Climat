#pragma once

#include <vector> // std::vector
#include <string> // std::string
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map> // std::map
#include <utility> // std::pair
#include <set>

class SAE_Datas_Heat {
    // Private Datas
    std::vector<std::pair<int, float>> _vData;
    void convertDataInList();
public:
    SAE_Datas_Heat();
    float findtempfordate(float a) const;
    std::vector<std::pair<int, float>> getvector();
    float moyennemois30ans(int year, int mois) const;
    float moyenneann(int mois) const;
    float moyenneann30ans(int mois) const;
    float moyennetri(int date) const;
    float moyennetri30ans(int ann, int tri) const;

    int moislepluschaud() const;
    int moislemoinschaud() const;


	float moyenneglobale() const;
	std::vector<std::pair<int, float>> getData() const { return _vData; }
	void setData(std::vector<std::pair<int, float>> data) { _vData = data; }
};