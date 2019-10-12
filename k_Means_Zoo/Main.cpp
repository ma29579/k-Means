#include "ZooBesucher.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <iomanip>


// Durch diese Funktion werden die zu analysierenden Datensätze der Zoo-Besucher eingelesen und in dem übergebenen Vektor für Zoo-Besucher abgelegt
void besucherEinlesen(std::vector<ZooBesucher*>& besucher, std::string csvPfad) 
{
	std::ifstream csvDatei(csvPfad);
	std::string csvInhalt;

	// Überprüfung ob die angegebene Datei erfolgreich gefunden und geöffnet werden kann.
	if (!csvDatei.good())
		return;

	// Die Titelzeile der CSV-Spalten wird eingelesen, aber nicht verwertet.
	std::getline(csvDatei, csvInhalt);

	/* Die geöffnete CSV-Datei wird zeilenweise eingelesen, solange das Datei-Ende noch nicht erreicht wurde.
	Dabei wird die eingelesene Zeile, anhand des verwendeten Trennzeichens in mehrere Bestandteile aufgeteilt,
	diese werden unteranderem mit der Hilfe von Typ-Konvertierungsfunktionen (std::stoi() & std::stof) dem Konstruktor der Zoo-Besucher-Klasse übergeben,
	um einen Zeiger auf ein Zoo-Besucher-Objekt zu erzeugen. Der besagte Zeiger wird dem übergebenen Vektor für Zeiger auf Zoo-Besucher hinzugefügt. */
	while (std::getline(csvDatei, csvInhalt)) 
	{
		std::stringstream st(csvInhalt);
		std::string tmpString;
		std::vector<std::string> zeilenBestandteile;

		while (std::getline(st, tmpString, ';'))
			zeilenBestandteile.push_back(tmpString);

		ZooBesucher* z = new ZooBesucher(zeilenBestandteile[0], std::stoi(zeilenBestandteile[1]), std::stof(zeilenBestandteile[2]));
		besucher.push_back(z);
	}
}


