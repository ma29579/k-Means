#ifndef ZOO_BESUCHER_H_
#define ZOO_BESUCHER_H_

#include <iostream>
#include <string>

class ZooBesucher
{

public:
	// Konstruktor f�r ZooBesucher - Objekte
	ZooBesucher(std::string besucherID, int alter, float bewertung) : besucherID_{ besucherID }, alter_{ alter }, bewertung_{ bewertung }{}

	// Methoden f�r den Abruf von Eigenschaften eines instanzierten Objektes
	std::string erhalteID() { return besucherID_; };
	int erhalteAlter() { return alter_; };
	float erhalteBewertung() { return bewertung_; };
	int erhalteCluster() { return cluster_; };

	// Methode f�r den Wechsel in ein bestimmtes Cluster
	void clusterwechsel(int cluster) { cluster_ = cluster; };

private:
	// Eigenschaften eines Zoo-Besuchers, die in der CSV-Datei �bermittelt werden
	std::string besucherID_;
	int alter_;
	float bewertung_;

	/* Diese Objektvariable wird f�r das Clusterverfahren ben�tigt.
	Durch die direkte Zuweisung des Wertes -1, ist erkennbar, dass ein Datensatz noch keinem Cluster zugewiesen wurde. */
	int cluster_ = -1;

};

#endif // !ZOO_BESUCHER_H_