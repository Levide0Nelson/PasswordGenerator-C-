#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>    // Pour le mélange avec std::shuffle
#include <vector>
#include <cmath>
#include <windows.h>   // Pour l'encodage UTF-8
#include <fstream>     // Pour la gestion du fichier de sauvegarde
#include <vector>

using namespace std;

struct PasswordResult
{
    string password;
    double entropie;
    string laSuggestion;
};

// Fonction de calcul de l'entropie et qui estime la robustesse du mot de passe généré

double calculateEntropy(int length, int charsetSize)
{
    return length* log2(charsetSize);
}



// Fonction pour les suggestions après calcul de l'entropie

string suggestion(double entropie)
{
    if (entropie < 40)
    {
        return "Mot de passe faible. Nous vous recommandons d'augmenter la longueur ou d'ajouter des symboles.";
    }
    else if (entropie < 60)
    {
        return "Mot de passe moyen. Pour plus de sécurité, nous vous recommandons d'ajouter des majuscules ou des chiffres.";
    }
    else if (entropie < 80)
    {
        return "Mot de passe robuste. Vous pouvez l'utiliser en toute sécurité.";
    }
    else
        return "Mot de passe très fort ! Excellent pour les usages sensibles";
}




// Fonction génératrice de mot de passe direct

string generatePassword(int length)
{
    string charset =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"       // Les différents caractères qui seront pris en compte pour le mot de passe
        "0123456789"
        "!@#$%^&()_+-=[]{}|;:,.<>?-";

    string password;
    mt19937 rng(static_cast<unsigned int>(time(nullptr))); // Moteur de générateur de bits aléatoire qui génère des entiers aléatoires sur 32bits
    uniform_int_distribution<> dist(0, charset.size()-1);  // filtre qui génère des entiers dans une plage donnée avec équibrobabilité pour chaque valeur

    for (int i = 0; i < length; i++)
    {
        password += charset[dist(rng)]; // génération d'un nombre aléatoire entre 0 et taille de charset -1 pour ajouter un caractère de charset à password
    }

    return password;
}




// Fonction génératrice de mot de passe donnant le choix à l'utilisateur de choisir ses options de Personnalisation.

string generatePasswordOption(int length, bool useLower, bool useUpper, bool useDigits, bool useSymbols) // Fonction de génération du mot de passe
{
    string charset = "";
    vector<char> requiredChars;

    string password;
    mt19937 rng(static_cast<unsigned int>(time(nullptr))); // Moteur de générateur de bits aléatoires qui génère un entier non signé de 32 bits en utilisant l'algorithme de Mersenne Twister

    if (useLower)
    {
        string lower = "abcdefghijklmnopqrstuvwxyz";
        charset += lower;
        requiredChars.push_back(lower[rng() % lower.size()]);
    }                                                         // Les différents caractères choisis aléatoirement
                                                                // prenant en compte l'algorithme de Mersenne Twister (mt19937)
    if (useUpper)
    {
        string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        charset += upper;
        requiredChars.push_back(upper[rng() % upper.size()]);
    }
    if (useDigits)
    {
        string digits = "0123456789";
        charset += digits;
        requiredChars.push_back(digits[rng() % digits.size()]);
    }
    if (useSymbols)
    {
        string symbols = "!@#$%^&()_+-=[]{}|;:,.<>?-";
        charset += symbols;
        requiredChars.push_back(symbols[rng() % symbols.size()]);
    }

    if (charset.empty())
    {
        return "Erreur : Veuillez choisir au moins un type de caractère pour la génération";
    }

    uniform_int_distribution<> dist(0, charset.size()-1);

    // Ajout des caractères aléatoires restants
    for (int i = 0; i < (length - requiredChars.size()); ++i)
    {
        password += charset[dist(rng)];
    }

    // Ajout des caractères obligatoires ajoutés dans le requiredChars
    for (char c : requiredChars)
    {
        password += c;
    }

    // Mélange final pour le password
    shuffle(password.begin(), password.end(), rng);

    return password;
}




// Fonction pour la génération finale

