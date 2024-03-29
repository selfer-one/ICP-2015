/**
* @author xkondr01 xkucha20
* @file herni_plan.cpp
* @brief Definice herniho planu a funkci s nim spojenych
*/
#include "herni_plan.h"
#include "time.h"
//#include <QDebug>

//Inicializace herniho planu
void HerniPlan::inicializace(int vel,int predmetu, int bodu){
    gui = false;
    pocetBodu = bodu;
    pocetPredmetu = predmetu;
    velikost = vel;
	vlozenePolicko = "F";
	lPole = 4;
	tPole = 0;
	iPole = 0;
	
	/*mapa = (Policko**)malloc(velikost * sizeof(Policko*));
	for(int r = 0; r <= velikost; r++){
		mapa[r] = (Policko*)malloc(velikost * sizeof(Policko));
	}*/
	//pevna policka
	mapa[0][0].druh = 0;
	mapa[0][0].otoceni = 1;
	mapa[0][velikost-1].druh = 0;
	mapa[0][velikost-1].otoceni = 2;
	mapa[velikost-1][0].druh = 0;
	mapa[velikost-1][0].otoceni = 0;
	mapa[velikost-1][velikost-1].druh = 0;
	mapa[velikost-1][velikost-1].otoceni = 3;

	//vytvoreni herniho planu
	int pocet_poli = (velikost*velikost)/3;
	for(int i = 0; i < velikost; i++){
		for(int j = 0; j < velikost; j++){
			//preskoceni rohu
			if((i == 0 && j == 0) || (i == 0 && j == velikost-1) || 
				(i == velikost-1 && j == 0) || (i == velikost-1 && j == velikost-1)) continue;
			//pevna policka
			if(i % 2 == 0 && j % 2 == 0) {
				tPole++;
				mapa[i][j].druh = 1;
				if(i == 0) mapa[i][j].otoceni = 0;
				else if(j == 0) mapa[i][j].otoceni = 3;
				else if(i == velikost-1) mapa[i][j].otoceni = 2;
				else if(j == velikost-1) mapa[i][j].otoceni = 1;
				else mapa[i][j].otoceni = rand() % 3;
				continue;
			}
			int nahoda;
			while(1){
				nahoda = rand() % 3;
				//zajisteni co nejrovnomernejsiho rozlozeni poli
				if(nahoda == 0 && lPole <= pocet_poli) break;
				else if(nahoda == 1 && tPole <= pocet_poli) break;
				else if(nahoda == 2 && iPole <= pocet_poli) break;
			}
			mapa[i][j].druh = nahoda;
			mapa[i][j].otoceni = rand() % 4;
			if(mapa[i][j].druh == 0) lPole++;
			else if(mapa[i][j].druh == 1) tPole++;
			else iPole++;
		}
	}
	//volne policko
	if(lPole < tPole && lPole < iPole) volne.druh = 0;
	else if(tPole < lPole && tPole < iPole) volne.druh = 1;
	else volne.druh = 2;
	volne.otoceni = rand() % 4;
	//predmety
    for(int i = 0; i < 24; i++){
        predmety[i] = herni_predmety(i);
		volnePredmety[i] = predmety[i];
	}
	volne.predmet = -1;
	for(int i = 0; i < velikost; i++){
		for(int j = 0; j < velikost; j++){
			mapa[i][j].predmet = -1;
		}
	}
	int radek;
	int sloupec;
	//rozmisteni predmetu
    for(int i = 0; i < pocetPredmetu; ){
		//umisteni jednoho predmetu na volne policko
		if(i == 17){
			volne.predmet = i;
			i++;
			continue;
		}
		//nahodne vybrani souradnic
		radek = rand() % velikost;
		sloupec = rand() % velikost;
		if((radek == 0 && sloupec == 0) || (radek == 0 && sloupec == velikost-1) ||
			(radek == velikost-1 && sloupec == 0) || (radek == velikost-1 && sloupec == velikost-1)) continue;
		if(mapa[radek][sloupec].predmet >= 0) continue;
		else mapa[radek][sloupec].predmet = i;
		i++;
	}
}
//Inicializace ulozeneho herniho planu
void HerniPlan::inicializace_ulozena(int vel,int predmetu, int bodu){
	velikost = vel;
    pocetBodu = bodu;
    pocetPredmetu = predmetu;
	/*mapa = (Policko**)malloc(velikost * sizeof(Policko*));
	for(int r = 0; r <= velikost; r++){
		mapa[r] = (Policko*)malloc(velikost * sizeof(Policko));
	}*/
}
//Nastavi, ze se pouziva graficka verze programu
void HerniPlan::je_gui(){
    gui = true;
}

