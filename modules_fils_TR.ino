// *** Module fils temps réel ***
//
// Version 0.2
// date :22/11/2017
// ref : réunion du 21/11/2017
// Objectif :
//  Utilisateur doit débrancher les files dans un ordre établie par en ordre de priorité :
//  -  le moment de débrancher quel fil en fonction de l’affichage de l’horloge sur le module maitre
//      o si trompé alors erreur
//  - la led en face de chaque file qui s’allume aléatoirement
//  - la fréquence affichée sur le module maitre
//  Une fois tous les fils débranchés dans l’ordre le module se désactive pour éviter qu’un faux contact communique une erreur
//  Lorsqu’il y a une erreur de débranchement l’ordre débranchement se poursuit comme si le file n’existait pas dans les successions.
//
// This code is in the public domain.
// Ce code fait partie du domaine public.


#include <Wire.h>

//définition de la broche 3 de la carte en tant que variable

const int iOrange_pin = 8; // pin D8
const int iJaune_pin = 7;
const int iVert_pin = 6;
const int iBleu_pin = 5;
const int iViolet_pin = 4;
const int iGris_pin = 3;
const int iBlanc_pin = 2; // pin D2

const int iLed_Orange_pin = A1; // pin A1
const int iLed_Jaune_pin = A2;
const int iLed_Vert_pin = A3; // pin A3
const int iLed_Bleu_pin = 11; // pin D11
const int iLed_Violet_pin = 12; // pin D12
const int iLed_Gris_pin = 9; // pin D9
const int iLed_Blanc_pin = 10; // pin D10

const int iLed_tab[7] = { A1, A2, A3, 11, 12, 9, 10 };

const int ianalogforrandom = 0; // pin A0

