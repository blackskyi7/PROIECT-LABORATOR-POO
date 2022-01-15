#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;
//Clasa de baza
class Base {
    public:
        int debt;
        int weight;
        string last_name, first_name, city;
        virtual void show() = 0;
        virtual bool category() = 0;
        virtual int debt_retrieve() = 0;
        virtual int money_retrieve() = 0;
        //fct virtuale, urmeaza a fi redefinite
};

class Players : public Base
{
public:
    int debt;
    int weight;
    string first_name;
    string last_name;
    string city;
    int competition_no; //Numarul de concurs 1 - 99
    int guardian_no; //id supervizor asociat


    Players(int, int, string, string, string, int); //constructor
    void show(); //metoda pentru afisarea unui concurent
    int debt_retrieve(); //metoda care returneaza datoria supervizorului
    int money_retrieve(); //aceasta returneaza premiul pe care va trebui sa il primeasca
    bool category(); //metoda pentru a putea vedea din ce categorie face parte utilizatorul
    ~Players(); //destructor
};

Players::Players(int competition_no, int guardian_no, string first_name, string last_name, string city,int weight)
{
    this->competition_no = competition_no;
    this->guardian_no = guardian_no;
    this->first_name = first_name;
    this->last_name = last_name;
    this->city = city;
    this->debt = 10000 + rand() % 90001;
    this->weight = weight;
}
void Players::show()
{
    cout<<"First name: "<<this->first_name<<", Last name: "<<this->last_name<<", City: "<<this->city<<", Debt: "<<this->debt<<", Weight: "<<this->weight<<" Competition No:"<<competition_no<<endl;

}
int Players::debt_retrieve()
{
    return this->debt;
}
int Players::money_retrieve()
{
    return 0;
}
bool Players::category()
{
    return 1;
}

class Guardians : public Base
{
public:
    int debt;
    int weight;
    string mask;
    string first_name;
    string last_name;
    string city;
    int money;

    Guardians(string,string,string,string,int);//constructor
    void show(); //afisarea
    int debt_retrieve(); //metoda care returneaza datoria supervizorului
    int money_retrieve(); //aceasta returneaza premiul pe care va trebui sa il primeasca
    bool category(); //categoria din care face parte
    ~Guardians(); //destructor

};


Guardians::Guardians(string first_name, string last_name, string city, string mask, int weight)
{
    this->first_name = first_name;
    this->last_name = last_name;
    this->city = city;
    this->mask = mask;
    this->weight = weight;
    this->debt = 10000 + rand() % 90001;
    this->money = 0;
}
void Guardians::show()
{
    cout<<"First name: "<<this->first_name<<", Last name: "<<this->last_name<<", City: "<<this->city<<", Mask: "<<this->mask<<" "<<"Money winnings: "<<this->money - this->debt<<endl;

}
int Guardians::debt_retrieve()
{
    return this->debt;
}
int Guardians::money_retrieve()
{
    return this->money;
}
bool Guardians::category()
{
    return 0;
}

struct Helper
{
    string first_name, last_name, city;
    //folosesc structura pentru a citi la primul pas din main
};

