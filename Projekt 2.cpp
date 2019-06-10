#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
#include <chrono>

#define L_GRAF 10         /// ILE GRAFÓW
#define L_WIERZ 150      ///  ILE WIERZCHOLKÓW W GRAFIE
#define GESTOSC 1       ///   GESTOSC GRAFU

using namespace std;
using namespace std::chrono;


struct dane {
	long int dystans; // POZWALA STWIERDZIC CZY WIERZCHOLEK JEST NIEODWIEDZONY LUB CZY JEST WIERZCHOLKIEM, W KTYRM SIE AKTUALNIE ZNAJDUJEMY
	bool odwiedzony; //  OZNACZA WIERZCHOLEK JAKO WCZESNIEJ ODWIEDZONY
};

struct element_listy {
	element_listy * nastepny;  //STRUKTURA POZWALA ZBUDWOWAC LISTE GDZIE KAZDE OGNIWO PROWADZI DO KOLEJNEGO
	long int wezel;           //ORAZ ZAWIERA INFORMACJE O SWOIM NUMERZE I W
	long int waga;
};

class Graf
{
private:

	long int *droga;
	long int **macierz_grafu;

	long int Wierzcholki;
	long int Krawedzie;
	long int Start;

	element_listy *element;
	element_listy **Lista;

public:
	Graf() {};

	Graf(long int Wierzcholki ,long int Krawedzie,long int Start)
	{
    this->Wierzcholki = Wierzcholki;
	this->Krawedzie = Krawedzie;
	this->Start = Start;

	macierz_grafu = new long int*[Wierzcholki];

	for (int i = 0; i < Wierzcholki; i++)
	{
		macierz_grafu[i] = new long int[Wierzcholki];
    }

	for (int i = 0; i < Wierzcholki; i++)
        {
        for (int j = 0; j < Wierzcholki; j++)
            {
            macierz_grafu[i][j] = 0;
            }
        }

	Lista = new element_listy*[Wierzcholki];

	for (int i = 0; i < Wierzcholki; i++)
        {
        Lista[i] = NULL;
        }

	}

	~Graf();


	long int dodaj_krawedz(int i, int j, int waga) //FUNKCJA WYPELNIAJACA MACIERZ I LISTE
	{
	    element = new element_listy;
        element->wezel = j;
        element->waga = waga;
        element->nastepny = Lista[i];
        Lista[i] = element;

	return macierz_grafu[i][j] = waga;
	}

	void wyswietl() ///FUNKCJA WYSWIETLAJACA GRAF W WERSJI MACIERZOWEJ
	{
	    for (int i = 0; i < Wierzcholki; i++)
	{
		for (int j = 0; j < Wierzcholki; j++)
		{
			std::cout << macierz_grafu[i][j] << "\t";
		}
		std::cout << endl;
	}
	}

	void wyswietl_liste() ///FUNKCJA WYSWIETLAJACA GRAF W WERSJI LISTOWEJ
	{
	    element_listy *tmp;
        for (int i = 0; i < Wierzcholki; i++)
        {
            tmp = Lista[i];
            cout << i << ":";
            while (tmp)
            {
                cout << tmp->wezel << " waga :" << tmp->waga<<" ";
                tmp = tmp->nastepny;
            }
            cout << endl;
        }
	}

	int najkrotsza_droga(dane* tab)  /// SZUKANIE WIERZCHOLKA O NAJKROTSZEJ DRODZE
	{
	   long int min = -1;
	long int mindist = 1000000;
	for (int i = 0; i < Wierzcholki; i++) {
		if (!tab[i].odwiedzony && tab[i].dystans < mindist) {
			min = i;
			mindist = tab[i].dystans;
		}
	}
	return min;
	}


	dane* Dijkstra() ///ALGORYTM DJIKSTRY DLA MACIERZY
	{
	    dane* tab = new dane[Wierzcholki];
        droga = new long int[Wierzcholki];
        for (int i = 0; i < Wierzcholki; i++)
            {
            if (i == Start)
                {
                    tab[i].dystans = 0;  ///AKTUALNY WIERZCHOLEK MA DROGE = 0
                }
                else
                    {
                        tab[i].dystans = 1000000; ///POZOSTALE WIERZCHOLKI MAJA NIESKONCZONA DROGE
                    }
		tab[i].odwiedzony = false; ///USTAWIAMY WIERZCHOLEK JAKO NIEDOWIEDZONY
		droga[i] = -1;
            }

	long int min = najkrotsza_droga(tab); ///SZUKAMY WIERZCHOLKA O NAJKROTSZEJ DRODZE

        while (min != -1)
            {
            tab[min].odwiedzony = true; ///WIERZCHOLEK OZNACZAMY JAKO ODWIEDZONY

            for (int i = 0; i < Wierzcholki; i++)
            {
                if (macierz_grafu[min][i] > 0 && tab[min].dystans + macierz_grafu[min][i] < tab[i].dystans)
                {
                    tab[i].dystans = tab[min].dystans + macierz_grafu[min][i]; ///GLOWNA CZESC ALGORYTMU
                    droga[i] = min;
                }
            }
		min = najkrotsza_droga(tab);
            }
	return tab;
	}