/* Diese Funktion führt die Clusterbildung, mit den zuvor eingelesenen Daten durch, dafür benötigt sie die verwendete Datenstruktur,
in welcher sich die eingelesenen Daten befinden und eine Angabe über die gewünschte Clusteranzahl. */
void erstelleCluster(std::vector<ZooBesucher*>& besucher, int clusterAnzahl) 
{
	// Speicherquelle für die benötigte Menge an Zentroiden (Merkmals-Reihenfolge : Alter, Bewertung).
	std::vector<std::pair<float, float>> zentroide;

	// Die Variable zentroidenTest wird im späteren Verlauf für das Beenden des Clusterverfahrens genutzt.
	bool zentroidenTest;

	std::vector<int> zufallszahlen;

	// Die aktuelle Zeit wird verwendet, um zu gewährleisten, dass die erstellten Pseudozufallszahlen, nicht den selben Seed benutzen.
	srand(time(0));

	// Durch Zufallszahlen werden Datensätze bestimmt, die als erste Zentroide benutzt werden.
	for (int i = 0; i < clusterAnzahl; i++) 
	{
		int zufallszahl = rand() % besucher.size() - 1;
		bool doppelteZufallszahl = false;

		// Überprüfung ob, eine Zufallszahl bereits für die Zentroid-Auswahl verwendet wurde.
		for (int j = 0; j < zufallszahlen.size(); j++) 
		{
			if (zufallszahlen[j] == zufallszahl) 
				doppelteZufallszahl = true;
		}

		/* Wurde eine doppelte Zufallszahl generiert, wird der Schleifendurchlauf wiederholt, 
		ansonsten wird der Datensatz als Zentroid genutzt und die Zufallszahl wird für eine spätere Überprüfung gespeichert. */
		if (doppelteZufallszahl) 
		{
			i--;
			continue;
		}
		else 
		{
			zentroide.push_back(std::pair<float, float>(besucher[zufallszahl]->erhalteAlter(), besucher[zufallszahl]->erhalteBewertung()));
			zufallszahlen.push_back(zufallszahl);
		}
	}

	do 
	{
		// Datenstruktur für die jeweiligen berechneten Distanzen zu den verschiedenen Zentroiden.
		std::vector<float> euklidischeDistanzen;

		// Datenstruktur für die berechneten Zentroiden
		std::vector <std::pair<float, float>> berechneteZentroiden;

		// Datenstruktur, welche die Datensatzanzahl pro Cluster speichert.
		std::vector<int> objektanzahlProZentroid;

		zentroidenTest = true;

		// Variablen für einen späteren Vergleich der euklidischen Distanzen.
		int zentroidenIndex;
	    float kleinsteEuklidischeDistanz;

		for (int i = 0; i < besucher.size(); i++) 
		{
			// Berechnung der jeweiligen eukldisischen Distanzen zu jedem Zentroid.
			for (int j = 0; j < clusterAnzahl; j++) 
			{
				float euklidischeDistanz = std::sqrt(std::pow(zentroide[j].first - besucher[i]->erhalteAlter(), 2) +
										   std::pow(zentroide[j].second - besucher[i]->erhalteBewertung(), 2));
				
				euklidischeDistanzen.push_back(euklidischeDistanz);
			}

			// Ermittlung des kleinsten Abstands zu einem Zentroid.
			zentroidenIndex = 0;
			kleinsteEuklidischeDistanz = euklidischeDistanzen[0];

			for (int j = 1; j < clusterAnzahl; j++) 
			{
				if (euklidischeDistanzen[j] < kleinsteEuklidischeDistanz) 
				{
					zentroidenIndex = j;
					kleinsteEuklidischeDistanz = euklidischeDistanzen[j];
				}
			}

			// Der Datensatz wird dem Cluster zugewiesen, dessen Zentroid den geringsten Abstand zu dem jeweiligen Datensatz besitzt.
			besucher[i]->clusterwechsel(zentroidenIndex);

			// Der Inhalt des Vektors, für die euklidischen Distanzen wird entfernt.
			euklidischeDistanzen.clear();

		}

		/* Der Vektor für die zukünftigen Zentroide und der Vektor für die Anzahl der Datensätze pro Cluster,
		werden mit der entsprechenden Anzahl an Elementen befüllt. */

		for (int i = 0; i < clusterAnzahl; i++) 
		{
			berechneteZentroiden.push_back(std::pair<float, float>(0, 0));
			objektanzahlProZentroid.push_back(0);
		}


		// Berechnungen für die zukünftigen Zentroiden werden durchgeführt

		/* Für jedes Cluster werden die relevanten Merkmale (Alter, Bewertung) aufsummiert,
		außerdem wird die Anzahl der Datensätze pro Cluster, für die spätere Mittelwertberechnung bestimmt. */
		for (int i = 0; i < besucher.size(); i++) 
		{
			int besucherCluster = besucher[i]->erhalteCluster();

			berechneteZentroiden[besucherCluster].first += besucher[i]->erhalteAlter();
			berechneteZentroiden[besucherCluster].second += besucher[i]->erhalteBewertung();

			objektanzahlProZentroid[besucherCluster] += 1;
		}

		// Die Mittelwerte der Clusterelemente, werden für die zukünftigen Zentroide bestimmt.
		for (int i = 0; i < clusterAnzahl; i++) 
		{
			if (berechneteZentroiden[i].first > 0) 
			{
				berechneteZentroiden[i].first /= objektanzahlProZentroid[i];
				berechneteZentroiden[i].second /= objektanzahlProZentroid[i];
			}
		}

		// Überprüfung, ob es die berechneten neuen Zentroide, den bereits verwendeten Zentroiden im vorherigen Iterations-Durchlauf entsprechen.
		for (int i = 0; i < clusterAnzahl; i++) 
		{
			if (berechneteZentroiden[i] != zentroide[i])
				zentroidenTest = false;
		}

		/* Entsprechen sich die Zentroide nicht, folgt ein weiterer Iterations-Durchlauf mit den zuvor ermittelten Zentroiden,
		andernfalls wird die Clusterbildung beendet. */
		if (!zentroidenTest) 
		{
			for (int i = 0; i < clusterAnzahl; i++) 
			{
				zentroide[i].first = berechneteZentroiden[i].first;
				zentroide[i].second = berechneteZentroiden[i].second;
			}
		}

	} while (!zentroidenTest);
}

/* Die Funktion gibt die ermittelten Cluster, mit den Informationen über die Zoo-Besucher in einer CSV-Datei aus, 
dafür müssen die Datenstruktur, in welcher die Zoo-Besucher gespeichert wurden und eine Zeichenkette, für den Namen der CSV-Datei übergeben werden. */
void erstelleCSVDatei(std::vector<ZooBesucher*>& besucher, std::string dateiname) 
{
	std::ofstream datei;
	datei.open(dateiname);

	datei << "PersonenID;Alter;Bewertung;Cluster" << std::endl;

	for (int i = 0; i < besucher.size(); i++)
	{
		datei << besucher[i]->erhalteID() << ";" << besucher[i]->erhalteAlter() << ";" << std::setprecision(2) << std::fixed << besucher[i]->erhalteBewertung() << ";" << besucher[i]->erhalteCluster() << std::endl;
	}

	datei.close();
}


int main(int argc, char** argv) 
{

	std::vector<ZooBesucher*> besucher;
	besucherEinlesen(besucher, "Zoo.csv");
	erstelleCluster(besucher, 3);
	erstelleCSVDatei(besucher, "Cluster_Ergebnis.csv");

	return EXIT_SUCCESS;
}

