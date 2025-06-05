#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include "lecturefichier.h"

#include <chrono>
#define LIBGRAPH2_LEVEL 4
#include <LibGraph2.h>
using namespace LibGraph2;

#include <iostream>
using namespace std;

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    //Affiche la fenêtre graphique avec une taille par défaut
    ILibGraph2* pLibgraph = GetLibGraph2();
    float hScreen = 600;
    float wScreen = 1100;
    float margin = 50;
    pLibgraph->show(CSize{ wScreen, hScreen });

    //Boucle principale d'événements
    evt e;
    auto tpStartBounce = chrono::high_resolution_clock::now();

    SAE_Datas_Heat data;
    auto b = data.getvector();


    //Choisir les affichages
    int mode = 0;
    bool lignes = 0;

	std::vector<CPoint> DonneesAfficher;


    //Création d'un stylo qui trace une ligne continue
    pen_DashStyles PenLine = pen_DashStyles::Solid;


	//Calculer le cadre de la fenêtre
    pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 2, PenLine);
    CPoint CadreP1{ margin, margin };
    CRectangle Rect{ CPoint{CadreP1.m_fX,CadreP1.m_fX+20}, CSize{ wScreen - 2 * margin , hScreen - 2 * margin } };




    ///////////////////////////
	std::vector<CPoint> TabDonneesMois;
	std::vector<CPoint> TabAnnee;
    float range = 0;
    CPoint MinMode1;
    CPoint MaxMode1{ 0,hScreen };

    CPoint MinMode3;
    CPoint MaxMode3{ 0,hScreen };

    for (int i = 0; i < b.size(); i++)
    {
        //Les points  pour chaques valeurs
        CPoint pt1{ CadreP1.m_fX + range+3 , hScreen/2 - (std::get<1>(b[i]) * 5) };
        if ( pt1.m_fY< MaxMode1.m_fY)
        {
			MaxMode1 = pt1;
        }
		if (MinMode1.m_fY <= pt1.m_fY)
		{
			MinMode1 = pt1;
		}   


        //Ne pas sortir du cadre
        if (pt1.m_fX >= CadreP1.m_fX + wScreen - 2 * margin )
        {
            break;
        }

		TabDonneesMois.push_back(pt1);
        range+=0.65;
    }

    ///////////////////////////////////
    std::vector<float> pYears;
    float startYear = (std::get<0>(b[0]) - 1) / 100;

    for (int i = 0; i < b.size();i++)
    {
        pYears.push_back(data.moyenneann(startYear + i));

    }

    range = 0;
    std::vector<CPoint> TabDonneesMois2;

    for (int i = 0; i <pYears.size(); i++)
    {
        //Les points et rectangles pour chaques valeurs
        CPoint pt1{ CadreP1.m_fX + range * 11, (hScreen / 2) - (pYears[i] * 10) };
        //Ne pas sortir du cadre
        if (pt1.m_fX >= CadreP1.m_fX + wScreen - 2 * margin)
        {
            break;
        }
        TabDonneesMois2.push_back(pt1);
        range+=0.716;
    }

    //////////////////////////////////
    range = 0;
    CPoint Min{ };
    CPoint Max{ };
    std::vector<CPoint> TabDonneesMois3;


    for (int i = 0; i < b.size(); i++)
    {
        //Les points et rectangles pour chaques valeurs
        CPoint pt1{ CadreP1.m_fX + range *11, (hScreen / 2 )- ( pYears[i])*10 };
        //Ne pas sortir du cadre
        if (pt1.m_fX >= CadreP1.m_fX + wScreen - 2 * margin)
        {
            break;
        }
        if (pt1.m_fY < MaxMode3.m_fY)
        {
            MaxMode3 = pt1;
        }
        if (MinMode3.m_fY < pt1.m_fY)
        {
            MinMode3 = pt1;
        }


 
        TabDonneesMois3.push_back(pt1);
        range+=0.72;
    }
    int annee;



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
    while (pLibgraph->waitForEvent(e))
    {

        switch (e.type)  //En fonction du type du dernier événement
        {
        case evt_type::evtRefresh:
            pLibgraph->beginPaint(); //Préparer la fenêtre pour le dessin

            //Afficher le cadre et l'abscisse
            pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 2, PenLine);
            pLibgraph->drawRectangle(Rect);


            //Afficher les conmmandes
            pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
            pLibgraph->setFont(CString{ "Consolas" }, 10, font_styles::FontStyleRegular);
            pLibgraph->drawString(CString{ "S : Précédent" }, CPoint{ 10, 5 });
            pLibgraph->drawString(CString{ "D : Suivant" }, CPoint{ 10, 17 });
            pLibgraph->drawString(CString{ "C : Console" }, CPoint{ 10, 29 });

            pLibgraph->drawString(CString{ "A : Mode Nuage de points" }, CPoint{ wScreen  - 200,5 });
            pLibgraph->drawString(CString{ "Z : Mode rayure chais plus" }, CPoint{ wScreen  - 200,  17 });
            pLibgraph->drawString(CString{ "E : Moyenne de temperatures par années" }, CPoint{ wScreen  - 200,    29 });
            pLibgraph->drawString(CString{ "R : Mode Choix d'année" }, CPoint{ wScreen  - 200,    41 });
            pLibgraph->setFont(CString{ "Consolas" }, 15, font_styles::FontStyleRegular);
            pLibgraph->drawString(CString{ "0°C" }, CPoint{ 8, hScreen / 2 - 13 });

            pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));

            if (mode == 0)
            {
				pLibgraph->setSolidBrush(MakeARGB(255,255, 255, 255));
				pLibgraph->drawRectangle(CRectangle{ CPoint{0,0},CSize{wScreen,hScreen} });
                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));

				pLibgraph->setFont(CString{ "Arial" }, 25, font_styles::FontStyleRegular);
				pLibgraph->drawString(CString{ "Choisissez un mode d'affichage..." }, CPoint{ wScreen / 2 - 220, 40 });
                pLibgraph->setFont(CString{ "Consolas" }, 10, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "S : Précédent" }, CPoint{ 10, 5 });
                pLibgraph->drawString(CString{ "D : Suivant" }, CPoint{ 10, 17 });
                pLibgraph->drawString(CString{ "C : Console" }, CPoint{ 10, 29 });
                pLibgraph->setFont(CString{ "Consolas" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "A : Mode Nuage de points" }, CPoint{ wScreen / 2 - 220, hScreen/2+50 });
                pLibgraph->drawString(CString{ "Z : Mode rayure chais plus" }, CPoint{ wScreen / 2 - 220, hScreen/2 + 70 });
                pLibgraph->drawString(CString{ "E : Moyenne de temperatures par années" }, CPoint{ wScreen / 2 - 220, hScreen/2 + 90 });
                pLibgraph->drawString(CString{ "R : Mode Choix d'année" }, CPoint{ wScreen / 2 - 220, hScreen/2 + 110 });
				pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));


            }



            /////////////////////////////////////////
            if (mode == 1)
            {
                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
				pLibgraph->setFont(CString{ "Arial" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Nuage de points" }, CPoint{ wScreen/2-100, 10 });

                pLibgraph->setFont(CString{ "Arial" }, 10, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Q : Afficher lignes" }, CPoint{ wScreen-300, 100 });
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));


                for (auto i : TabDonneesMois)
                {
					pLibgraph->setPen(MakeARGB(255, 200,200, 200),3,PenLine);
                    pLibgraph->drawEllipse(CRectangle{ i,CSize{1,1} });
                }
                if (lignes)
                {
                    for (int i = 0; i < TabDonneesMois.size() - 1; i++)
                    {
                        pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 1, PenLine);
                        pLibgraph->drawLine(TabDonneesMois[i], TabDonneesMois[i + 1]);
                    }
                }
                pLibgraph->setPen(MakeARGB(255, 255, 0, 0), 3, PenLine);
                pLibgraph->drawEllipse(CRectangle{ MaxMode1,CSize{1,1} });
                pLibgraph->setSolidBrush(MakeARGB(255,255, 0, 0));
                pLibgraph->setFont(CString{ "Consolas" }, 10, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Maximum : " + to_string((hScreen / 2 - MaxMode1.m_fY) / 5) + " °C" }, CPoint{ MaxMode1.m_fX,MaxMode1.m_fY-20});

                pLibgraph->setPen(MakeARGB(255, 0, 0, 255), 3, PenLine);
                pLibgraph->drawEllipse(CRectangle{ MinMode1,CSize{1,1} });
                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 255));
                pLibgraph->drawString(CString{ "Minimum : " + to_string((hScreen / 2 - MinMode1.m_fY) / 5) + " °C" }, CPoint{ MinMode1.m_fX,MinMode1.m_fY + 20 });
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));

            }

            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            if (mode == 2)
            {
                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
                pLibgraph->setFont(CString{ "Arial" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "L'Ensemble des températures" }, CPoint{ wScreen / 2 - 180, 10 });
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));



                for (int i = 0;i < TabDonneesMois.size();i++)
                {

                    if (TabDonneesMois[i].m_fY <= hScreen / 2)
                    {
                        pLibgraph->setPen(MakeARGB(255, 255, 75, 75), 2, PenLine);
                        pLibgraph->drawRectangle(CRectangle{ TabDonneesMois[i] , CSize{ 1,(hScreen / 2) - TabDonneesMois[i].m_fY } });
                    }
                    else
                    {
                        pLibgraph->setPen(MakeARGB(255, 75,75, 255), 2, PenLine);
                        pLibgraph->drawRectangle(CRectangle{ CPoint{TabDonneesMois[i].m_fX,hScreen / 2} , CSize{1,(TabDonneesMois[i].m_fY) - hScreen / 2} });
                    }

                }
            }
            
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////

            if (mode == 3)
            {
                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
                pLibgraph->setFont(CString{ "Arial" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Temp. Moy. par Années" }, CPoint{ wScreen / 2 - 180, 10 });
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));


                for (int i = 0;i < TabDonneesMois3.size();i++)
                {
                    pLibgraph->setPen(MakeARGB(255, ((hScreen / 2) - TabDonneesMois3[i].m_fY) * 3.2, 0, 0), 2, PenLine);
                    pLibgraph->drawEllipse(CRectangle{ TabDonneesMois3[i],CSize{4,4} });


                    pLibgraph->setPen(MakeARGB(255, 255, 0, 0), 3, PenLine);
                    pLibgraph->drawEllipse(CRectangle{ MaxMode3,CSize{1,1} });
                    pLibgraph->setSolidBrush(MakeARGB(255, 255, 0, 0));
                    pLibgraph->setFont(CString{ "Arial" }, 10, font_styles::FontStyleRegular);
                    pLibgraph->drawString(CString{ "Maximum : " + to_string((hScreen / 2 - MaxMode3.m_fY) / 10) + " °C" }, CPoint{ MaxMode3.m_fX-150,MaxMode3.m_fY - 20 });

                    pLibgraph->setPen(MakeARGB(255, 0, 0, 255), 3, PenLine);
                    pLibgraph->drawEllipse(CRectangle{ MinMode3,CSize{1,1} });
                    pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 255));
                    pLibgraph->drawString(CString{ "Minimum : " + to_string((hScreen/2-MinMode3.m_fY)/10) + " °C"}, CPoint{MinMode3.m_fX - 50,MinMode3.m_fY + 10});
                    pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));

                    if ((i + 6) % 10 == 0)
                    {
                        pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 1, PenLine);
                        CPoint pDate{ TabDonneesMois3[i].m_fX , hScreen - 50 };
                        pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));


                        pLibgraph->setFont(CString{ "Consolas" }, 10, font_styles::FontStyleRegular);
                        pLibgraph->drawString(CString{ to_string(1896 + i) }, CPoint{ TabDonneesMois3[i].m_fX - 18, hScreen - margin });
                        pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));
                        pLibgraph->drawLine(TabDonneesMois3[i], pDate);
                    }
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            if (mode == 4)
            {

                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
                pLibgraph->setFont(CString{ "Arial" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Choisissez une année..." }, CPoint{ wScreen / 2 - 160, 10 });
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));
                for (int i = 0;i < TabDonneesMois2.size();i++)
                {

                    pLibgraph->setPen(MakeARGB(255, 50, 50, 50), 1, PenLine);
                    pLibgraph->drawRectangle(CRectangle{ TabDonneesMois2[i],CSize{7,hScreen - TabDonneesMois2[i].m_fY - margin} });


                    if ((i + 6) % 10 == 0)
                    {
                        pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 1, PenLine);
                        CPoint pDate{ TabDonneesMois3[i].m_fX , hScreen - 50 };
                        pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));

                        pLibgraph->setFont(CString{ "Consolas" }, 10, font_styles::FontStyleRegular);
                        pLibgraph->drawString(CString{ to_string(1896 + i) }, CPoint{ TabDonneesMois3[i].m_fX - 18, hScreen - margin });
                        pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));
                    }
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////
            if (mode == 5)
            {

                pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 0));
                pLibgraph->setFont(CString{ "Arial" }, 20, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ to_string(annee)}, CPoint{wScreen / 2 - 60, 10});
                pLibgraph->setFont(CString{ "Arial" }, 10, font_styles::FontStyleRegular);
                pLibgraph->drawString(CString{ "Temp. Moy. : " + to_string(data.moyenneann(annee)) + "°C"}, CPoint{wScreen - 300, 90});
                pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));

                for (int i = 0;i < TabAnnee.size();i++)
                {
                    if (TabAnnee[i].m_fY <= hScreen / 2)
                    {
                        pLibgraph->setSolidBrush(MakeARGB(255, 200, 0, 0));
                        pLibgraph->setPen(MakeARGB(255, 255, 0, 0), 2, PenLine);
                        pLibgraph->drawRectangle(CRectangle{ TabAnnee[i] , CSize{ 83,(hScreen / 2) - TabAnnee[i].m_fY } });
                        
                    }
                    else
                    {
                        pLibgraph->setSolidBrush(MakeARGB(255, 0, 0, 200));
                        pLibgraph->setPen(MakeARGB(255, 0, 0, 255), 2, PenLine);
                        pLibgraph->drawRectangle(CRectangle{ CPoint{TabAnnee[i].m_fX,hScreen / 2} , CSize{83,(TabAnnee[i].m_fY) - hScreen / 2} });
                    }
                    pLibgraph->setSolidBrush(MakeARGB(0, 0, 0, 0));
                }



            }



            pLibgraph->setPen(MakeARGB(255, 0, 0, 0), 1, PenLine);
            pLibgraph->drawLine(CPoint{ margin,  hScreen / 2 }, CPoint{ wScreen - margin,  hScreen / 2 });





            pLibgraph->endPaint();
            break;

        case evt_type::evtKeyDown:  //Enfoncement d'une touche
            switch (e.vkKeyCode) //En fonction de la touche enfoncée
            {

            case 'C':
                std::cout << "Console ouverte" << std::endl;
                pLibgraph->showConsole();
                break;
            case 'D':

                if (mode == 5)
                {
                    mode = 4;
                    pLibgraph->askForRefresh();
                    break;
                }
                mode++;
                if (mode > 4)
                {
                    mode = 1;
                }

                pLibgraph->askForRefresh();
                break;

            case 'S':
                if (mode == 5)
                {
                    mode = 4;
                    pLibgraph->askForRefresh();
                    break;
                }
                mode--;
                if (mode <= 0)
                {
                    mode = 4;
                }
                pLibgraph->askForRefresh();
                break;

            case 'A':
				mode = 1;
                pLibgraph->askForRefresh();
                break;
            case 'Q':
                mode = 1;
                lignes = !lignes;
                pLibgraph->askForRefresh();
                break;
            case 'Z':
                mode = 2;   
                pLibgraph->askForRefresh();
                break;
            case 'E':
                mode = 3;
                pLibgraph->askForRefresh();
                break;
            case 'R':
                mode = 4;
                pLibgraph->askForRefresh();
                break;
            }
            break;



        case evt_type::evtMouseDown:  //Déplacement de la souris

            //Afficher une année précise
            if (mode == 5)
            {

                TabAnnee = {};
                range = 0;
                mode == 4;
            }

            if (mode == 4)
            {

                annee = startYear;
                mode = 5;
                for (int i = 0; i < TabDonneesMois2.size(); i++)
                {
                    if (!(e.x > TabDonneesMois2[i].m_fX and e.x < TabDonneesMois2[i].m_fX + 10))
                    {
                        annee++;
                    }
                    else
                    {
                        break;
                    }
                }

            }
            if (mode == 5)
            {

                TabAnnee = {};
                range = 0;
				for (int i=0; i<b.size();i++)
				{
					if (((std::get<0>(b[i]) - 1) / 100) == annee)
					{
						TabAnnee.push_back(CPoint{ CadreP1.m_fX + range*83+2 , hScreen / 2 - (std::get<1>(b[i]) * 10) });
                        range++;
					}
				}
            }

            pLibgraph->askForRefresh();
            break;
        }
    }

    ReleaseLibGraph2();
    return 0;
}