	dane* Dijkstra_Lista() ///OPERACJE ANALOGICZNE JAK DLA MACIERZY
	{
	    dane* tab = new dane[Wierzcholki];
        droga = new long int[Wierzcholki];
        element_listy *pomocniczy;

        for (int i = 0; i < Wierzcholki; i++) {
            if (i == Start)
            {
                tab[i].dystans = 0;
            }
                else
                    {
                        tab[i].dystans = 1000000;
                    }
		tab[i].odwiedzony = false;
		droga[i] = -1;
	}

	long int min = najkrotsza_droga(tab);

	while (min != -1)
        {
		tab[min].odwiedzony = true;
		pomocniczy = Lista[min];

		for (pomocniczy = Lista[min]; pomocniczy; pomocniczy = pomocniczy->nastepny)
		{
			if (tab[min].dystans + pomocniczy->waga < tab[pomocniczy->wezel].dystans)
                {
				tab[pomocniczy->wezel].dystans = tab[min].dystans + pomocniczy->waga;

				droga[pomocniczy->wezel] = min;
                }

		}
		min = najkrotsza_droga(tab);

        }
	return tab;
	}


	void zapisz( dane *tab) ///ZAPISYWANIE DO PLIKU INFORMACJI O DROGACH I ICH KOSZTACH
	{
    long int licznik = 0;
	long int *Tab_drogi = new long int[Wierzcholki];
	ofstream plik;
	plik.open("Wyniki.txt");
        for (int i = 0; i < Wierzcholki; i++)
        {
            plik << i << ":" ;
                for (int j = i; j > -1; j = droga[j])
                    {
                    Tab_drogi[licznik++] = j;
                    }
		while (licznik)
            plik << Tab_drogi[--licznik] << "->";
            plik << "( "<< tab[i].dystans << ") " << endl;

	}
	plik.close();
	}
};
Graf::~Graf(){}

void Wczytaj(Graf *&_Graf) ///POBIERANIE DANYCH O GRAFIE Z PLIKU
{
	long int Krawedzie;
	long int Wierzcholki;
	long int Start;
	long int wierz_poccz;
	long int wierz_kon;
	long int waga;

	long int warosc;
	ifstream plik;


    plik.open("PLIK_Z_GRAFEM.txt");
	if (plik.good())
	{
		plik >> Krawedzie;
		plik >> Wierzcholki;
		plik >> Start;
		_Graf = new Graf(Wierzcholki, Krawedzie, Start);
		while (!plik.eof())
		{
			plik >> wierz_poccz;
			plik >> wierz_kon;
			plik >> waga;

			_Graf->dodaj_krawedz(wierz_poccz, wierz_kon, waga);
		}

	}
	plik.close();
}

void generuj() ///GENEROWANIE NOWEGO GRAFU ZGODNIE Z Z¥DANIAMI
{
	long int Krawedzie;
	long int Wierzcholki;
	long int Start;
	long int licznik=0;
	double gestosc;

	ofstream plik;
	int waga;

	plik.open("PLIK_Z_GRAFEM.txt");

	Wierzcholki = L_WIERZ;
	gestosc = GESTOSC;
	Start = rand() % (L_WIERZ-1) + 1;
	Krawedzie = ((Wierzcholki*(Wierzcholki - 1)) / 2)*gestosc;

		plik << Krawedzie << " " << Wierzcholki << " " << Start << endl;

        srand (time(nullptr));
		for (int i = 0; i < Wierzcholki; i++)
			for (int j = i; j < Wierzcholki; j++)
			{
				if (i != j && licznik < Krawedzie)
				{
					waga = rand() % 5000 + 1;
					plik << i << " " << j << " " << waga << endl;
					plik << j << " " << i << " " << waga << endl;
					licznik++;
				}
			}

	plik.close();

}

int main()
{
    Graf *_Graf = NULL;

		   ofstream pomiary;
		for(int i = 0; i < L_GRAF; i++)
            {
		     generuj(); ///GENEROWANIE NOWEGO GRAFU
             Wczytaj(_Graf); ///WCZYTANIE GRAFU Z PLIKU

             high_resolution_clock::time_point t1 = high_resolution_clock::now();

             //dane* tab = _Graf->Dijkstra(); ///WYBOR ALGORYTMU DLA DANEGO GRAFU
            dane* tab = _Graf->Dijkstra_Lista(); ///ZAPISYWANIE WYNIKU ALGORYTMU DO POMOCNICZEJ TABLICY

			 _Graf->zapisz(tab); ///ZAPISANIE INFORMACJI O DROGACH I WAGACH

			 high_resolution_clock::time_point t2 = high_resolution_clock::now();
			 auto duration = duration_cast<microseconds>( t2 - t1 ).count();
            pomiary.open ("BADANIE_lISTA_010.csv",ios::app); ///ZAPISYWANIE CZASU WYKONANIA ALGORYTMU
            pomiary << duration <<";"<< "\n";
            pomiary.close();
            }


	return 0;
}
