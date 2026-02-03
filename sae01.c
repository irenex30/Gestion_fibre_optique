#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996)

enum { nb_char_max_nom = 31, nb_char_max_role = 3, nbmax_entreprise = 50, nb_char = 100, nbmax_mission = 500 };
enum { nbmax_acceptation = 10000, nbmax_sous_traitance = 5, nb_char_max_signification = 40, nbmax_code = 4, nb = 100 };

typedef struct {
	unsigned char identifiant;
	char nom[nb_char_max_nom];
	char role[nb_char_max_role];
} ENTREPRISE;

typedef struct {
	unsigned int identifiant;
	unsigned int id_ent;
	char nom[nb_char_max_nom];
	double remuneration;
	char attribuee;
	char terminee;
	unsigned int ens_accept[nb];
	unsigned int ens_ent_echec[nb];
	unsigned int compteur_echec;
	unsigned int nb_sous_trait;
	unsigned int nb_signification;
	unsigned int code_signification[nbmax_code];
} MISSION;

typedef struct {
	unsigned int cd_retour;
	char signification[nb_char_max_signification];
	double majoration;
} CODERETOUR;

void inscription(ENTREPRISE* entreprise, unsigned int* compte_entreprise) {

	//inscription de l'entreprise, son id, son nom et son rôle, dans l'ensemble des entreprises regroupé dans un tableau

	char nom_ent[nb_char_max_nom];
	char role[nb_char_max_role];
	scanf("%s%s", role, nom_ent);
	if (strcmp(role, "OP") != 0 && strcmp(role, "AG") != 0 && strcmp(role, "IN") != 0) {
		printf("Role incorrect\n");
		return;
	}
	for (unsigned int i = 0; i < *compte_entreprise; ++i) {
		if (strcmp(entreprise[i].nom, nom_ent) == 0) {
			printf("Nom incorrect\n");
			return;
		}
	}
	*compte_entreprise += 1;
	entreprise[*compte_entreprise - 1].identifiant = *compte_entreprise;
	strcpy(entreprise[*compte_entreprise - 1].nom, nom_ent);
	strcpy(entreprise[*compte_entreprise - 1].role, role);
	printf("Inscription realisee (%u)\n", *compte_entreprise);
}

void mission(MISSION* mis, const ENTREPRISE* entreprise, unsigned int* compte_mission, unsigned int* compte_entreprise) {

	//enregistrer une mission, par son id, nom, rémunération, donnée par une entreprise, dans l'ensemble des missions regroupées dans un tableau

	unsigned int id;
	char nom_mission[nb_char_max_nom];
	float remu;
	scanf("%u%s%f", &id, nom_mission, &remu);
	if (remu < 0.) {
		printf("Remuneration incorrecte\n");
		return;
	}
	for (unsigned int i = 0; i < *compte_entreprise; ++i) {
		if (id == entreprise[i].identifiant && (strcmp(entreprise[i].role, "OP") == 0)) {
			mis[*compte_mission].identifiant = *compte_mission + 1;
			mis[*compte_mission].id_ent = id;
			strcpy(mis[*compte_mission].nom, nom_mission);
			mis[*compte_mission].remuneration = remu;
			mis[*compte_mission].attribuee = 0;
			mis[*compte_mission].terminee = 0;
			mis[*compte_mission].nb_sous_trait = 0;
			mis[*compte_mission].nb_signification = 0;
			mis[*compte_mission].compteur_echec = 0;
			*compte_mission += 1;
			printf("Mission publiee (%u)\n", *compte_mission);
			return;
		}
	}
	printf("Identifiant incorrect\n");
}