vector<PasswordResult> generation(int const nbreDePassword )
{
    int length;
    char lower, upper, digits, symbols, choice;
    vector<PasswordResult> results;

    cout << "Je préfère choisir moi même (o/n): ";
    cin >> choice;

    if (choice == 'o' || choice == 'O')
    {
        for (int i = 0; i < nbreDePassword; ++i)
        {
            do
            {
                cout << "Longueur du mot de passe que vous voulez générer: ";
                cin >> length;
                cout << endl;
            } while (length <= 0);

            do
            {
                cout << "Inclure des lettres miniscules ? (o/n) : ";
                cin>> lower;
                cout << endl;
            } while (lower !='o' && lower != 'O' && lower != 'n' && lower != 'N');

            do
            {
                cout << "Inclure des lettres majuscules ? (o/n) : ";
                cin >> upper;
                cout << endl;
            }while (upper !='o' && upper != 'O' && upper != 'n' && upper != 'N');

            do
            {
                cout << "Inclure des chiffres ? (o/n) : ";
                cin >> digits;
                cout << endl;
            }while (digits !='o' && digits != 'O' && digits != 'n' && digits != 'N');

            do
            {
                cout << "Inclure des symboles ? (o/n) : ";
                cin >> symbols;
                cout << endl;
            }while (symbols !='o' && symbols != 'O' && symbols != 'n' && symbols != 'N');

            if ((lower == 'n' || lower == 'N') && (upper == 'n' || upper == 'N') && (digits == 'n' || digits == 'N') && (symbols == 'n' || symbols == 'N') )
            {
                cout << "Erreur : Veuillez choisir au moins un type de caractère pour continuer la génération" << endl;
                int nbreDePasswordcontinue = nbreDePassword;
                return generation(nbreDePasswordcontinue);
            }
            else
            {
                PasswordResult result;
                result.password = generatePasswordOption(
                    length,
                    lower == 'o' || lower == 'O',
                    upper == 'o' || upper == 'O',
                    digits == 'o' || digits == 'O',
                    symbols == 'o' || symbols == 'O'
                );

                // Pour l'évaluation de la robustesse
                int charSize;
                if (lower == 'o' || lower == 'O')
                    charSize += 26;
                if (upper == 'o' || upper == 'O')
                    charSize += 26;
                if (digits == 'o' || digits == 'O')
                    charSize += 10;
                if (symbols == 'o' || symbols == 'O')
                    charSize += 28;

                result.entropie = calculateEntropy(length, charSize);
                result.laSuggestion = suggestion(result.entropie);
                results.push_back(result);
            }
        }
        return results;

    }
    else if (choice == 'n' || choice == 'N')
    {
        for (int i = 0; i < nbreDePassword; ++i)
        {
            PasswordResult result;

            do
            {
                cout << "Longueur du mot de passe que vous voulez générer: ";
                cin >> length;
                cout << endl;
            } while (length <= 0);

            result.password = generatePassword(length);

            int charsetSize = 26+26+10+26;
            result.entropie = calculateEntropy(length, charsetSize);
            result.laSuggestion = suggestion(result.entropie);
            results.push_back(result);
        }

        return results;
    }
    else
    {
        cout << "Vous devez entrer 'o' ou 'n'" << endl;
        return generation(nbreDePassword); // Capture le mot de passe pour retourner le password générer
    }
}

// Fonction pour la création du fichier de sauvagarde des mots de passe générés (en cas de besoin) avec horodatage uniquement
void savePassword (const string& password, int index)
{
    ofstream fichier("passwords_Save.txt", ios::app); //mode ajout sans écrasement des données à l'ouverture du fichier de sauvegarde

    if (fichier.is_open())
    {
        // Obtention de la date et de l'heure courante
        time_t heure_courante = time(nullptr);
        char horodatage[100];
        strftime(horodatage, sizeof(horodatage), "%Y-%m-%d %H:%M:%S", localtime(&heure_courante));

        // Ecriture dans le fichier
        fichier << "Mot de passe #"<< index << " : " << password << " [" << horodatage << "]\n";
        fichier.close();
        cout << "Mot(s) de passe sauvegardé(s) automatiquement avec succès.";
    }
    else
        cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde. Une erreur a dû se produire.\n";
}