//Nastavi druh, otoceni a predmet policka
void HerniPlan::nastav_policko(int x, int y, int druh, int otoceni, int predmet){
	mapa[x][y].druh = druh;
	mapa[x][y].otoceni = otoceni;
	mapa[x][y].predmet = predmet;
}
//Nastavi druh, otoceni a predmet volneho policka
void HerniPlan::nastav_volne_policko(int druh, int otoceni, int predmet){
	volne.druh = druh;
	volne.otoceni = otoceni;
	volne.predmet = predmet;
}
//Nastavi predmet ziskany z ulozene hry do pole predmetu
void HerniPlan::nastav_predmet(int i, string predmet, string volnyPredmet){
	predmety[i] = predmet;
	volnePredmety[i] = volnyPredmet;
}
//Prirazeni nahodneho predmetu hraci
void HerniPlan::prirad_predmet(Hrac *hrac){
	int nahoda;
	while(1){
        nahoda = rand() % pocetPredmetu;
		if(volnePredmety[nahoda] != ""){
			hrac->nastav_predmet(nahoda);
			volnePredmety[nahoda] = "";
			break;
		}
		
	}
}
//Ulozene herni predmety v unicode
string HerniPlan::herni_predmety(int cislo){
	switch (cislo){
		case 0:
            return "\u27B4";
		case 1:
            return "\u2606";
		case 2:
            return "\u260F";
		case 3:
            return "\u2602";
		case 4:
            return "\u2600";
		case 5:
            return "\u262C";
		case 6:
            return "\u2620";
		case 7:
            return "\u2658";
		case 8:
            return "\u2704";
		case 9:
            return "\u272A";
		case 10:
            return "\u2652";
		case 11:
            return "\u2709";
		case 12:
            return "\u2707";
		case 13:
            return "\u2654";
		case 14:
            return "\u2708";
		case 15:
            return "\u270E";
		case 16:
            return "\u271A";
		case 17:
            return "\u2625";
		case 18:
            return "\u273F";
		case 19:
            return "\u2731";
        case 20:
            return "\u2646";
        case 21:
            return "\u2648";
        case 22:
            return "\u2624";
        case 23:
            return "\u2653";
		default:
			return "CHYBA";
	}
}
//Vykresleni stredu policka
void HerniPlan::vykresli_stred(int postava, int predmet, int zacatek){
	switch(postava){
		case 0:
            cout << "\x1B[31m\u263B\x1B[0m";
			break;
		case 1:
            cout << "\x1B[32m\u263B\x1B[0m";
			break;
		case 2:
            cout << "\x1B[36m\u263B\x1B[0m";
			break;
		case 3:
            cout << "\x1B[33m\u263B\x1B[0m";
			break;
		case 4:
            cout << "\u263B";
			break;
		default:
			if(zacatek == 0) cout << "\x1B[31m\u2591\x1B[0m";
			else if(zacatek == 1) cout << "\x1B[32m\u2591\x1B[0m";
			else if(zacatek == 2) cout << "\x1B[36m\u2591\x1B[0m";
			else if(zacatek == 3) cout << "\x1B[33m\u2591\x1B[0m";
			else cout << "\u2591";
			break;
	}
	if(predmet >= 0) cout << predmety[predmet];
	else {
		if(zacatek == 0) cout << "\x1B[31m\u2591\x1B[0m";
		else if(zacatek == 1) cout << "\x1B[32m\u2591\x1B[0m";
		else if(zacatek == 2) cout << "\x1B[36m\u2591\x1B[0m";
		else if(zacatek == 3) cout << "\x1B[33m\u2591\x1B[0m";
		else cout << "\u2591";
	}
}
//Posune hracem o jedno pole
int HerniPlan::pohyb_hrace(string prikaz, int velikost, Hrac *hrac, int cisloHrace, stack<string> *historie){
	int radek;
	int sloupec;
	hrac->vrat_pozici(&radek,&sloupec);
	//krok dolu
	if(prikaz == "d"){
		if(radek == velikost-1) return 0;
		//kontrola, zda se muze hrac pohnout dolu z aktualniho policka
		if((mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 0) ||
			(mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 3) ||
			(mapa[radek][sloupec].druh == 1 && mapa[radek][sloupec].otoceni == 2) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 1) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 3)) return 0; 
		//kontrola, zda se muze hrac posunout na nove policko
		if((mapa[radek+1][sloupec].druh == 0 && mapa[radek+1][sloupec].otoceni == 1) ||
			(mapa[radek+1][sloupec].druh == 0 && mapa[radek+1][sloupec].otoceni == 2) ||
			(mapa[radek+1][sloupec].druh == 1 && mapa[radek+1][sloupec].otoceni == 0) ||
			(mapa[radek+1][sloupec].druh == 2 && mapa[radek+1][sloupec].otoceni == 1) ||
			(mapa[radek+1][sloupec].druh == 2 && mapa[radek+1][sloupec].otoceni == 3)) return 0; 
		hrac->pohyb(prikaz[0], velikost, false);
		radek++;
		historie->push("n");
	}
	//krok nahoru
	else if(prikaz == "n"){
		if(radek == 0) return 0;
		//kontrola, zda se muze hrac pohnout nahoru z aktualniho policka
		if((mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 1) ||
			(mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 2) ||
			(mapa[radek][sloupec].druh == 1 && mapa[radek][sloupec].otoceni == 0) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 1) ||
            (mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 3)) return 0;
		//kontrola, zda se muze hrac posunout na nove policko
		if((mapa[radek-1][sloupec].druh == 0 && mapa[radek-1][sloupec].otoceni == 0) ||
			(mapa[radek-1][sloupec].druh == 0 && mapa[radek-1][sloupec].otoceni == 3) ||
			(mapa[radek-1][sloupec].druh == 1 && mapa[radek-1][sloupec].otoceni == 2) ||
			(mapa[radek-1][sloupec].druh == 2 && mapa[radek-1][sloupec].otoceni == 1) ||
			(mapa[radek-1][sloupec].druh == 2 && mapa[radek-1][sloupec].otoceni == 3)) return 0; 
		hrac->pohyb(prikaz[0], velikost, false);
		radek--;
		historie->push("d");
	}
	//krok vpravo
	else if(prikaz == "p"){
		if(sloupec == velikost-1) return 0;
		//kontrola, zda se muze hrac doprava dolu z aktualniho policka
		if((mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 2) ||
			(mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 3) ||
			(mapa[radek][sloupec].druh == 1 && mapa[radek][sloupec].otoceni == 1) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 0) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 2)) return 0; 
		//kontrola, zda se muze hrac posunout na nove policko
		if((mapa[radek][sloupec+1].druh == 0 && mapa[radek][sloupec+1].otoceni == 0) ||
			(mapa[radek][sloupec+1].druh == 0 && mapa[radek][sloupec+1].otoceni == 1) ||
			(mapa[radek][sloupec+1].druh == 1 && mapa[radek][sloupec+1].otoceni == 3) ||
			(mapa[radek][sloupec+1].druh == 2 && mapa[radek][sloupec+1].otoceni == 0) ||
			(mapa[radek][sloupec+1].druh == 2 && mapa[radek][sloupec+1].otoceni == 2)) return 0; 
		hrac->pohyb(prikaz[0], velikost, false);
		sloupec++;
		historie->push("l");
	}
	//krok vlevo
	else if(prikaz == "l"){
		if(sloupec == 0) return 0;
		//kontrola, zda se muze hrac pohnout doleva z aktualniho policka
		if((mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 0) ||
			(mapa[radek][sloupec].druh == 0 && mapa[radek][sloupec].otoceni == 1) ||
			(mapa[radek][sloupec].druh == 1 && mapa[radek][sloupec].otoceni == 3) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 0) ||
			(mapa[radek][sloupec].druh == 2 && mapa[radek][sloupec].otoceni == 2)) return 0; 
		//kontrola, zda se muze hrac posunout na nove policko
        if((mapa[radek][sloupec-1].druh == 0 && mapa[radek][sloupec-1].otoceni == 2) ||
			(mapa[radek][sloupec-1].druh == 0 && mapa[radek][sloupec-1].otoceni == 3) ||
			(mapa[radek][sloupec-1].druh == 1 && mapa[radek][sloupec-1].otoceni == 1) ||
			(mapa[radek][sloupec-1].druh == 2 && mapa[radek][sloupec-1].otoceni == 0) ||
			(mapa[radek][sloupec-1].druh == 2 && mapa[radek][sloupec-1].otoceni == 2)) return 0; 
        hrac->pohyb(prikaz[0], velikost, false);
		sloupec--;
		historie->push("p");
	}
	if(mapa[radek][sloupec].predmet == hrac->hledany_predmet()){
		hrac->pridej_bod();
		//umisteni identifikatoru pohybu na vrsek
		string vrsek = historie->top();
		historie->pop();
		//ulozeni cisla predmetu na zasobnik
		char intStr[2];
		sprintf(intStr, "%i", mapa[radek][sloupec].predmet);
		historie->push(string(intStr));
		mapa[radek][sloupec].predmet = NIC;
        if(hrac->pocet_bodu() == pocetBodu){
			hrac->nastav_konec();
		}
		else prirad_predmet(hrac);
        historie->push("bod");
        //qDebug() << "Pushing vrsek: " << QString::fromStdString(vrsek);
        if(gui == false){
            historie->push(vrsek);
        }
	}
	else if(hrac->hledany_predmet() == KONEC){
		if(cisloHrace == 0 && radek == 0 && sloupec == 0) return 1;
		else if(cisloHrace == 1 && radek == velikost-1 && sloupec == velikost -1) return 2;
		else if(cisloHrace == 2 && radek == 0 && sloupec == velikost-1) return 3;
		else if(cisloHrace == 3 && radek == velikost-1 && sloupec == 0) return 4;
		
	}

    return -1;
}
//Vykresleni jednoho policka
void HerniPlan::vypis_policko(Policko mapa, int cast, int postava, int zacatek){
	//políčko tvaru L
	if(mapa.druh == 0){
		if(mapa.otoceni == 0){
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2588\u2588";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
		}
		else if(mapa.otoceni == 1){
			if(cast == 0) cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2588\u2588";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
		else if(mapa.otoceni == 2){
			if(cast == 0) cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2588\u2588\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
		else {
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2588\u2588\t";
			}
			else cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
		}
	}
	//políčko tvaru T
	else if(mapa.druh == 1){
		if(mapa.otoceni == 0){
			if(cast == 0) cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
		else if(mapa.otoceni == 1){
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2588\u2588\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
		else if(mapa.otoceni == 2){
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
		}
		else{
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2588\u2588";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
	}
	//políčko tvaru I
	else if(mapa.druh == 2){
		if(mapa.otoceni == 0 || mapa.otoceni == 2){
			if(cast == 0) cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2588\u2588";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2588\u2588\t";
			}
			else cout << "\u2588\u2588\u2591\u2591\u2588\u2588\t";
		}
		else{
			if(cast == 0) cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
			else if(cast == 1) {
				cout << "\u2591\u2591";
				vykresli_stred(postava, mapa.predmet, zacatek);
				cout << "\u2591\u2591\t";
			}
			else cout << "\u2588\u2588\u2588\u2588\u2588\u2588\t";
		}
	}
}
//Vytvoreni popisku herniho planu
void HerniPlan::popisek(int radek, int cast, bool zacatek){
	if(radek == 0 && cast == 0 && zacatek == true){
		if(velikost >= 5) cout << "\t\t" << "   A\t\t   B";
		if(velikost >= 7) cout << "\t\t   C";
		if(velikost >= 9) cout << "\t\t   D";
		if(velikost >= 11) cout << "\t\t   E"; 
		cout << endl;
	}
	if(cast == 1){
		switch (radek){
			case 1:
				if(zacatek == true) cout << "   Y\t";
				else cout << "   I\t";
				break;
			case 3:
				if(zacatek == true) cout << "   X\t";
				else cout << "   J\t";
				break;
			case 5:
				if(zacatek == true) cout << "   W\t";
				else cout << "   K\t";
				break;
			case 7:
				if(zacatek == true) cout << "   V\t";
				else cout << "   L\t";
				break;
			case 9:
				if(zacatek == true) cout << "   U\t";
				else cout << "   M\t";
				break;
			default:
				cout << "\t";
		}
	}
	else cout << "\t";

}
// Vypsani (vykresleni) herni mapy
void HerniPlan::vypis(Hrac hrac[], int pocetHracu){
	for(int i = 0; i < velikost; i++){
		//rozdeleni jednoho policka na tri kvuli lepsi graficke podobe
		for(int cast = 0; cast < 3; cast++){
			popisek(i, cast, true);
			for(int j = 0; j < velikost; j++){
				int postava = -1;
				//hledani hracu na policku
				for(int h = 0; h < pocetHracu; h++){
					if(hrac[h].je_postava(i,j)){
						if(postava >= 0) postava = 4;
						else postava = h;
					}
				}
				int zacatek = NIC;
				if(i == 0 && j == 0) zacatek = 0;
				else if(i == velikost-1 && j == velikost -1) zacatek = 1;
				else if(i == 0 && j == velikost-1) zacatek = 2;
				else if(i == velikost-1 && j == 0) zacatek = 3;
				vypis_policko(mapa[i][j], cast, postava, zacatek);
			}
			popisek(i, cast, false);
			//vypise herni stav
			if(i == 0 && cast == 0){
				cout << "\x1B[31mHrac 1:\x1B[0m " << hrac[0].pocet_bodu() << " bodu";
				if(pocetHracu > 2)
					cout << "\t" << "\x1B[36mHrac 3:\x1B[0m " << hrac[2].pocet_bodu() << " bodu";
			}
			if(i == 0 && cast == 1){
				cout << "\x1B[32mHrac 2:\x1B[0m " << hrac[1].pocet_bodu() << " bodu";
				if(pocetHracu > 3)
					cout << "\t" << "\x1B[33mHrac 4:\x1B[0m " << hrac[3].pocet_bodu() << " bodu";
			}
			//vypise volne policko
			if(i == 1){
				cout << "\t";
				vypis_policko(volne, cast, NIC, NIC);
			}
			cout << endl;
		}
		cout << endl;
		if(i + 1 == velikost){
			if(velikost >= 5) cout << "\t\t" << "   S\t\t   R";
			if(velikost >= 7) cout << "\t\t   Q";
			if(velikost >= 9) cout << "\t\t   P";
			if(velikost >= 11) cout << "\t\t   O"; 
			cout << endl;
		}
	}
}
//Vycisti terminalovou obrazovku
int HerniPlan::cista_obrazovka(){
	return write(STDOUT_FILENO,"\e[1;1H\e[2J\n",12);
}
//Vlozi volne policko na souradnici a ulozi vysunute
void HerniPlan::vloz(int x, int y, Hrac hrac[], int pocetHracu){
	Policko vysunute;
	//vlozeni volne policko podle souradnic
	if(x == 0){
		vysunute = mapa[velikost-1][y];
		for(int i = velikost-1; i > 0; i--){
			mapa[i][y] = mapa[i-1][y];
		}
		mapa[0][y] = volne; 
		volne = vysunute;
	}
	else if(y == 0){
		vysunute = mapa[x][velikost-1];
		for(int i = velikost-1; i > 0; i--){
			mapa[x][i] = mapa[x][i-1];
		}
		mapa[x][0] = volne; 
		volne = vysunute;
	}
	else if(x == velikost-1){
		vysunute = mapa[0][y];
		for(int i = 0; i < velikost; i++){
			mapa[i][y] = mapa[i+1][y];
		}
		mapa[velikost-1][y] = volne; 
		volne = vysunute;
	}
	else if(y == velikost-1){
		vysunute = mapa[x][0];
		for(int i = 0; i < velikost-1; i++){
			mapa[x][i] = mapa[x][i+1];
		}
		mapa[x][velikost-1] = volne; 
		volne = vysunute;
	}
	int radek;
	int sloupec;
	//posun hraci na posunutych polich
	for(int i = 0; i < pocetHracu; i++){
		hrac[i].vrat_pozici(&radek,&sloupec);
		if(x == 0 && sloupec == y){
			hrac[i].pohyb('d', velikost, true);
		}
		else if(y == 0 && radek == x){
			hrac[i].pohyb('p', velikost, true);
		}
		else if(x == velikost-1 && sloupec == y){
			hrac[i].pohyb('n', velikost, true);
		}
		else if(y == velikost-1 && radek == x){
			hrac[i].pohyb('l', velikost, true);
		}
	}
}
//Posunuti rady
bool HerniPlan::posun(string symbol, Hrac hrac[], int pocetHracu){
	if(symbol == "A" && vlozenePolicko != "S") vloz(0,1,hrac,pocetHracu);
	else if(symbol == "B" && vlozenePolicko != "R") vloz(0,3,hrac,pocetHracu);
	else if(velikost >= 7 && symbol == "C" && vlozenePolicko != "Q") vloz(0,5,hrac,pocetHracu);
	else if(velikost >= 9 && symbol == "D" && vlozenePolicko != "P") vloz(0,7,hrac,pocetHracu);
	else if(velikost >= 11 && symbol == "E" && vlozenePolicko != "O") vloz(0,9,hrac,pocetHracu);
	
	else if(symbol == "I" && vlozenePolicko != "Y") vloz(1,velikost-1,hrac,pocetHracu);
	else if(symbol == "J" && vlozenePolicko != "X") vloz(3,velikost-1,hrac,pocetHracu);
	else if(velikost >= 7 && symbol == "K" && vlozenePolicko != "W") vloz(5,velikost-1,hrac,pocetHracu);
	else if(velikost >= 9 && symbol == "L" && vlozenePolicko != "V") vloz(7,velikost-1,hrac,pocetHracu);
	else if(velikost >= 11 && symbol == "M" && vlozenePolicko != "U") vloz(9,velikost-1,hrac,pocetHracu);
	
	else if(velikost >= 11 && symbol == "O" && vlozenePolicko != "E") vloz(velikost-1,9,hrac,pocetHracu);
	else if(velikost >= 9 && symbol == "P" && vlozenePolicko != "D") vloz(velikost-1,7,hrac,pocetHracu);
	else if(velikost >= 7 && symbol == "Q" && vlozenePolicko != "C") vloz(velikost-1,5,hrac,pocetHracu);
	else if(symbol == "R" && vlozenePolicko != "B") vloz(velikost-1,3,hrac,pocetHracu);
	else if(symbol == "S" && vlozenePolicko != "A") vloz(velikost-1,1,hrac,pocetHracu);
	
	else if(velikost >= 11 && symbol == "U" && vlozenePolicko != "M") vloz(9,0,hrac,pocetHracu);
	else if(velikost >= 9 && symbol == "V" && vlozenePolicko != "L") vloz(7,0,hrac,pocetHracu);
	else if(velikost >= 7 && symbol == "W" && vlozenePolicko != "K") vloz(5,0,hrac,pocetHracu);
	else if(symbol == "X" && vlozenePolicko != "J") vloz(3,0,hrac,pocetHracu);
	else if(symbol == "Y" && vlozenePolicko != "I") vloz(1,0,hrac,pocetHracu);
	else {
		return false;
	}
	vlozenePolicko = symbol;
	return true;
}
//Otoci s volnym polickem
void HerniPlan::otoc(){
	if(volne.otoceni == 3) volne.otoceni = 0;
	else volne.otoceni++; 
}
//Otoci s volnym polickem zpet
void HerniPlan::otoc_zpet(){
	if(volne.otoceni == 0) volne.otoceni = 3;
	else volne.otoceni--; 
}
//Posledni provedene vlozeni policka
string HerniPlan::vlozene_policko(){
	return vlozenePolicko;
}
//Nastavi vlozenePolicko po navraceni vlozeneho policka
void HerniPlan::nastav_vlozene_policko(string policko){
	vlozenePolicko = policko;
}
//Vrati hracem sebrany predmet zpet do hry a vrati hru do stavu pred sebranim tohoto predmetu
void HerniPlan::vrat_predmet_zpet(int x, int y, string predmet, Hrac *hrac){
	int iPredmet = atoi(predmet.c_str());
	volnePredmety[hrac->hledany_predmet()] = herni_predmety(hrac->hledany_predmet());
	mapa[x][y].predmet = iPredmet;
	volnePredmety[iPredmet] = iPredmet;
	hrac->nastav_predmet(iPredmet);
}
//Ulozeni herniho planu do souboru
void HerniPlan::uloz(ofstream *soubor){
	*soubor << velikost << endl << pocetPredmetu << endl << pocetBodu << endl << vlozenePolicko << endl;
	for(int i = 0; i < velikost; i++){
		for(int j = 0; j < velikost; j++){
			*soubor << mapa[i][j].druh << endl << mapa[i][j].otoceni << endl << mapa[i][j].predmet << endl;
		}
	}
	*soubor << volne.druh << endl << volne.otoceni << endl << volne.predmet << endl;
	//ulozeni predmetu
    for(int i = 0; i < pocetPredmetu; i++){
		*soubor << predmety[i] << endl << volnePredmety[i] << endl;
	}
}

//Vrati druh policka na souradnicich
int HerniPlan::vrat_druh(int x, int y){
    if(x < 0 && y < 0) return volne.druh;
    return mapa[x][y].druh;
}
//Vrati otoceni policka na souradnicich

int HerniPlan::vrat_otoceni(int x, int y){
    if(x < 0 && y < 0) return volne.otoceni;
    return mapa[x][y].otoceni;
}
//vrati predmet policka na souradnicich
int HerniPlan::vrat_predmet(int x, int y){
    if(x < 0 && y < 0) return volne.predmet;
    return mapa[x][y].predmet;
}