void RedLightGreenLight(Base **people)
//ca parametru e lista concurentilor
{
    int i, j;
    int n = 108;
    for(i=9; i<n; i++)
        if(static_cast<Players*>(people[i])->competition_no % 2 == 0)
            //verific daca numarul concurentului este par
        {
            static_cast<Guardians*>(people[static_cast<Players*>(people[i])->guardian_no])->money += people[i]->debt_retrieve();
            //se adauga la premiul supervizorului datoria concurentului eliminat
            for(j=i; j<n; j++)
                people[j] = people[j+1];
            //aici se elimina concurentul gasit cu numar de concurs par
            n--; //se scade numarul total de concurenti
            i--; //se verifica din nou pozitia i deoarece pe aceasta este un nou concurent
        }
}
void TugOfWar(Base **people)
//ca parametru e lista concurentilor
{
    int i,j,k;
    int weight1 = 0;
    int weight2 = 0;
    int weight3 = 0;
    int weight4 = 0; //greutatile echipelor pentru jocul de tug of war
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(people+9,people+59,default_random_engine(seed)); //amestec concurentii


    //parcurg lista de la primul concurent si ii adaug  pe rand in echipe de cate 12

    //de la 0 la 8 sunt supraveghetorii, fiind amestecat dupa, primii 2 ar fi ramas fara echipa deci i-am considerat trecuti in etapa urmatoare
    for(i=11; i<23; i++)
    {
        weight1 += people[i]->weight; //prima echipa e de la 11 pana la 22
        weight2 += people[i+12]->weight; // a doua e de la 23 pana la 34
        weight3 += people[i+24]->weight; // a treia e de la 35 la 46
        weight4 += people[i+36]->weight; //a patra e de la 47 la 58
    }
    int n = 59;
    //nr de utilizatori (dintre care primii 9 sunt supervizorii)

    if (weight1>weight2)
    //verific care echipa e mai grea si elimin pe cea mai usoara, repet acelasi lucru si pentru echipa 3 si 4 dupa
    {
        for(k=23; k<35; k++)
            static_cast<Guardians*>(people[static_cast<Players*>(people[k])->guardian_no])->money += people[k]->debt_retrieve();
        for(i=0; i<12; i++)
        {
            for(j=23; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
    }
    else
    {
        for(k=11; k<23; k++)
            static_cast<Guardians*>(people[static_cast<Players*>(people[k])->guardian_no])->money += people[k]->debt_retrieve();
        for(i=0; i<12; i++)
        {
            for(j=11; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
    }
    if (weight3>weight4)
    {
        for(k=35; k<47; k++)
            static_cast<Guardians*>(people[static_cast<Players*>(people[k])->guardian_no])->money += people[k]->debt_retrieve();
        for(i=0; i<12; i++)
        {
            for(j=35; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
    }
    else
    {
        for(k=23; k<35; k++)
            static_cast<Guardians*>(people[static_cast<Players*>(people[k])->guardian_no])->money += people[k]->debt_retrieve();
        for(i=0; i<12; i++)
        {
            for(j=23; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
    }
}

void Marbles (Base**people)
//ca parametru e lista concurentilor
{
    int i,j;
    int no1, no2;
    int n = 34; //nr de concurenti din concurs
    for(i=9; i<n; i++)
    {
        no1 = rand()%100;
        no2 = rand()%100;
        //generam 2 numere random, folosite pentru Marbles, cel cu nr cel mai mic castiga
        if(no1>no2)
        {
            static_cast<Guardians*>(people[static_cast<Players*>(people[i])->guardian_no])->money += people[i]->debt_retrieve();
            for(j=i; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
        else
        {
            static_cast<Guardians*>(people[static_cast<Players*>(people[i+1])->guardian_no])->money += people[i+1]->debt_retrieve();
            for(j=i+1; j<n; j++)
                people[j] = people[j+1];
            n--;
        }
    }
}

void Genken(Base **people, int winning)
{
    int i,j;
    for(i=9; i<22; i++)
        for(j=i+1; j<22; j++)
            if(static_cast<Players*>(people[i])->competition_no < static_cast<Players*>(people[j])->competition_no)
            {
                Players* temp = static_cast<Players*>(people[i]);
                people[i] = people[j];
                people[j] = temp;
            }
    string moves[3] = {"rock", "scissors", "paper"}; //lista cu mutarile
    string pick1;
    string pick2; //mutarile concurentilor
    Players *winner = static_cast<Players*>(people[9]);
    //castigator va fi considerat primul concurent, fiind cum e cerut in cerinta primul jucator ca are numarul de concurs cel mai mare
    for(i=10; i<22; i++)
    {

        pick1 = moves[rand() % 3];
        pick2 = moves[rand() % 3];
        while(pick1 == pick2)
        {
            pick1 = moves[rand() % 3];
            pick2 = moves[rand() % 3];
        }
        if((pick1 == "rock" && pick2 == "scissors") || (pick1 == "paper" && pick2 == "rock") || (pick1 == "scissors" && pick2 == "paper")) //criteriile de castigare pentru jocul genken
            static_cast<Guardians*>(people[static_cast<Players*>(people[i])->guardian_no])->money += people[i]->debt_retrieve();
        else
        {
            static_cast<Guardians*>(people[winner->guardian_no])->money += people[i+1]->debt_retrieve();
            winner = static_cast<Players*>(people[i]);

        }
    }
    cout<<"\n\nWinner of Genken and Squid Game is:\n";
    winner->show();
    cout<<"Winner won: "<<winning - winner->debt;
    static_cast<Guardians*>(people[winner->guardian_no])->money += static_cast<Guardians*>(people[winner->guardian_no])->debt_retrieve() * 10;
    //supervizorul care il are pe castigator primeste propria sa datorie * 10 ca in cerinta
}

int main()
{

    int i, j;
    int winning = 0; //castigul total pentru castigator
    Base **people = new Base*[108];

    ifstream f;
    f.open("file.in");
    Helper temp[108];
    for(i=0; i<108; i++) //citesc datele din fisier, prenume, nume, oras
        f>>temp[i].first_name>>temp[i].last_name>>temp[i].city;
    f.close(); //inchid fisier

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(temp,temp+108,default_random_engine(seed)); //acum datele utilizatorilor sunt aleator aranjate
    //pentru a fi aleator am folosit shuffle

    string mask[9] = {"triangle " , "triangle ", "triangle", "square", "square", "square", "circle", "circle", "circle"};
    //amestecam mastile pentru a fi random echipele
    shuffle(mask,mask+9,default_random_engine(seed));
    for(i=0; i<108; i++)
    {
        //daca indexul e mai mic ca 9 sunt supervizori, dupa sunt concurenti
        if(i<9)
            people[i] = new Guardians(temp[i].first_name,temp[i].last_name,temp[i].city,mask[i], 50 + (rand() % 51));

        else
            people[i] = new Players(i-8,i%9,temp[i].first_name,temp[i].last_name,temp[i].city, 50 + (rand() % 51));

        winning += people[i]->debt_retrieve(); //se adauga la premiul final toata datoria

    }

    //incepe jocul, apelam fiecare functie pentru fiecare joc in parte
    RedLightGreenLight(people);
    cout<<"Winners of Red Light Green Light:\n\n";
    for(i=9; i<59; i++)
        people[i]->show();

    TugOfWar(people);
    cout<<"\n\nWinners of Tug of War:\n\n";
    for(i=9; i<35; i++)
        people[i]->show();

    Marbles(people);
    cout<<"\n\nWinners of Marbles:\n\n";
    for(i=9; i<22; i++)
        people[i]->show();
    Genken(people,winning);
    //3 variabile in care salvam totalul sumei castigate de fiecare echipa de supraveghetori
    int tri = 0;
    int sq=0;
    int circ=0;
    //sortam supervizorii dupa suma castigata care inseamna premiul de la concurentii eliminati - datoria lor
    for(i=0; i<9; i++)
        for(j=i+1; j<9; j++)
        {
            if(people[i]->money_retrieve()-people[i]->debt_retrieve() < people[j]->money_retrieve()-people[j]->debt_retrieve())
            {
                Guardians* temp = static_cast<Guardians*>(people[i]);
                people[i] = people[j];
                people[j] = temp;
            }

        }
    //adaugam la sumele castigate de echipe suma fiecarui supervizor din acea echipa
    for(i=0; i<9; i++)
    {
        if(static_cast<Guardians*>(people[i])->mask == "triangle")
            tri += people[i]->money_retrieve();
        else if(static_cast<Guardians*>(people[i])->mask == "square")
            sq += people[i]->money_retrieve();
        else if(static_cast<Guardians*>(people[i])->mask == "circle")
            circ += people[i]->money_retrieve();
    }
    cout<<"\n\nGuardians sorted:\n";
    for(i=0; i<9; i++)
        people[i]->show();

    //afisam echipa cu cei mai multi bani stransi
    if (tri>sq && tri>circ)
        cout<<"\nGuardian team Triangle got the most money";
    if (sq>tri && sq>circ)
        cout<<"\nGuardian team Square got the most money";
    if (circ>sq && circ>tri)
        cout<<"\nGuardian team Circle got the most money";
}