// Fonction pour création d'un fichier de sauvegarde avec horodatage et contexte d'utilisation du mot de passe généré par l'utilisateur
void savePasswordWithContextAndTime(const string& password, const string& usingContext, int index)
{
    ofstream fichier("password_Saved_Context_and_Time.txt", ios::app);
    if (fichier.is_open())
    {
        // Obtention de l'heure et de la date courante
        time_t heureCourante = time(nullptr);
        char horodatage[100];
        strftime(horodatage, sizeof(horodatage), "%d-%m-%Y  %H:%M:%S", localtime(&heureCourante));

        // Ecriture dans le fichier de sauvegarde
        fichier << "Mot de passe #" << index << " : " << password << "\n";
        fichier << "Créé le : " << horodatage << "\n";
        fichier <<  "Contexte de création : " << usingContext << "\n";
        fichier << "\n";
        fichier.close();
    }
    else
        cerr << "Impossible d'ouvrir le ficher de sauvegarde. Une erreur a dû se produire.\n";
}

// Fonction pour l'affichage de l'historique des mots de passe sauvegardés dans le fichier de sauvegarde
void showPasswordSaved()
{
    ifstream fichier ("password_Saved_Context_and_Time.txt");
    if (fichier.is_open())
    {
        string ligne;
        cout << "\n\t===HISTORIQUE DES MOTS DE PASSE GENERES===\t\n";
        while (getline(fichier,ligne))
        {
            cout << ligne << endl;
        }
        fichier.close();
    }
    else
    {
        cerr << "Une Erreur s'est produite : Impossible d'ouvrir le fichier d'Historique.\n";
    }
}

// Fonction pour suppression du fichier d'historique des mots de passe générés
void deletePasswordSaved()
{
    char confirmation;
    cout << "Êtes-vous sûr de vouloir vider le fichier de sauvegarde ? (o/n) : ";
    cin >> confirmation;

    if (confirmation == 'o' || confirmation == 'O')
    {
        ofstream fichier ("password_Saved_Context_and_Time.txt", ios::trunc);  // ouverture du fichier de sauvegarde en mode écrasement
        if (fichier.is_open())
        {
            fichier.close();
            cout << "Historique vidé avec succès.\n";
        }
        else
        {
            cerr << "Erreur : Impossible de vider le fichier d'Historique\n";
        }
    }
    else
        cout << "Opération annulée.\n";
}

void researchPasswordByKeyword()
{
    cout << "\t\tRecherche par mot-clé\t\t" << endl;
    cin.ignore(); // Nettoyage du buffer. Essentiel pour utiliser getline()
    string keyword;

    cout << "Entrer le mot clé à rechercher : ";
    getline(cin, keyword);

    ifstream fichier("password_Saved_Context_and_Time.txt");

    if (fichier.is_open())
    {
        string ligne;
        bool blocEnCours = false;
        bool blocCorrespondant = false;
        bool auMoinsUnBlocTrouve = false;
        vector<string> lignes {""};

        cout << "\n\t\t---Résultats de la recherche---\t\t\n";

        while (getline(fichier, ligne))
        {
            // Détection du début d'un bloc (mot de passe + Horodatage + Contexte)
            if (ligne.find("Mot de passe #") != string::npos ) // Où string::npos est utilisé pour indiquer indiquer qu'une
            {                                                                                                                   // opération de recherche sur une chaîne de caractère n'a pas trouvé de correspondance
                blocEnCours = true;
                blocCorrespondant = false;
                lignes.clear();
            }

            // Vérifie si le mot clé est présent dans le bloc en cours
            if (blocEnCours)
            {
                lignes.push_back(ligne);
                if ((ligne.find("Contexte de création : ") != string::npos && ligne.find(keyword) != string::npos) || (ligne.find("Créé le") != string::npos && ligne.find(keyword) != string::npos))
                    blocCorrespondant = true;
            }

            // Fin de bloc (ligne vide)
            if (blocEnCours && ligne.empty())
            {
                if (blocCorrespondant)
                {
                    for (const string& elmt : lignes)
                        cout << elmt << endl;
                    cout << endl;
                    auMoinsUnBlocTrouve = true;
                }
                blocEnCours = false;
                blocCorrespondant = false;
                lignes.clear();
            }
        }
        fichier.close();
        if ( !auMoinsUnBlocTrouve )
        {
            cout << "Aucun mot de passe n'est associé au mot clé \"" << keyword << "\".\n";
        }
    }
    else
    {
        cerr << "Une erreur s'est produite! Impossible d'ouvrir le fichier de sauvegarde.\n";
    }
}


