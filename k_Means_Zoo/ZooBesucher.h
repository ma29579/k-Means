#ifndef ZOO_BESUCHER_H_
#define ZOO_BESUCHER_H_

#include <iostream>
#include <string>

class ZooBesucher
{

public:
	// Konstruktor für ZooBesucher - Objekte
	ZooBesucher(std::string besucherID, int alter, float bewertung) : besucherID_{ besucherID }, alter_{ alter }, bewertung_{ bewertung }{}

	// Methoden für den Abruf von Eigenschaften eines instanzierten Objektes
	std::string erhalteID() { return besucherID_; };
	int erhalteAlter() { return alter_; };
	float erhalteBewertung() { return bewertung_; };
	int erhalteCluster() { return cluster_; };

	// Methode für den Wechsel in ein bestimmtes Cluster
	void clusterwechsel(int cluster) { cluster_ = cluster; };

private:
	// Eigenschaften eines Zoo-Besuchers, die in der CSV-Datei übermittelt werden
	std::string besucherID_;
	int alter_;
	float bewertung_;

	/* Diese Objektvariable wird für das Clusterverfahren benötigt.
	Durch die direkte Zuweisung des Wertes -1, ist erkennbar, dass ein Datensatz noch keinem Cluster zugewiesen wurde. */
	int cluster_ = -1;

};

#endif // !ZOO_BESUCHER_H_