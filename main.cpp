#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <functional>


using namespace std;

class vasuti_halozat_allapot;

class termek
{
    string nev, honnan, hova;
    int mennyiseg;
public:
    termek(){}
    termek(string _nev, string _honnan, string _hova, int _mennyiseg)
    {
        nev=_nev;
        honnan=_honnan;//hibás meggondolás
        hova=_hova;//hibás meggondolás következménye
        mennyiseg=_mennyiseg;
    }
    string get_nev()
    {return nev;}
    string get_honnan()
    {return honnan;}
    string get_hova()
    {return hova;}
    int get_mennyiseg()
    {return mennyiseg;}
    void mennyiseg_modosit(int _mennyiseg)
    {mennyiseg=_mennyiseg;}
};

//ostream& operator<<(ostream& out, termek t)
//{
//    out<<"Termek_nev: "<<t.get_nev()<<", kiindulo allomas: "<<t.get_honnan()<<", celallomas: "<<t.get_hova()<<", mennyiseg: "<<t.get_mennyiseg()<<endl;
//    return out;
//}

class kocsi
{
    string nev;
    int toltottseg;
    map<string, int> rakomany;
    int rakomany_merete;
    pair<bool, string> hely;//false, ha allomason van, true, ha vonaton
public:
    kocsi(){}
    kocsi(string _nev, int _kapacitas)
    {
        nev=_nev;
        toltottseg=0;
    }
    string get_nev()
    {return nev;}
//    int get_aktualis_kapacitas()
//    {return (kapacitas-toltottseg);}
    bool csatlakoztatva_e()
    {return hely.first;}
//    map<string, termek> get_rakomany()
//    {return rakomanyok;}
//    bool felpakol(vector<termek> _termekek)
//    {
//        if(!csatlakoztatva)
//        {
//            for(auto a:_termekek)
//            {
//                if(get_aktualis_kapacitas()>=a.get_mennyiseg())
//                {
//                    rakomanyok.insert(pair<string, termek>(a.get_nev(), a));
//                    toltottseg+=a.get_mennyiseg();
//                }
//                else
//                {
//                    return false;//visszatérhetnénk azzal, amit nem tudtunk bepakolni
//                }
//            }
//            return true;
//        }
//        return false;
//    }
//    void lepakol(map<string, int> lepakolando)
//    {
//        if(!csatlakoztatva)
//        {
//            for(auto it=lepakolando.begin();it!=lepakolando.end();++it)
//            {

//                if(rakomanyok.find(it->first)!=rakomanyok.end())
//                {
//                    toltottseg-=rakomanyok.find(it->first)->second.get_mennyiseg();
//                    rakomanyok.erase(rakomanyok.find(it->first));
//                }
//            }
//        }
//    }
    void felcsatol(string _vonat)
    {
        hely=pair<bool, string>(1, _vonat);
    }
    void lecsatol(string _allomas)
    {
        hely=pair<bool, string>(0, _allomas);
    }
    bool operator<(const kocsi& a)const
    {
        if(nev<a.nev)
            return true;
        return false;
    }
};

ostream& operator<<(ostream& out, kocsi k)
{
    out<<"Kocsi_nev: "<<k.get_nev();
    k.csatlakoztatva_e() ? out<<", csatlakoztatva" : out<<", nincs csatlakoztatva";
    out<<endl;
    return out;
}

struct allomas
{
    string nev;
//    map<string, termek> termekek;
    set<kocsi> kocsik;
    set<string> vonatok;
public:
    allomas(string _nev)
    {
        nev=_nev;
    }
    string get_name()
        {return nev;}
    bool operator<(const allomas& a)const
    {
        if(nev<a.nev)
            return true;
        return false;
    }
//    void termek_lerak(termek _termek)//nevek kitalálása
//    {
//        termekek.insert(pair<string, termek>(_termek.get_nev(), _termek));
//    }
    void kocsi_hozzaad(kocsi _kocsi)
    {
        kocsik.insert(_kocsi);
    }
//    void termek_felpakol(termek _termek)
//    {
//        termekek.find(_termek.get_nev())->second.mennyiseg_modosit(termekek.find(_termek.get_nev())->second.get_mennyiseg()+_termek.get_mennyiseg());//itt tartok
//    }
    void vonat_beerkezik(string _vonat)
    {
        vonatok.insert(_vonat);
    }
    void vonat_elmegy(string _vonat)
    {
        vonatok.erase(vonatok.find(_vonat));
    }
};

ostream& operator<<(ostream& out, allomas a)
{
    out<<"Allomas nev: "<<a.get_name()<<endl;
    if(a.vonatok.size()>0)
    {
        out<<"\t Vonatok az allomason: ";
        for(auto b:a.vonatok)
        {
            out<<b<<"  ";
        }
        out<<endl;
    }
    if(a.kocsik.size()>0)
    {
        out<<"\t Kocsik az allomason: ";
        for(auto b:a.kocsik)
        {
            out<<b.get_nev()<<"  ";
        }
        out<<endl;
    }
//    if(a.termekek.size()>0)
//    {
//        out<<"\t Termekek az allomason: ";
//        for(auto b:a.termekek)
//        {
//            out<<b.first<<"  ";
//        }
//        out<<endl;
//    }
    return out;
}