void consultation(const MISSION* mis, const ENTREPRISE* entreprise, unsigned int* compte_mission) {
	unsigned char disponible = 1;
	for (unsigned int i = 0; i < *compte_mission; ++i) {
		if (mis[i].attribuee == 0) {
			disponible = 0;
			printf("%u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
		}
	}
	if (disponible == 1)
		printf("Aucune mission disponible\n");
}

void acceptation(MISSION* mis, const ENTREPRISE* entreprise, unsigned int* compte_entreprise, unsigned int* compte_mission) {
	unsigned int id_ent, id_mis;
	char connue = 1, correct = 1;
	scanf("%u%u", &id_ent, &id_mis);
	for (unsigned int j = 0; j < *compte_mission; ++j) {
		if (id_mis == mis[j].identifiant) {
			if (mis[j].attribuee == 1) {
				printf("Mission incorrecte\n");
				return;
			}
			else if (mis[j].compteur_echec > 0) {
				for (unsigned int k = 0; k < mis[j].compteur_echec; ++k) {
					if (mis[j].ens_ent_echec[k] == id_ent) {
						printf("Entreprise incorrecte\n");
						return;
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < *compte_entreprise; ++i) {
		if (id_ent == entreprise[i].identifiant && (strcmp(entreprise[i].role, "OP") != 0)) {
			correct = 0;
			for (unsigned int j = 0; j < *compte_mission; ++j) {
				if (id_mis == mis[j].identifiant) {
					connue = 0;
					mis[id_mis - 1].attribuee = 1;
					mis[id_mis - 1].ens_accept[mis[id_mis - 1].attribuee - 1] = id_ent;
					printf("Acceptation enregistree\n");
					return;
				}
			}
		}
	}
	if (correct == 1) {
		printf("Entreprise incorrecte\n");
	}
	if (correct == 0 && connue == 1) {
		printf("Mission incorrecte\n");
	}
}

void detail(const MISSION* mis, const ENTREPRISE* entreprise, const CODERETOUR* tab_code) {
	unsigned int id_mis = 0;
	scanf("%u", &id_mis);
	if (mis[id_mis - 1].attribuee == 1 || mis[id_mis - 1].identifiant != id_mis) {
		printf("Identifiant incorrect\n");
		return;
	}
	printf("%u %s %s %.2f (%u)\n", mis[id_mis - 1].identifiant, mis[id_mis - 1].nom, entreprise[mis[id_mis - 1].id_ent - 1].nom, mis[id_mis - 1].remuneration, mis[id_mis - 1].nb_sous_trait);
	for (int i = mis[id_mis - 1].nb_signification - 1; i > -1; --i) {
		printf("%s\n", tab_code[mis[id_mis - 1].code_signification[i]].signification);
	}
}

void sous_traitance(const ENTREPRISE* entreprise, MISSION* mis, unsigned int* compte_entreprise, unsigned int* compte_mission) {
	unsigned int id_ent, id_mis;
	float remu;
	scanf("%u%u%f", &id_ent, &id_mis, &remu);
	char connue = 1, correct = 1;
	if (remu <= 0.0) {
		printf("Remuneration incorrecte\n");
		return;
	}
	for (unsigned int i = 0; i < *compte_entreprise; ++i) {
		if (id_ent == entreprise[i].identifiant && (strcmp(entreprise[i].role, "AG") == 0)) {
			correct = 0;
			for (unsigned int j = 0; j < *compte_mission; ++j) {
				if (id_mis == mis[j].identifiant && mis[j].attribuee == 0 && mis[j].nb_sous_trait < nbmax_sous_traitance) {
					connue = 0;
					mis[j].attribuee = 1;
					mis[*compte_mission].id_ent = id_ent;
					mis[*compte_mission].identifiant = *compte_mission + 1;
					mis[*compte_mission].remuneration = remu;
					strcpy(mis[*compte_mission].nom, mis[j].nom);
					mis[*compte_mission].attribuee = 0;
					mis[*compte_mission].terminee = 0;
					mis[*compte_mission].compteur_echec = 0;
					mis[*compte_mission].nb_sous_trait = mis[j].nb_sous_trait + 1;
					mis[*compte_mission].nb_signification = mis[j].nb_signification;
					for (unsigned int k = 0; k < mis[j].nb_signification; ++k) {
						mis[*compte_mission].code_signification[k] = mis[j].code_signification[k];
					}
					*compte_mission += 1;
					printf("Sous-traitance enregistree (%u)\n", *compte_mission);
					return;
				}
			}
		}
	}
	if (correct == 1) {
		printf("Entreprise incorrecte\n");
	}
	if (correct == 0 && connue == 1) {
		printf("Mission incorrecte\n");
	}
}

void rapport(MISSION* mis, const CODERETOUR* tab_code, unsigned int* compte_mission) {
	unsigned int id_mis = 0;
	unsigned int code_retour = 0;
	char correct = 1, connue = 1;
	scanf("%u%u", &id_mis, &code_retour);
	for (unsigned int i = 0; i < *compte_mission; ++i) {
		if (mis[i].identifiant == id_mis)
			connue = 0;
	}
	if (connue == 1 || mis[id_mis - 1].attribuee == 0) {
		printf("Mission incorrecte\n");
		return;
	}
	for (unsigned int i = 0; i < nbmax_code; ++i) {
		if (code_retour == tab_code[i].cd_retour) {
			correct = 0;
		}
	}
	if (correct == 1) {
		printf("Code de retour incorrect\n");
		return;
	}
	if (code_retour == 0) {
		printf("Rapport enregistre\n");
		mis[id_mis - 1].terminee = 1;
		for (unsigned int i = 0; i < *compte_mission; ++i) {
			if (strcmp(mis[i].nom, mis[id_mis - 1].nom) == 0) {
				mis[i].terminee = 1;
			}
		}
		return;
	}
	else {
		mis[*compte_mission].id_ent = mis[id_mis - 1].id_ent;
		mis[*compte_mission].identifiant = *compte_mission + 1;
		mis[*compte_mission].remuneration = mis[id_mis - 1].remuneration * tab_code[code_retour].majoration;
		strcpy(mis[*compte_mission].nom, mis[id_mis - 1].nom);
		mis[*compte_mission].attribuee = 0;
		mis[*compte_mission].terminee = 0;
		mis[id_mis - 1].terminee = 1;
		for (unsigned int i = 0; i < mis[id_mis - 1].compteur_echec; ++i) {
			mis[*compte_mission].ens_ent_echec[i] = mis[id_mis - 1].ens_ent_echec[i];
		}
		mis[id_mis - 1].compteur_echec += 1;
		mis[*compte_mission].compteur_echec = mis[id_mis - 1].compteur_echec;
		mis[id_mis - 1].ens_ent_echec[mis[id_mis - 1].compteur_echec - 1] = mis[id_mis - 1].ens_accept[mis[id_mis - 1].attribuee - 1];
		mis[*compte_mission].ens_ent_echec[mis[*compte_mission].compteur_echec - 1] = mis[id_mis - 1].ens_ent_echec[mis[id_mis - 1].compteur_echec - 1];
		mis[*compte_mission].nb_sous_trait = 0;
		mis[*compte_mission].nb_signification = mis[id_mis - 1].nb_signification + 1;
		if (mis[id_mis - 1].nb_signification != 0) {
			for (unsigned int i = 0; i < mis[id_mis - 1].nb_signification; ++i) {
				mis[*compte_mission].code_signification[i] = mis[id_mis - 1].code_signification[i];
			}
			mis[*compte_mission].code_signification[mis[*compte_mission].nb_signification - 1] = tab_code[code_retour].cd_retour;
		}
		else {
			mis[*compte_mission].code_signification[mis[*compte_mission].nb_signification - 1] = tab_code[code_retour].cd_retour;
		}
		*compte_mission += 1;
		printf("Rapport enregistre (%u)\n", *compte_mission);
	}
}

void recapitulatif(const ENTREPRISE* entreprise, const MISSION* mis, unsigned int* compte_entreprise, unsigned int* compte_mission) {
	unsigned int id_ent;
	scanf("%u", &id_ent);
	unsigned int compte_non_attribuees = 0, compte_attribuees = 0, compte_terminees = 0, compte_a_realiser = 0, compte_realisees = 0;
	unsigned int connue = 1;
	for (unsigned int j = 0; j < *compte_entreprise; ++j) {
		if (id_ent == entreprise[j].identifiant) {
			connue = 0;
			for (unsigned int i = 0; i < *compte_mission; ++i) {
				if (id_ent == mis[i].id_ent) {
					if (mis[i].attribuee == 0) {
						compte_non_attribuees += 1;
						break;
					}
				}
			}
			if (compte_non_attribuees == 1) {
				printf("* non attribuees\n");
				for (unsigned int i = 0; i < *compte_mission; ++i) {
					if (id_ent == mis[i].id_ent) {
						if (mis[i].attribuee == 0) {
							printf("  %u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
						}
					}
				}
			}
			for (unsigned int i = 0; i < *compte_mission; ++i) {
				if (id_ent == mis[i].id_ent) {
					if (mis[i].attribuee == 1 && mis[i].terminee == 0) {
						compte_attribuees += 1;
						break;
					}
				}
			}
			if (compte_attribuees == 1) {
				printf("* attribuees\n");
				for (unsigned int i = 0; i < *compte_mission; ++i) {
					if (id_ent == mis[i].id_ent) {
						if (mis[i].attribuee == 1 && mis[i].terminee == 0) {
							printf("  %u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
						}
					}
				}
			}
			for (unsigned int i = 0; i < *compte_mission; ++i) {
				if (id_ent == mis[i].id_ent) {
					if (mis[i].terminee == 1) {
						compte_terminees += 1;
						break;
					}
				}
			}
			if (compte_terminees == 1) {
				printf("* terminees\n");
				for (unsigned int i = 0; i < *compte_mission; ++i) {
					if (id_ent == mis[i].id_ent) {
						if (mis[i].terminee == 1) {
							printf("  %u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
						}
					}
				}
			}
			for (unsigned int i = 0; i < *compte_mission; ++i) {
				if (mis[i].attribuee == 1 && id_ent == mis[i].ens_accept[mis[i].attribuee - 1]) {
					if (mis[i].terminee == 0) {
						compte_a_realiser += 1;
						break;
					}
				}
			}
			if (compte_a_realiser == 1) {
				printf("* a realiser\n");
				for (unsigned int i = 0; i < *compte_mission; ++i) {
					if (mis[i].attribuee == 1 && id_ent == mis[i].ens_accept[mis[i].attribuee - 1]) {
						if (mis[i].terminee == 0) {
							printf("  %u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
						}
					}
				}
			}
			for (unsigned int i = 0; i < *compte_mission; ++i) {
				if (mis[i].attribuee == 1 && id_ent == mis[i].ens_accept[mis[i].attribuee - 1]) {
					if (mis[i].terminee == 1) {
						compte_realisees += 1;
						break;
					}
				}
			}
			if (compte_realisees == 1) {
				printf("* realisees\n");
				for (unsigned int i = 0; i < *compte_mission; ++i) {
					if (mis[i].attribuee == 1 && id_ent == mis[i].ens_accept[mis[i].attribuee - 1]) {
						if (mis[i].terminee == 1) {
							printf("  %u %s %s %.2f (%u)\n", mis[i].identifiant, mis[i].nom, entreprise[mis[i].id_ent - 1].nom, mis[i].remuneration, mis[i].nb_sous_trait);
						}
					}
				}
			}
		}
	}
	if (connue == 1)
		printf("Entreprise incorrecte\n");
}

int main() {
	char commande[nb_char];
	unsigned int compte_entreprise = 0;
	ENTREPRISE entreprise[nbmax_entreprise];
	unsigned int compte_mission = 0;
	MISSION mis[nbmax_mission];
	CODERETOUR tab_code[] = { {0, "Succes", 1.}, {1, "Local non accessible", 1.}, {2, "Pas de signal dans le boitier general", 1.055}, {3, "Recepteur defectueux", 1.04} };
	do {
		scanf("%s", commande);
		if (strcmp(commande, "inscription") == 0) {
			inscription(entreprise, &compte_entreprise);
		}
		else if (strcmp(commande, "mission") == 0) {
			mission(mis, entreprise, &compte_mission, &compte_entreprise);
		}
		else if (strcmp(commande, "consultation") == 0) {
			consultation(mis, entreprise, &compte_mission);
		}
		else if (strcmp(commande, "acceptation") == 0) {
			acceptation(mis, entreprise, &compte_entreprise, &compte_mission);
		}
		else if (strcmp(commande, "detail") == 0) {
			detail(mis, entreprise, tab_code);
		}
		else if (strcmp(commande, "sous-traitance") == 0) {
			sous_traitance(entreprise, mis, &compte_entreprise, &compte_mission);
		}
		else if (strcmp(commande, "rapport") == 0) {
			rapport(mis, tab_code, &compte_mission);
		}
		else if (strcmp(commande, "recapitulatif") == 0) {
			recapitulatif(entreprise, mis, &compte_entreprise, &compte_mission);
		}
	} while (strcmp(commande, "exit") != 0);
}