// Fonction principale

int main()
{
    SetConsoleOutputCP(CP_UTF7);  // Pour l'affichage avec l'encodage UNICODE afin de bien afficher les caractères accentués dans la console

    cout << "Bienvenu dans le générateur de mot de passe" << endl;
    cout << endl;

    int choice;
    string usingContext;

    while (choice != 6)
    {
        cout << "\n===MENU===\n";
        cout << "1. Générer un mot de passe.\n";
        cout << "2. Générer plusieurs mots de passe\n";
        cout << "3. Afficher l'historique des mots de passe générés.\n";
        cout << "4. Rechercher un mot de passe par mot-clé dans l'historique\n";
        cout << "5. Supprimer l'historique des mots de passe générés.\n";
        cout << "6. Quitter le programme\n";
        cout << endl;
        cout << "Votre choix : ";
        cin >> choice;

        switch (choice)
        {
            case 1 :
                {
                    cout << "Avant de continuer souhaitez-vous choisir les caractères qui seront inclus dans votre mot de passe généré ou préférez-vous qu'on le fasse à votre place ?";
                    cout << endl;
                    vector <PasswordResult> results = generation(1); // Génère le mot de passe une fois
                    cout << "Mot de passe généré : " << results[0].password << endl;
                    cout << results[0].laSuggestion << endl;
                    cout << endl;
                    static int i = 0;

                    char saveChoice;
                        cout << "Voulez- vous sauvegarder le mot de passe généré (o/n)? : ";
                        cin >> saveChoice;
                        cin.ignore();
                        if (saveChoice == 'o' || saveChoice == 'O')
                        {
                            savePassword(results[0].password, i+1);
                            cout << "Où comptez-vous utiliser ce mot de passe ? \n";
                            cout << "Donner une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                            getline(cin, usingContext);
                            savePasswordWithContextAndTime(results[0].password, usingContext, i+1);
                            cout << endl;
                        }
                        else if (saveChoice == 'n' || saveChoice == 'N')
                        {
                             cout <<" Votre mot de passe ne sera pas sauvegardé. Vous ne pourrez plus avoir accès à lui en cas de besoin"<<endl;
                             cout << "Êtes-vous sûr de ne pas vouloir sauvegarder le mot de passe (o/n) ? : ";
                             cin >> saveChoice;
                             cin.ignore();
                             if (saveChoice == 'o' || saveChoice == 'O')
                             {
                                 cout << "Sauvegarde non effectuée.";
                             }
                             else if (saveChoice == 'n' || saveChoice == 'N')
                             {
                                 savePassword(results[0].password, i+1);
                                 cout << endl;
                                 cout << "Où comptez-vous utiliser ce mot de passe ? \n";
                                 cout << "Donner une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                                 getline(cin, usingContext);
                                 savePasswordWithContextAndTime(results[0].password, usingContext, i+1);
                                 cout << endl;
                             }
                             else
                             {
                                 savePassword(results[0].password, i+1);
                                 cout << "Où comptez-vous utiliser ce mot de passe ? \n";
                                 cout << "Donner une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                                 getline(cin, usingContext);
                                 savePasswordWithContextAndTime(results[0].password, usingContext, i+1);
                                 cout << "Une erreur s'est produite : Vous devez entrer 'o' ou 'n'" << endl;
                                 cout << "Les mots de passe ont été sauvegarder pour éviter tout risque. Vous pouvez vider l'historique dans le menu\n";
                             }
                        }
                        else
                        {
                            savePassword(results[0].password, i+1);
                            cout << "Où comptez-vous utiliser ce mot de passe ? \n";
                            cout << "Donner une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                            getline(cin, usingContext);
                            savePasswordWithContextAndTime(results[0].password, usingContext, i+1);
                            cout << "Une erreur s'est produite : Vous devez entrer 'o' ou 'n'" << endl;
                            cout << "Les mots de passe ont été sauvegarder pour éviter tout risque. Vous pouver vider l'historique dans le menu\n";
                        }
                    break;
                }

            case 2 :
                {
                    int nbreDePassword;

                    cout << "Combien de mots de passe voulez-vous générer aujourd'hui ? " << endl;
                    cin >> nbreDePassword;
                    cin.ignore();  // pour vider le buffer
                    cout << "Avant de continuer souhaitez-vous choisir les caractères qui seront inclus dans votre mot de passe généré ou préférez-vous qu'on le fasse à votre place ?";
                    cout << endl;
                    vector <PasswordResult> results = generation(nbreDePassword);

                    for (int i =0; i < results.size(); ++i)
                    {
                        cout << "Mot de passe généré N°# " << (i+1) << " : " << results[i].password << endl;
                        cout << results[i].laSuggestion << endl;
                        cout << endl;
                    }
                    cout << endl;
                    char saveChoice;
                    cout << "Voulez- vous sauvegarder les mots de passe générés (o/n)? : ";
                    cin >> saveChoice;
                    cin.ignore();
                    if (saveChoice == 'o' || saveChoice == 'O')
                    {
                        for (int i =0; i < results.size(); ++i)
                        {
                            savePassword(results[i].password, i+1);
                            cout << "\nOù comptez-vous utiliser le mot de passe #" << i+1 << " ?\n";
                            cout << "Donnez une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                            getline(cin, usingContext);
                            savePasswordWithContextAndTime(results[i].password, usingContext, i+1);

                        }
                    }
                    else if (saveChoice == 'n' || saveChoice == 'N')
                    {
                        cout <<" Vos mots de passe ne seraont pas sauvegardés. Vous ne pourrez plus avoir accès à eux en cas de besoin"<<endl;
                        cout << "Êtes-vous sûr de ne pas vouloir sauvegarder les mots de passe (o/n) ? : ";
                        cin >> saveChoice;
                        cin.ignore();
                        if (saveChoice == 'o' || saveChoice == 'O')
                        {
                            cout << "\nSauvegarde non effectuée.\n";
                        }
                        else if (saveChoice == 'n' || saveChoice == 'N')
                        {
                            for (int i =0; i < results.size(); ++i)
                            {
                                savePassword(results[i].password, i+1);
                                cout << "\nOù comptez-vous utiliser le mot de passe #"<< i+1 << " ?\n";
                                cout << "Donnez une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                                getline(cin, usingContext);
                                savePasswordWithContextAndTime(results[i].password, usingContext, i+1);
                                cout << endl;
                            }
                        }
                        else
                        {
                            for (int i =0; i < results.size(); ++i)
                                savePassword(results[i].password, i+1);
                            cout << "\nUne erreur s'est produite : Vous devez entrer 'o' ou 'n'" << endl;
                            cout << "Les mots de passe ont été sauvegarder pour éviter tout risque. Vous pouver vider l'historique dans le menu\n";
                        }
                    }
                    else
                    {
                        for (int i =0; i < results.size(); ++i)
                        {
                            savePassword(results[i].password, i+1);
                            cout << "\nOù comptez-vous utiliser le mot de passe #"<< i+1 << " ?\n";
                            cout << "Donnez une description claire pour vous en souvenir plus tard. Nous la sauvegarderont dans votre historique : " << endl;
                            getline(cin, usingContext);
                            savePasswordWithContextAndTime(results[i].password, usingContext, i+1);
                        }
                        cout << "\nUne erreur s'est produite : Vous devez entrer 'o' ou 'n'" << endl;
                        cout << "Les mots de passe ont été sauvegarder pour éviter tout risque. Vous pouver vider l'historique dans le menu\n";
                    }

                    break;
                }
            case 3 :
            {
                showPasswordSaved();
                break;
            }
            case 4:
                {
                    researchPasswordByKeyword();
                    break;
                }
            case 5:
                {
                    deletePasswordSaved();
                    break;
                }
            case 6 :
                {
                    cout << "Au revoir !" << endl;
                    break;
                }
            default:
                cout << "Choix invalide. Veuillez réessayer." << endl;
        }
    }

    return 0;
}
