#include <algorithm>    // std::find
#include <numeric> // std::accumulate
#include <iostream>
#include "lecturefichier.h"

void SAE_Datas_Heat::convertDataInList()
{
    std::vector<std::pair<int, float>> data;

    // Ouvre le fichier CSV en mode lecture
    std::ifstream file("aigoual_Mensuel.csv");

    // Vérifie si le fichier a bien été ouvert
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }

    std::string line;
    // Lire chaque ligne du fichier
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int col1;
        float col2;

        // Lire les deux colonnes séparées par une virgule et les convertir en float
        if (ss >> col1) {
            ss.ignore(); // Ignore la virgule
            if (ss >> col2) {
                // Créer une paire avec les deux colonnes de type float et l'ajouter au vecteur
                data.push_back(std::pair<int, float>(col1, col2)); // Explicitement type `std::pair<float, float>`
            }
        }
    }
    _vData = data;
    // Fermer le fichier
    file.close();
}

SAE_Datas_Heat::SAE_Datas_Heat() {
    convertDataInList();
}

float SAE_Datas_Heat::findtempfordate(float a) const
{
    bool trouve = false; //bool si trouvé
    for (int i = 0; i < _vData.size(); i++)
    {
        if (a == std::get<0>(_vData[i])) //si date trouvée
        {
            return std::get<1>(_vData[i]); // renvoie la température
            trouve = true;
        }
        if (i == _vData.size() - 1 && trouve == false)
        {
            return 1000; // valeur sentinelle (apparemment c'est comme ça que sa s'appelle ) en cas de mois manquant
        }
    }
}

std::vector<std::pair<int, float>> SAE_Datas_Heat::getvector()
{
    return _vData; //renvoie le vecteur _vData
}

float SAE_Datas_Heat::moyennemois30ans(int year, int mois) const
{
    auto deb = year * 100 + mois; //départ
    float moyenne = 0;
    float manquant = 0; //mois manquants 
    for (int i = 0; i < 30; i++) // 30 ans
    {
        moyenne += findtempfordate(deb);
        deb += 100; //avance d'un ans
        if (deb > 202200)
        {
            std::cout << "annee de depart trop recente";
            return 0;
        }
        if (moyenne >= 1000) //détection de la valeur sentinelle
        {
            manquant += 1;
            moyenne -= 1000;
        }
    }
    moyenne = moyenne / (30 - manquant); // moyenne en enlevant les mois manquants
    return moyenne;
}

float SAE_Datas_Heat::moyenneann(int year) const
{
    auto deb = year * 100 + 1; // commence en janvier
    float moyenne = 0;
    int manquant = 0;
    for (int i = 0; i < 11; i++)
    {
        moyenne += findtempfordate(deb);
        deb += 1;
        if (moyenne >= 1000) //détection de la valeur sentinelle
        {
            manquant += 1;
            moyenne -= 1000;
        }
    }
    moyenne = moyenne / (12 - manquant); // moyenne en enlevant les mois manquants
    return moyenne;
}

float SAE_Datas_Heat::moyenneann30ans(int year) const
{
    float moyenne = 0;

    for (int i = 0; i < 30; i++)
    {
        moyenne += moyenneann(year);
        year += 1;
        if (year > 2022) // si l'année dépasse 2022
        {
            std::cout << "annee trop recente";
            return 0;
        }
    }
    moyenne = moyenne / 30;
    return moyenne;
}

float SAE_Datas_Heat::moyennetri(int date) const
{
    float moyennetri = 0;
    auto mon = date;
    float manquant = 0;
    for (int i = 0; i < 2; i++) // sur 3 mois
    {
        moyennetri += findtempfordate(date);
        mon++;
        if (moyennetri >= 1000) //détection de la valeur sentinelle
        {
            manquant += 1;
            moyennetri -= 1000;
        }
    }
    moyennetri = moyennetri / (3 - manquant);
    return moyennetri;
}

float SAE_Datas_Heat::moyennetri30ans(int ann, int tri) const
{
    float moyenne = 0;
    int year;
    switch (tri)
    {
    case(1):
        year = ann * 100 + 1; // commmence en janvier
        for (int i = 0; i < 30; i++)
        {
            moyenne += moyennetri(year);
            year += 100;
            if (year > 202200)
            {
                std::cout << "annee trop recente";
                return 0;
            }
        }
        break;
    case(2):
        year = ann * 100 + 4; // commmence en avril
        for (int i = 0; i < 30; i++)
        {
            moyenne += moyennetri(year);
            year += 100;
            if (year > 202200)
            {
                std::cout << "annee trop recente";
                return 0;
            }
        }
        break;
    case(3):
        year = ann * 100 + 7; // commmence en juillet
        for (int i = 0; i < 30; i++)
        {
            moyenne += moyennetri(year);
            year += 100;
            if (year > 202200)
            {
                std::cout << "annee trop recente";
                return 0;
            }
        }
        break;
    case(4):
        year = ann * 100 + 10; // commmence en octobre
        for (int i = 0; i < 30; i++)
        {
            moyenne += moyennetri(year);
            year += 100;
            if (year > 202200)
            {
                std::cout << "annee trop recente";
                return 0;
            }
        }
        break;
    }
    moyenne = moyenne / 30;
    return moyenne;
}

float SAE_Datas_Heat::moyenneglobale() const
{
    float deb = (std::get<0>(_vData[0]) - 1) / 100; // premier mois
    float fin = (std::get<0>(_vData.back()) - 12) / 10; // dernier mois 
    float moyenne = 0;
    while (deb <= fin)
    {
        moyenne += moyenneann(deb);
        deb++;
    }
    moyenne = moyenne / _vData.size();
    return moyenne;
}

int SAE_Datas_Heat::moislepluschaud() const
{
    auto max = std::get<1>(_vData[0]);
    auto maxdate = std::get<0>(_vData[0]);
    for (auto i : _vData)
    {
        if (max < std::get<1>(i))
        {
            max = std::get<1>(i);
            maxdate = std::get<0>(i);
        }
    }

    return maxdate;
}

int SAE_Datas_Heat::moislemoinschaud() const
{
    auto min = std::get<1>(_vData[0]);
    auto mindate = std::get<0>(_vData[0]);
    for (auto i : _vData)
    {
        if (min > std::get<1>(i))
        {
            min = std::get<1>(i);
            mindate = std::get<0>(i);
        }
    }

    return mindate;
}