#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>    // Pour le mélange avec std::shuffle
#include <vector>
#include <cmath>
#include <utility>

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
        for (int i = 0; i<nbreDePassword; ++i)
        {
            do
            {
                cout << "Longueur du mot de passe que vous voulez générer: ";
                cin >> length;
            } while (length <= 0);

            do
            {
                cout << "Inclure des lettres miniscules ? (o/n) : ";
                cin>> lower;
            } while (lower !='o' && lower != 'O' && lower != 'n' && lower != 'N');

            do
            {
                cout << "Inclure des lettres majuscules ? (o/n) : ";
                cin >> upper;
            }while (upper !='o' && upper != 'O' && upper != 'n' && upper != 'N');

            do
            {
                cout << "Inclure des chiffres ? (o/n) : ";
                cin >> digits;
            }while (digits !='o' && digits != 'O' && digits != 'n' && digits != 'N');

            do
            {
                cout << "Inclure des symboles ? (o/n) : ";
                cin >> symbols;
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
                return results;
            }
        }

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



// Fonction principale

int main()
{

    cout << "Bienvenu dans le générateur de mot de passe" << endl;
    cout << endl;

    int choice;

    while (choice != 3)
    {
        cout << "\n===MENU===\n";
        cout << "1. Générer un mot de passe.\n";
        cout << "2. Générer plusieurs mots de passe\n";
        cout << "3. Quitter le programme\n";
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
                    break;
                }

            case 2 :
                {
                    int nbreDePassword;

                    cout << "Combien de mots de passe voulez-vous générer aujourd'hui ? " << endl;
                    cin >> nbreDePassword;

                    cout << "Avant de continuer souhaitez-vous choisir les caractères qui seront inclus dans votre mot de passe généré ou préférez-vous qu'on le fasse à votre place ?";
                    cout << endl;
                    vector <PasswordResult> results = generation(nbreDePassword);

                    for (int i =0; i < results.size(); ++i)
                        {
                            cout << "Mot de passe généré N°#" << (i+1) << " : " << results[i].password << endl;

                            cout << results[i].laSuggestion << endl;
                            cout << endl;
                        }
                        cout << endl;
                    break;
                }

            case 3 :
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