class vonat
{
    string nev;
    set<string> kocsik;
    int kocsi_kapacitas;
    map<int, string> menetrend;
    string aktualis_allomas;
//    map<string, map<string, int>> terkep;
public:
    vonat(){}
    vonat(string _nev, int _kocsi_kapacitas)
    {
        nev=_nev;
        kocsi_kapacitas=_kocsi_kapacitas;
    }
    bool menetrend_hozzaadas(vector<int>& idopontok, vector<string>& allomasok)
    {
        if(idopontok.size()==allomasok.size())
        {
            menetrend.clear();
            for(size_t i=0;i<idopontok.size();i++)
            {
                menetrend.insert(pair<int, string>(idopontok[i], allomasok[i]));
            }
            return true;
        }
        return false;
    }
    string get_nev()
    {return nev;}
    set<string> get_kocsik()
    {return kocsik;}
    int get_kapacitas()
    {return kocsi_kapacitas;}
    map<int, string> get_menetrend()
    {return menetrend;}
    string get_aktualis_allomas()
    {return aktualis_allomas;}
    void kocsi_felcsatol(string kocsi)
    {
        kocsik.insert(kocsi);
    }
    void kocsi_lecsatol(string kocsi)
    {
        kocsik.erase(kocsik.find(kocsi));
    }
    void mozgat(int ido)
    {
        if(menetrend.find(ido)!=menetrend.end())
        {
            aktualis_allomas=menetrend.find(ido)->second;
        }
    }
};

map<kocsi, string> kocsik_olvas(ifstream& f)
{
    map<kocsi, string> m;
    kocsi k;
    string a;
    int ka;
    f>>a;
    while(a!=";")
    {
        f>>ka;
        k=kocsi(a, ka);
        f>>a;
        m[k]=a;
        f>>a;
    }
    return m;
}

map<string, vonat> vonatok_olvas(ifstream& f, map<string, set<string>>& global)
{
    map<string, vonat> m;
    vector<string> allomasok;
    vector<int> idopontok;
    string n;
    string s;
    int k;
    int i;
    int db;
    f>>n;
    while(n!=";")
    {
        f>>db;
        f>>k;
        for(int j=0;j<db;j++)
        {
            f>>s;
            f>>i;
            allomasok.push_back(s);
            idopontok.push_back(i);
        }
        vonat v(n, k);
        v.menetrend_hozzaadas(idopontok, allomasok);
        for(size_t j=0;j<allomasok.size();j++)
        {
            if(global.find(allomasok[j])==global.end())
            {
                global[allomasok[j]];
            }
            if(j<allomasok.size()-1)
            {
               global[allomasok[j]].insert(allomasok[j+1]);
            }
        }
        allomasok.clear();
        idopontok.clear();
        m[n]=v;
        f>>n;
    }
    return m;
}

vector<termek> termekek_olvas(ifstream& f)
{
    vector<termek> v;
    string n, fr, t;
    int q;
    f>>n;
    while(n!=";")
    {
        f>>fr>>t>>q;
        v.push_back(termek(n, fr, t, q));
        f>>n;
    }
    return v;
}

class vasuti_halozat_allapot
{
    map<string, vonat> vonatok;
    map<string, allomas> allomasok;
    int ido;//meggondolandó
public:
    vasuti_halozat_allapot(map<string, vonat> _vonatok, map<string, allomas> _allomasok)
    {
        vonatok=_vonatok;
        allomasok=_allomasok;
        ido=-1;//meggondolando
    }
    vasuti_halozat_allapot(string file="teszt.txt")
    {
        map<string, set<string>> global_terkep;
        map<string, map<int, string>> menetrendek;
        map<string, int> vonatok_kocsikapacitasa;
        ifstream f(file);
        map<kocsi, string> _kocsik;
        vector<termek> _termekek;
        if(f.fail())
        {
            cerr<<"File olvasasi hiba!"<<endl;
        }
        else
        {
            vonatok=vonatok_olvas(f, global_terkep);
            _kocsik=kocsik_olvas(f);
            _termekek=termekek_olvas(f);
            f.close();
            for(auto a:global_terkep)
            {
                cout<<a.first<<endl;
                for(auto b:a.second)
                {
                    cout<<"\t"<<b<<" ";
                }
                cout<<endl;
            }
        }
    }
    set<vasuti_halozat_allapot> gyerekek_general()
    {
        set<vasuti_halozat_allapot> s;
        //...
        return s;
    }
    bool operator<(vasuti_halozat_allapot a)
    {

    }
};

double heur(vasuti_halozat_allapot& node)
{
    //feltételek
    return 1;
}

struct vasuti_halozat_graf
{
    priority_queue<vasuti_halozat_allapot, vector<vasuti_halozat_allapot>, std::function<bool(vasuti_halozat_allapot, vasuti_halozat_allapot)>> q;
//    auto cmp=([](vasuti_halozat_allapot left, vasuti_halozat_allapot right){return heur(left)<heur(right);});
};

class database
{
    map<string, set<string>> terkep;
    map<string, map<int, string>> menetrendek;
    map<string, int> vonatok_kocsikapacitasa;
    map<string, int> kocsik_arukapacitasa;
    vasuti_halozat_allapot cel;

};

int main()
{
    vasuti_halozat_allapot k;
    termek t("cucc1", "Budapest", "Hatvan", 10);
    kocsi k1("a_kocsi", 10);
    allomas a("Budapest");
    a.kocsi_hozzaad(k1);
//    k1.felpakol({t});
    cout<<k1<<a;
    return 0;
}
