#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <dir.h>
#include <functional>
#include <list>
#include <algorithm>

using namespace std;

class database;

class kocsi
{
    string nev;
    int toltottseg;
    map<string, int> rakomany;
    pair<bool, string> hely;//false, ha allomason van, true, ha vonaton
public:
    kocsi(){}
    kocsi(string _nev, string _kiindulo_pont)
    {
        nev=_nev;
        toltottseg=0;
        hely.first=false;
        hely.second=_kiindulo_pont;
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

class allomas
{
    string nev;
    map<string, int> termekek;
public:
    allomas(){};
    allomas(string _nev)
    {
        nev=_nev;
    }
    string get_name()
    {return nev;}
    map<string, int> get_termekek()
    {return termekek;}
    bool operator<(const allomas& a)const
    {
        if(nev<a.nev)
            return true;
        return false;
    }
    void termek_lerak(string termek_neve, int termek_mennyisege)
    {
        termekek[termek_neve]=termek_mennyisege;
    }
    void termek_kivesz(string termek_neve, int termek_mennyisege)
    {
        termekek[termek_neve]-=termek_mennyisege;
    }
//    void termek_lerak(termek _termek)//nevek kitalálása
//    {
//        termekek.insert(pair<string, termek>(_termek.get_nev(), _termek));
//    }
//    void termek_felpakol(termek _termek)
//    {
//        termekek.find(_termek.get_nev())->second.mennyiseg_modosit(termekek.find(_termek.get_nev())->second.get_mennyiseg()+_termek.get_mennyiseg());//itt tartok
//    }
};

map<string, pair<kocsi, int>> kocsik_olvas(ifstream& f)
{
    map<string, pair<kocsi, int>> m;
    pair<kocsi, int> p;
    kocsi k;
    string n;
    string h;
    int ka;
    f>>n;
    while(n!=";")
    {
        f>>ka;
        f>>h;
        k=kocsi(n, h);
        p.first=k;
        p.second=ka;
        m.insert(pair<string, pair<kocsi, int>>(n, pair<kocsi, int>(k, ka)));
        f>>n;
    }
    return m;
}

map<string, pair<int, map<int, string>>> vonatok_olvas(ifstream& f, map<string, set<string>>& global)
{
    map<string, pair<int, map<int, string>>> m;
    pair<string, pair<int, map<int, string>>> p1;
    pair<int, map<int, string>> p2;
    map<int, string> menet;
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
        p1.first=n;
        p2.first=k;
        for(int j=0;j<db;j++)
        {
            f>>s;
            f>>i;
            p2.second.insert(pair<int, string>(i, s));
            allomasok.push_back(s);
            idopontok.push_back(i);
        }
        p1.second=p2;
        m.insert(p1);
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
        f>>n;
    }
    return m;
}

vector<pair<vector<string>, int>> termekek_olvas(ifstream& f)
{
    vector<pair<vector<string>, int>> v;
    string n, fr, t;
    pair<vector<string>, int> p;
    vector<string> s;
    int q;
    f>>n;
    while(n!=";")
    {
        f>>fr>>t>>q;
        s.push_back(n);//első helyen név
        s.push_back(fr);//2. honnan
        s.push_back(t);//3. hova
        p.first=s;
        p.second=q;
        v.push_back(p);
        s.clear();
        f>>n;
    }
//    for(int i=0;i<v.size();i++)
//    {
//        for(int j=0;j<v[i].first.size();j++)
//        {
//            cout<<v[i].first[j]<<" ";
//        }
//        cout<<"\t"<<v[i].second<<endl;
//    }
    return v;
}

bool from_to_check(string from, string to, map<string, set<string>>& terkep)
{
    if(terkep.find(from)==terkep.end())
        return false;
    list<vector<string>> lista;
    lista.push_back({from});
    bool stop=false;
    while(!stop)
    {
        if(lista.size()==0)
        {
            return false;
        }
        vector<string> aktualis_utvonal=*lista.begin();
        lista.erase(lista.begin());
        string aktualis_allomas=aktualis_utvonal[aktualis_utvonal.size()-1];
        map<string, set<string>>::iterator it=terkep.find(aktualis_allomas);
        if(it!=terkep.end() && it->second.size()>0)
        {
            for(string s:it->second)
            {
                if(find(aktualis_utvonal.begin(), aktualis_utvonal.end(), s)==aktualis_utvonal.end())
                {
                    auto v=aktualis_utvonal;
                    v.push_back(s);
                    lista.push_back(v);
                    if(s==to)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void termekvalogatas(vector<pair<vector<string>, int>>& termekek, map<string, set<string>>& terkep)
{
    vector<vector<pair<vector<string>, int>>::iterator> torlendo;
    for(auto it=termekek.begin();it!=termekek.end();++it)
    {
        if(!from_to_check(it->first[1], it->first[2], terkep))
        {
            cout<<"Informacio: "<<it->first[1]<<" helyrol, "<<it->first[2]<<" helyre nem lehet eljuttatni "<<it->first[0]<<" termeket."<<endl;
            torlendo.push_back(it);
        }
    }
    for(auto it:torlendo)
    {
        termekek.erase(it);
    }
//    for(int i=0;i<termekek.size();i++)
//    {
//        for(int j=0;j<termekek[i].first.size();j++)
//        {
//            cout<<termekek[i].first[j]<<" ";
//        }
//        cout<<"\t"<<termekek[i].second<<endl;
//    }
}

class database
{
    map<string, set<string>> terkep;
    map<string, map<int, string>> menetrendek;
    map<string, int> vonatok_kocsikapacitasa;
    map<string, int> kocsik_arukapacitasa;
    map<string, allomas> cel;
public:
    database(){};
    database(map<string, set<string>> _terkep, map<string, map<int, string>> _menetrendek, map<string, int> _vonatok_kocsikapacitasa, map<string, int> _kocsik_arukapacitasa, map<string, allomas> _cel)
    {
        terkep=_terkep;
        menetrendek=_menetrendek;
        vonatok_kocsikapacitasa=_vonatok_kocsikapacitasa;
        kocsik_arukapacitasa=_kocsik_arukapacitasa;
        cel=_cel;
    }
};

ostream& operator<<(ostream& out, map<string, set<string>>& t)
{
    for(auto a:t)
    {
        out<<a.first<<endl;
        for(auto b:a.second)
        {
            out<<"\t"<<b<<" ";
        }
        out<<endl;
    }
    return out;
}

ostream& operator<<(ostream& out, map<string, allomas>& a)
{
    for(auto p:a)
    {
        out<<p.first<<":"<<endl;
        out<<"\t"<<"Nev"<<"\t"<<"Mennyiseg"<<endl;
        for(auto b:p.second.get_termekek())
        {
            out<<"\t"<<b.first<<"\t"<<b.second<<endl;
        }
    }
}

class vasuti_halozat
{
    map<string, allomas> allomasok;
    map<string, int> vonatok_toltottsege;//valszeg ez sem kell
    vector<kocsi> kocsik;
    int ido;//meggondolandó
public:
    vasuti_halozat(map<string, allomas> _allomasok)
    {
        allomasok=_allomasok;
        ido=-1;//meggondolando
    }
    vasuti_halozat(database d=database(), string file="teszt.txt")
    {
        ido=0;
        map<string, set<string>> global_terkep;
        map<string, map<int, string>> menetrendek;
        map<string, int> vonatok_kocsikapacitasa;
        map<string, int> kocsik_arukapacitasa;
        ifstream f(file);
        if(f.fail())
        {
            cerr<<"File olvasasi hiba!"<<endl;
        }
        else
        {
            auto vonatok=vonatok_olvas(f, global_terkep);
            auto _kocsik=kocsik_olvas(f);
            auto _termekek=termekek_olvas(f);
            f.close();
            for(auto p:vonatok)
            {
                vonatok_kocsikapacitasa.insert(pair<string, int>(p.first, p.second.first));
            }
            for(auto p:vonatok)
            {
                menetrendek.insert(pair<string, map<int, string>>(p.first, p.second.second));
            }
            for(auto p:_kocsik)
            {
                kocsik.push_back(p.second.first);
                kocsik_arukapacitasa.insert(pair<string, int>(p.first, p.second.second));
            }
            termekvalogatas(_termekek, global_terkep);
            map<string, allomas> cel;
            for(auto p:global_terkep)
            {
                allomasok.insert(pair<string, allomas>(p.first, allomas(p.first)));
                cel.insert(pair<string, allomas>(p.first, allomas(p.first)));
            }
            for(auto p:_termekek)
            {
                auto it1=cel.find(p.first[2]);
                if(it1!=cel.end())
                {
                    it1->second.termek_lerak(p.first[0], p.second);
                }
                auto it2=allomasok.find(p.first[1]);
                if(it2!=allomasok.end())
                {
                    it2->second.termek_lerak(p.first[0], p.second);
                }
            }
            d=database(global_terkep, menetrendek, vonatok_kocsikapacitasa, kocsik_arukapacitasa, cel);
            cout<<global_terkep;
            cout<<allomasok;
            cout<<cel;
        }
    }
    bool operator<(vasuti_halozat a)
    {

    }
};


double heur(vasuti_halozat& node)
{
    //feltételek
    return 1;
}

struct vasuti_halozat_allapot
{
    vasuti_halozat node;
    vector<vasuti_halozat> elozmenyek;
    database* d;
    vasuti_halozat_allapot(database* _d, vasuti_halozat kezdo)
    {
        node=kezdo;
        d=_d;
    }
    vector<vasuti_halozat_allapot> gyerekek_general()
    {
        vector<vasuti_halozat_allapot> v;

        return v;
    }
};

//priority_queue<vasuti_halozat_allapot, vector<vasuti_halozat_allapot>, std::function<bool(vasuti_halozat_allapot, vasuti_halozat_allapot)>> q;
////    auto cmp=([](vasuti_halozat_allapot left, vasuti_halozat_allapot right){return heur(left)<heur(right);});

int main()
{
    database d;
    vasuti_halozat k(d);
//    kocsi k1("a_kocsi", 10, "Budapest");
//    allomas a("Budapest");
//    a.kocsi_hozzaad(k1);
//    k1.felpakol({t});
//    cout<<k1<<a;
    return 0;
}