// variables will change:
int ietat_fils; // variable permettant de monitorer l'état de la filerie
int ietat_fils_precedent = 0;
int ietat_fils_init = 0;// état initial de la variable concernant l'état de la filerie
int iiterration = 0; // variable de boucle afin de scruter chaque bits de la variable etat_fils
bool boolerreur = 0; //
int iLedOnOff[7]; // définition du tableau de 0 ou de 1 permettant l'allumage de la matrice de led
int iordersolving[7];// définition de l'ordre de résolution
int itabrangement[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // tableau pour le rangement des bits
int ifrequence;
int timer;
int initilisation = 2;
int i;
int j;
int k;
int visu = 0; //debogage

//différents possibilité en fonction des fréquences.
int itab_1[7] = { 0, 1, 2, 3, 4, 5, 6 };
int itab_2[7] = { 6, 5, 4, 3, 2, 1, 0 };
int itab_3[7] = { 0, 2, 4, 6, 5, 3, 1 };
int itab_4[7] = { 5, 3, 1, 0, 2, 4, 6 };


void setup()
{
    //initialisation de la broche 2 à 10 comme étant une entrée
    // les entrées sont en pull up de façon à mettre le potentiel à 5V en cas de débranchement
    pinMode(iOrange_pin, INPUT_PULLUP);
    pinMode(iJaune_pin, INPUT_PULLUP);
    pinMode(iVert_pin, INPUT_PULLUP);
    pinMode(iBleu_pin, INPUT_PULLUP);
    pinMode(iViolet_pin, INPUT_PULLUP);
    pinMode(iGris_pin, INPUT_PULLUP);
    pinMode(iBlanc_pin, INPUT_PULLUP);

    //initialisation de la seul broche de led présent sur
    pinMode(iLed_Orange_pin, OUTPUT);
    pinMode(iLed_Jaune_pin, OUTPUT);
    pinMode(iLed_Vert_pin, OUTPUT);
    pinMode(iLed_Bleu_pin, OUTPUT);
    pinMode(iLed_Violet_pin, OUTPUT);
    pinMode(iLed_Gris_pin, OUTPUT);
    pinMode(iLed_Blanc_pin, OUTPUT);

    //définition de la pin pour la graine aléatoire
    randomSeed(analogRead(ianalogforrandom));

    //référence de communication tochange
    Wire.begin(2);                // to_change Joindre le bus i2c avec l'adresse #2
    // Wire.onRequest(requestEvent); // enregistrer l'événement (lorsqu'une demande arrive)
    Serial.begin(9600);  // démarré une communication série
}

void loop()
{
    // phase d'initialisation
    if (initilisation == 2) {
        Serial.println("je m'initialise");
        Serial.println("Quelle est la valeur de ma graine?");
        Serial.println(analogRead(ianalogforrandom));
        //  Phase d’initialisation
        //  • Réception de la valeur de fréquence
        Serial.println("échange avec le master : valeur de firmware/fréquence");// à remplacer par la fonction de communication
        ifrequence = 4;//<----------------------------------------------------------------------------------------------------------------------------------------------------------------- Fréquences
        Serial.println(ifrequence);
        //  • Réception du temps de résolution
        Serial.println("échange avec le master : valeur du timer au lancement de la bombe"); // à remplacer par la fonction de communication
        // timer=XX to_change

        //  • Etablissement de l’état des led présentes sur le module (aléatoire)
        for (i = 0; i <= 6; ++i) {
            if (random(0, 2) == 0) {
                iLedOnOff[i] = LOW;
            }
            else {
                iLedOnOff[i] = HIGH;
            }
        }
        /*//<débug>
        iLedOnOff[0] = HIGH;
        iLedOnOff[1] = LOW;
        iLedOnOff[2] = HIGH;
        iLedOnOff[3] = LOW;
        iLedOnOff[4] = HIGH;
        iLedOnOff[5] = LOW;
        iLedOnOff[6] = LOW;
        //<\débug>*/
        /*//<débug>
          Serial.println("Etats des leds");
          for (i = 0; i <= 6; ++i) {
          Serial.println(iLedOnOff[i]);
          }
          //<\débug>*/

        digitalWrite(iLed_Orange_pin, iLedOnOff[0]);
        digitalWrite(iLed_Jaune_pin, iLedOnOff[1]);
        digitalWrite(iLed_Vert_pin, iLedOnOff[2]);
        digitalWrite(iLed_Bleu_pin, iLedOnOff[3]);
        digitalWrite(iLed_Violet_pin, iLedOnOff[4]);
        digitalWrite(iLed_Gris_pin, iLedOnOff[5]);
        digitalWrite(iLed_Blanc_pin, iLedOnOff[6]);

        //<débug>
        Serial.println("ordre de résolution");
        for (i = 0; i <= 6; ++i) {
            Serial.println(iordersolving[i]);
        }
        //<\débug>

        //  • Mettre les bits de fil dans l’ordre de résolution en fonction des conditions (fréquence et led)
        if (ifrequence == 1) { // si 1er séléction de fréquence on charge dans le tableau la suite de tab_1 ={0,1,2,3,4,5,6}
            Serial.println("Fréquence 1");
            Serial.println("{0,1,2,3,4,5,6}");
            for (i = 0; i <= 6; ++i) {
                iordersolving[i] = itab_1[i];
            }
        }
        else if (ifrequence == 2) { // si 2ème séléction de fréquence on charge dans le tableau la suite de tab_2 ={6,5,4,3,2,1,0}
            Serial.println("Fréquence 2");
            Serial.println("{6,5,4,3,2,1,0}");
            for (i = 0; i <= 6; ++i) {
                iordersolving[i] = itab_2[i];
            }
        }
        else if (ifrequence == 3) { // si 3ème séléction de fréquence on charge dans le tableau la suite de tab_3 ={0,2,4,6,5,3,1}
            Serial.println("Fréquence 3");
            Serial.println("{0,2,4,6,5,3,1}");
            for (i = 0; i <= 6; ++i) {
                iordersolving[i] = itab_3[i];

            }
        }
        else { // sinon pour la 4ème séléction de fréquence on charge dans le tableau la suite de tab_4 ={3,2,4,1,5,0,6}
            Serial.println("Fréquence 4");
            Serial.println("{3,2,4,1,5,0,6}");
            for (i = 0; i <= 6; ++i) {
                iordersolving[i] = itab_4[i];
            }
        }

        //<débug>
        Serial.println("ordre de résolution");
        for (i = 0; i <= 6; ++i) {
            Serial.println(iordersolving[i]);
        }
        //<\débug>

        //#### fonction ####
        // décalage des bits en fonction des led
        // int réorganiser_ordre_led (iordersolving[]; iLedOnOff[])

        int ivaleurpivot;

        /*
        Pour réorganiser les couleurs nous allons remplire un tableau en considérant que les couleurs sont représenté par des chiffres.
        La valeur 0 représente une absence de couleur.
        1 = Orange
        2 = Jaune
        3 = Vert
        4 = Bleu
        5 = Violet
        6 = Gris
        7 = Blanc
        Nous allons remplire un tableau de 13 case
        Certains fils peuvent avoir des attributions d'ordre fixe.
        Donc ces positions dans le tableau reste vide juqu'à ce qu'on ai déroulé toutes les conditions de LED.
        à la fin nous remplirons les trous existant pour en établir la suite logique
        */

        // transformer l'ordre de résolution en tableau de couleur avec ordre.
        for (i = 0; i <= 6; ++i) {
            itabrangement[7 + iordersolving[i]] = i + 1; // on rempli les case de 7 à 13 (on commence le compte à 0)
        }

        //<débug>
        Serial.println("ordre de résolution");
        for (i = 0; i <= 6; ++i) {
            Serial.println(iordersolving[i]);
        }
        Serial.println("tableau de rangement 1");
        for (i = 0; i <= 14; ++i) {
            Serial.println(itabrangement[i]);
        }
        //<\débug>

        // led Orange éteinte ==> le file orange à couper en dernier
        if (iLedOnOff[0] == 0) {
            itabrangement[7 + iordersolving[0]] = 0; // orange en case 0
            itabrangement[6] = 1; // 1 pour Orange
        }
        // led jaune allumé ==> ne pas couper : donc à écarter du rangement final
        if (iLedOnOff[1] == 1) {
            itabrangement[7 + iordersolving[1]] = 0; // Jaune en case 1
            itabrangement[14] = 2; // 2 pour jaune
        }
        // ce cas sera complété d'un traitement lors de l'éxécution

        // led Bleu allumé ==> le file à couper en 5ème
        if (iLedOnOff[3] == 1) { // Bleu en case 3
            itabrangement[7 + iordersolving[3]] = 0; // Bleu en case 3
            itabrangement[4] = 4; // 4 pour bleu
        }

        // led Bleu éteitnte ==> le file à couper en 4ème
        if (iLedOnOff[3] == 0) { // Bleu en case 3
            itabrangement[7 + iordersolving[3]] = 0; // Bleu en case 3
            itabrangement[3] = 4; // 4 pour bleu
        }

        // led Violet allumé ==> le file Violet à couper en 1er
        if (iLedOnOff[4] == 1) {
            itabrangement[7 + iordersolving[4]] = 0; // violet en case 4
            itabrangement[0] = 5; // 5 pour violet
        }

        // led blanc allumé ==> le file à couper en 3ème
        if (iLedOnOff[6] == 1) { // Blanc en case 6
            itabrangement[7 + iordersolving[6]] = 0; // Blanc en case 6
            itabrangement[2] = 7; // 7 pour blanc
        }

        //<débug>
        Serial.println("tableau de rangement 2");
        for (i = 0; i <= 14; ++i) {
            Serial.println(itabrangement[i]);
        }
        //<\débug>

        // Faire resortir l'ordre des couleurs
        j = 0;
        for(i = 0; i <= 6; ++i) {
            while (itabrangement[i] == 0) { // tant que la case des 7 première case du tableau n'est pas rempli par une couleur
                itabrangement[i] = itabrangement[7 + j];
                itabrangement[7 + j]=0;
                ++j;
            }
        }

        //<débug>
        Serial.println("tableau de rangement 3");
        for (i = 0; i <= 14; ++i) {
            Serial.println(itabrangement[i]);
        }
        //<\débug>

        // remettre les couleur dans iordersolving
        for (i = 0; i <= 6; ++i) {
            iordersolving[itabrangement[i] - 1] = i;
        }

        //<débug>
        Serial.println("tableau de rangement 4");
        for (i = 0; i <= 14; ++i) {
            Serial.println(itabrangement[i]);
        }
        Serial.println("ordre de résolution");
        for (i = 0; i <= 6; ++i) {
            Serial.println(iordersolving[i]);
        }
        //<\débug>
        //#### fin de la fonction ####

        ietat_fils = (digitalRead(iOrange_pin) << iordersolving[0]) | (digitalRead(iJaune_pin) << iordersolving[1]) | (digitalRead(iVert_pin) << iordersolving[2]) | (digitalRead(iBleu_pin) << iordersolving[3]) | (digitalRead(iViolet_pin) << iordersolving[4]) | (digitalRead(iGris_pin) << iordersolving[5]) | (digitalRead(iBlanc_pin) << iordersolving[6]);
        Serial.println("état de la filerie");
        Serial.println(ietat_fils);

        //  • Initialisation de la variable de boucle
        iiterration = 0;
        //  • Lancement du top départ
        Serial.println("échange avec le master : top départ et lancement du chrono");
        --initilisation;// afin de sortir de la boucle de résolution

        //<débug>
        Serial.println("ordre de résolution");
        for (i = 0; i <= 6; ++i) {
            Serial.println(iordersolving[i] + 1);
        }
        Serial.println("iiterration");
        Serial.println(iiterration);
        //<\débug>
    }

    //  Phase de run (loop)
    ietat_fils_init = (digitalRead(iOrange_pin) << iordersolving[0]) | (digitalRead(iJaune_pin) << iordersolving[1]) | (digitalRead(iVert_pin) << iordersolving[2]) | (digitalRead(iBleu_pin) << iordersolving[3]) | (digitalRead(iViolet_pin) << iordersolving[4]) | (digitalRead(iGris_pin) << iordersolving[5]) | (digitalRead(iBlanc_pin) << iordersolving[6]);
    //  o Si l’état actuel des fils différent de l’état précédent
    if (ietat_fils != ietat_fils_init) {
        Serial.println("si les branchements sont différents que précédement");
        //   Si bon fil
        /* debug
        Serial.println("ietat_fils_init");
        Serial.println(ietat_fils_init);
        Serial.println("ietat_fils");
        Serial.println(ietat_fils);
        Serial.println("ietat_fils + (1 << iiterration)");
        Serial.println(ietat_fils + (1 << iiterration));
        Serial.println("iiterration");
        Serial.println(iiterration);
        Serial.println("(1 << iiterration) & ietat_fils_init");
        Serial.println((1 << iiterration) & ietat_fils_init);
        fin débug*/
        if (ietat_fils + (1 << iiterration) == ietat_fils_init) {
            Serial.println("Dans la configuration attendu");
            //  • Case temps
            //  o Orange : débranché avec une dizaine paire ?
            //  o Vert : débranché sur un 3,4 ou 5 au niveau des secondes ?
            //  o Bleu : débranché avec 5 affiché ?
            //  o Blanc : débranché lorsque digit affiche 1 ?
            //   Si erreur : envoie d’un message d’erreur au maitre
            //   Si ok : next
            //  • Incrémentation de la variable de résolution
            ++iiterration;
            Serial.println("(1 << iiterration) & ietat_fils_init");
            Serial.println((1 << iiterration) & ietat_fils_init);
        }

        //   Sinon
        //  • Envoie d’une erreur au maitre
        else {
            Serial.println("BOOM");
            Serial.println(ietat_fils);
            for (j = 0; j <= 4; j++) {
                for (i = 0; i <= 6; i++) {
                    digitalWrite(iLed_tab[i], HIGH);
                }
                delay(200);
                for (i = 0; i <= 6; i++) {
                    digitalWrite(iLed_tab[i], LOW);
                }
                delay(200);
            }
            // réafficher sur les leds pour la résolution.
            for (i = 0; i <= 6; i++) {
                digitalWrite(iLed_tab[i], iLedOnOff[i]);
            }
        }
        //  o Sinon
        //   Rien
    }
    //  o Sauvegarde de l’état final des fils
    ietat_fils = ietat_fils_init;

    //  o Si variable de résolution =7
    if (iiterration == 7) {
        //   Mettre le module en veille.
        Serial.println("BRAVO");
        Serial.println(ietat_fils);
        i = 0;
        while (1) {
            for (j = 0; j <= 6; ++j) {
                digitalWrite(iLed_tab[j], LOW);
            }
            if (i == 6) {

                digitalWrite(iLed_tab[i], HIGH);
                digitalWrite(iLed_tab[0], HIGH);
                i = 0;
            }
            else
            {
                digitalWrite(iLed_tab[i], HIGH);
                digitalWrite(iLed_tab[i + 1], HIGH);
                ++i;
            }
            delay(200);
        }
    }

    //  • Si valeur incrémentation = un file déjà débranché
    if ((1 << iiterration) & ietat_fils_init) {
        //  o Incrémentation de la variable de résolution
        Serial.println("on saute un file déjà débranché");
        ++iiterration;
    }
    if ((iordersolving[1] == iiterration) & iLedOnOff[1]) { // on saute une iterration si c'est le tour du file jaune et que la led est allumé.
      //  o Incrémentation de la variable de résolution
        Serial.println("on saute un file déjà débranché parceque le jaune");
        ++iiterration;
    }

    delay(100); //  • Tempo 100ms (pour éviter les effets de rebond)
}

