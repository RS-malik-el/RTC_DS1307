/*
	DATE : 28/04/2022
	AUTEUR : RACHEL SYSTEME

	Ce programme permet de lire la date et l'heure via le module
	RTC DS1307 puis de l'afficher dans le moniteur série.

	NB: Après téléversement du programme, supprimer la fonction
	"config_RTC()" puis téléverser de nouveau le programme.
*/

#include <RTClib.h>

// Création de l'objet RTC_DS1307
RTC_DS1307 rtc;

// Création de l'objet DateTime
DateTime Now;

String jour[7]{
	"Dimanche",
	"Lundi",
	"Mardi",
	"Mercredi",
	"Jeudi",
	"Vendredi",
	"Samedi"
};

// Prototypes des fonctions
void config_RTC(int decalage = 10);
void date(void);
void time_f24(void);
void time_f12(void);

void setup(){
	// Initialisation de la communication série
	Serial.begin(9600);
	// Initialisation de la communication au module RTC_DS1307
	rtc.begin();
/*	
	Après avoir téléversé le programme, supprimer cette fonction puis 
	téléverser le programme à nouveau.
	NB : si elle n'est pas supprimée, à chaque lancement du programme,
	la date et l'heure seront configurées à la date du téléversement du
	programme.

	Insérer le retard approximatif dans les paramètres de la fonction
	EX : config_RTC(20); // valeur maxi 60
*/
	config_RTC(); 
}

void loop(){
	date();
	time_f24();
	time_f12();
	Serial.println("");
	delay(1000);
}

void config_RTC(int decalage){
	// Si le décalage dépasse 60 secondes
	if (decalage > 60)
		decalage = 60;
/*
	Configuration de la date et l'heure du module RTC
	à la date et l'heure de la compilation du programme.

	Configuration manuelle 27 avril 2022 à 11:00:00
	rtc.adjust(DateTime(2022, 4, 27, 11, 0, 0));
*/
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	
	Now = rtc.now(); // Mise à jour

	int date 	= (int) Now.day();
	int mois 	= (int) Now.month();
	int  annee 	= (int) Now.year();

	int H = (int) Now.hour();
	int M = (int) Now.minute();
	int S = (int) Now.second();

	// Nombre de secondes total
	int S_Total = S + decalage;
	// Nombre de minutes total
	int M_Total =  M + (S_Total / 60);
	// Nombre d'heures total
	int H_Total =  H + (M_Total / 60);

	S = S_Total % 60;
	M = M_Total % 60;
	H = H_Total % 24;
/*
	Configuration de la date et l'heure du module RTC
	à la date et l'heure de la compilation du programme
	plus le décalage issu lors du transfert vers l'arduino

*/
	rtc.adjust(DateTime(annee, mois, date, H, M, S));
}

void date(void){
	Now = rtc.now();

	Serial.print(jour[Now.dayOfTheWeek()]);
	Serial.print(" ");
	Serial.print(Now.day());
	Serial.print("/");
	Serial.print(Now.month());
	Serial.print("/");
	Serial.print(Now.year());
	Serial.println("");
}

// format 24 heures
void time_f24(){
	Now = rtc.now();

	Serial.print(Now.hour());
	Serial.print(":");
	Serial.print(Now.minute());
	Serial.print(":");
	Serial.print(Now.second());
	Serial.println("");
}

// format 12 heures
void time_f12(){
	Now = rtc.now();

	Serial.print(Now.twelveHour());
	Serial.print(":");
	Serial.print(Now.minute());
	Serial.print(":");
	Serial.print(Now.second());

	if (Now.isPM())
		Serial.print(" PM");
	else
		Serial.print(" AM");
	
	Serial.println("");
}
