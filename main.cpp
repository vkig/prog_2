#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <dir.h>
#include <functional>
#include <list>
#include <algorithm>

using namespace std;

bool stop=false;

class database;
//lkkt seged
int fv(int a, int b)
{
    if(a==0)
        return b;
    return fv(b%a, a);
}
//lkkt
int lkkt(set<int>& s)
{
    int a, b;
    while(s.size()>1)
    {
        a=*s.begin();
        s.erase(s.begin());
        b=*s.begin();
        s.erase(s.begin());
        s.insert(a*b/fv(a,b));
    }
    return *s.begin();
}

class kocsi
{
    string nev;
    map<string, int> rakomany;
    pair<bool, string> hely;//false, ha allomason van, true, ha vonaton
public:
    kocsi() {}
    kocsi(string _nev, string _kiindulo_pont)
    {
        nev=_nev;
        hely.first=false;
        hely.second=_kiindulo_pont;
    }
    string get_nev()
    {
        return nev;
    }
    int get_toltottseg()
    {
        int toltottseg=0;
        for(auto a:rakomany)
        {
            toltottseg+=a.second;
        }
        return toltottseg;
    }
    string get_hely()
    {
        return hely.second;
    }
//    int get_aktualis_kapacitas()
//    {return (kapacitas-toltottseg);}
    bool csatlakoztatva_e()
    {
        return hely.first;
    }
    map<string, int> get_rakomany()
    {
        return rakomany;
    }
    void lerak(string s,int i)
    {
        rakomany[s]-=i;
        if(rakomany[s]==0)
        {
            rakomany.erase(rakomany.find(s));
        }
    }
    void felvesz(string s, int i)
    {
        if(rakomany.find(s)==rakomany.end())
        {
            rakomany[s]=i;
        }
        else
        {
            rakomany[s]+=i;
        }

    }
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
        if(nev!=a.nev)
        {
            return nev<a.nev;
        }
        else if(rakomany!=a.rakomany)
        {
            return rakomany<a.rakomany;
        }
        else
            return hely<a.hely;
    }
    bool operator==(kocsi a)const
    {
        if(a.nev==nev && a.hely==hely && a.rakomany==rakomany)
            return true;
        return false;
    }
    bool operator!=(kocsi a)const
    {
        if(a.nev!=nev || a.hely!=hely || a.rakomany!=rakomany)
            return true;
        return false;
    }
};

ostream& operator<<(ostream& out, kocsi k)
{
    out<<"Kocsi_nev: "<<k.get_nev();
    k.csatlakoztatva_e() ? out<<", csatlakoztatva" : out<<", nincs csatlakoztatva";
    out<<k.get_hely()<<" ; ";
    for(auto a:k.get_rakomany())
    {
        out<<a.first<<":"<<a.second<<"~~";
    }
    out<<"^^^";
    out<<endl;
    return out;
}

class allomas
{
    string nev;
    map<string, int> termekek;
public:
    allomas() {};
    allomas(string _nev)
    {
        nev=_nev;
    }
    string get_name()
    {
        return nev;
    }
    int get_termek(string s)
    {
        if(termekek.find(s)!=termekek.end())
        {
                    return termekek[s];
        }
        else {
            return 0;
        }
    }
    int termek_mennyiseg()
    {
        int osszeg=0;
        for(auto a:termekek)
        {
            osszeg+=a.second;
        }
        return osszeg;
    }
    map<string, int> get_termekek()
    {
        return termekek;
    }
    void termek_lerak(string termek_neve, int termek_mennyisege)
    {
        if(termekek.find(termek_neve)==termekek.end())
        {
            termekek[termek_neve]=termek_mennyisege;
        }
        else
        {
            termekek[termek_neve]+=termek_mennyisege;
        }
        if(termekek[termek_neve]==0)
        {
            termekek.erase(termekek.find(termek_neve));
        }
    }
    void termek_elvisz(string termek_neve, int termek_mennyisege)
    {
        termekek[termek_neve]-=termek_mennyisege;
        if(termekek[termek_neve]==0)
        {
            termekek.erase(termekek.find(termek_neve));
        }
    }
    bool operator<(allomas a)const
    {
        if(nev!=a.get_name())
        {
            return nev<a.get_name();
        }
        else
        {
            return termekek<a.get_termekek();
        }
    }
    bool operator==(allomas a)const
    {
        if(nev==a.get_name() && termekek==a.get_termekek())
            return true;
        return false;
    }
    bool operator!=(allomas a)const
    {
        if(nev!=a.get_name() || termekek!=a.get_termekek())
            return true;
        return false;
    }
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
        for(int j=0; j<db; j++)
        {
            f>>s;
            f>>i;
            p2.second.insert(pair<int, string>(i, s));
            allomasok.push_back(s);
            idopontok.push_back(i);
        }
        p1.second=p2;
        p2.second.clear();
        m.insert(p1);
        for(size_t j=0; j<allomasok.size(); j++)
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
        s.push_back(n);//elsõ helyen név
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

vector<set<string>> reszosztas(map<string, set<string>> global)
{
    vector<set<string>> reszek;
    size_t i=0;
    while(global.size()!=0)
    {
        reszek.push_back(global.begin()->second);
        reszek[i].insert(global.begin()->first);
        global.erase(global.begin());
        bool mind=0;
        while(!mind)
        {
            set<string>seged=reszek[i];
            for(string r:reszek[i])
            {
                if(global.find(r)!=global.end())
                {
                    seged.insert(global[r].begin(), global[r].end());
                    global.erase(global.find(r));
                }
            }
            reszek[i]=seged;
            mind=true;
            for(auto all: global)
            {
                if(reszek[i].find(all.first)!=reszek[i].end())
                {
                    mind=false;
                }
            }
        }
        i++;
    }
//    cout<<"Reszek:"<<endl;
//    for(auto r: reszek)
//    {
//        for(auto a: r)
//        {
//            cout<<a;
//        }
//        cout<<endl;
//    }
//    cout<<endl;
    return reszek;
}
//folosleges es nem megvalosithato reszek torlese
void kocsivalogatas(vector<set<string>>& resz,map<string, pair<kocsi, int>> &kocsik,
                    map<string, vector<string>>& menetrendek, map<string, int>& vonatkap,map<string, set<string>>& terkep, vector<pair<vector<string>, int>>& termekek)
{
    set<string>kocsihelyek;
    set<string> termekhelyek;
    for(auto a:termekek)
    {
        termekhelyek.insert(a.first[1]);
    }
    for(auto s:kocsik)
    {
        kocsihelyek.insert(s.second.first.get_hely());
    }
    map<string, set<string>> m;
    for(auto p1:terkep)
    {
        for(auto p2:menetrendek)
        {
            if(find(p2.second.begin(), p2.second.end(), p1.first)!=p2.second.end())
            {
                if(m.find(p1.first)==m.end())
                {
                    string k=p2.first;
                    m.insert(pair<string, set<string>>(p1.first, {k}));
                }
                else
                {
                    string k=p2.first;
                    m[p1.first].insert(k);
                }
            }
        }
    }
    vector<set<string>> torlendo_resz;
    for(auto a:resz)
    {
        bool van_kocsi=false;
        bool van_termek=false;
        for(auto b:a)
        {
            if(kocsihelyek.find(b)!=kocsihelyek.end())
                van_kocsi=true;
            if(termekhelyek.find(b)!=termekhelyek.end())
                van_termek=true;
        }
        if(!van_kocsi || !van_termek)
        {
            if(find(torlendo_resz.begin(), torlendo_resz.end(), a)==torlendo_resz.end())
                torlendo_resz.push_back(a);
            for(auto b:a)
            {
                //kocsik torlese
                vector<map<string, pair<kocsi, int>>::iterator> torlendo_kocsik;
                for(auto p:kocsik)
                {
                    if(p.second.first.get_hely()==b)
                    {
                        torlendo_kocsik.push_back(kocsik.find(p.first));
                    }
                }
                for(auto it:torlendo_kocsik)
                {
                    cout<<"A(z) "<<it->first<<" nevu kocsi nem lat el funkciot, ezert toroltuk."<<endl;
                    kocsik.erase(it);
                }
                //terkep torlese
                terkep.erase(terkep.find(b));
                //termekek torlese
                vector<vector<pair<vector<string>, int>>::iterator> torlendo;
                for(auto it=termekek.begin(); it!=termekek.end(); ++it)
                {
                    if(it->first[1]==b)
                    {
                        torlendo.push_back(it);
                    }
                }
                for(auto it:torlendo)
                {
                    cout<<"Informacio: "<<it->first[1]<<" helyrol, "<<it->first[2]<<" helyre nem lehet eljuttatni "<<it->first[0]<<" termeket."<<endl;
                    termekek.erase(it);
                }
                //vonatok torlese (menetrend, vonatkapacitas)
                for(auto i:m[b])
                {
                    if(vonatkap.find(i)!=vonatkap.end())
                    {
                        cout<<"A(z) "<<i<<" nevu vonat nem lat el funkciot, ezert toroltuk."<<endl;
                        vonatkap.erase(vonatkap.find(i));
                        menetrendek.erase(menetrendek.find(i));
                    }
                }
            }
        }
    }
    //reszgraf torlese
    vector<vector<set<string>>::iterator> torlendo_it1;
    for(auto it=resz.begin(); it!=resz.end(); ++it)
    {
        for(auto it2=torlendo_resz.begin(); it2!=torlendo_resz.end(); ++it2)
        {
            if(*it==*it2)
            {
                torlendo_it1.push_back(it);
            }
        }
    }
    for(auto it:torlendo_it1)
    {
        resz.erase(it);
    }
}

void termekvalogatas(vector<pair<vector<string>, int>>& termekek, vector<set<string>> resz)
{

    vector<vector<pair<vector<string>, int>>::iterator> torlendo;
    for(auto it=termekek.begin(); it!=termekek.end(); ++it)
    {
        for(auto i:resz)
        {
            if(i.find(it->first[1])!=i.end())
            {
                if(i.find(it->first[2])==i.end())
                {
                    cout<<"Informacio: "<<it->first[1]<<" helyrol, "<<it->first[2]<<" helyre nem lehet eljuttatni "<<it->first[0]<<" termeket."<<endl;
                    torlendo.push_back(it);
                }
            }
        }

    }
    for(auto it:torlendo)
    {
        termekek.erase(it);
    }
}

ostream& operator<<(ostream& out, map<string, vector<string>>& m)
{
    for(auto p1:m)
    {
        out<<p1.first<<endl;
        for(size_t i=0; i<p1.second.size(); i++)
        {
            out<<"\t"<<i<<" "<<p1.second[i]<<endl;
        }
    }
    return out;
}

class database
{
    map<string, set<string>> terkep;
    map<string, vector<string>> menetrendek;//átgondolandó
    map<string, map<int, vector<string>>> vonatok_adott_allomason;
    map<string, int> vonatok_kocsikapacitasa;
    map<string, int> kocsik_arukapacitasa;
    map<string, allomas> cel;
    map<string, int> lkkt_k;
public:
    database() {};
    database(map<string, set<string>> _terkep, map<string, vector<string>> _menetrendek, map<string, int> _vonatok_kocsikapacitasa, map<string, int> _kocsik_arukapacitasa, map<string, allomas> _cel)
    {
        terkep=_terkep;
        menetrendek=_menetrendek;
        vonatok_kocsikapacitasa=_vonatok_kocsikapacitasa;
        kocsik_arukapacitasa=_kocsik_arukapacitasa;
        cel=_cel;
        map<string, set<int>> m;
        //vonatok_adott_allomason legyartasa
        for(auto p1:terkep)
        {
            for(auto p2:menetrendek)
            {
                if(find(p2.second.begin(), p2.second.end(), p1.first)!=p2.second.end())
                {
                    if(m.find(p1.first)==m.end())
                    {
                        int k=p2.second.size();
                        m.insert(pair<string, set<int>>(p1.first, {k}));
                    }
                    else
                    {
                        int k=p2.second.size();
                        m[p1.first].insert(k);
                    }
                }
            }
        }
        for(auto p:m)
        {
            lkkt_k.insert(pair<string, int>(p.first, lkkt(p.second)));
        }
        for(auto allomasok:lkkt_k)
        {
            for(int i=0; i<allomasok.second; i++)
            {
                for(auto v:menetrendek)
                {
                    if(v.second[i%v.second.size()]==allomasok.first)
                    {
                        if(vonatok_adott_allomason.find(allomasok.first)==vonatok_adott_allomason.end())
                        {
                            vonatok_adott_allomason[allomasok.first].insert(pair<int, vector<string>>(i, {v.first}));
                        }
                        else
                        {
                            vonatok_adott_allomason[v.second[i%v.second.size()]][i].push_back(v.first);
                        }
                    }
                }
            }
        }
        //kiiratas
//        for(auto p1:vonatok_adott_allomason)
//        {
//            cout<<p1.first<<endl;
//            for(auto p2:p1.second)
//            {
//                cout<<"\t"<<p2.first<<"\t";
//                for(auto p3:p2.second)
//                {
//                    cout<<p3<<" ";
//                }
//                cout<<endl;
//            }
//            cout<<endl;
//        }
    }
    string get_vonat_allomas(string vonat, int ido)
    {
        return menetrendek[vonat][ido%menetrendek[vonat].size()];
    }
    map<string,allomas> get_cel()
    {
        return cel;
    }
    int get_vonat_kocsikapacitas(string vonat)
    {
        return vonatok_kocsikapacitasa[vonat];
    }
    map<string, map<int, vector<string>>> get_vonatok_adott_allomason()
    {
        return vonatok_adott_allomason;
    }
    bool cel_e(map<string, allomas>& c)
    {
        if(c==cel)
            return true;
        return false;
    }
    int get_kocsi_arukapacitasa(string kocsi)
    {
        return kocsik_arukapacitasa[kocsi];
    }
    vector<string> get_vonatok_allomason(string allomas, int ido)
    {
        auto it=vonatok_adott_allomason[allomas].find(ido%lkkt_k[allomas]);
        if(it==vonatok_adott_allomason[allomas].end())
            return vector<string>();
        else
            return it->second;
    }
    bool keresztul_megy_e_masik(string allomas)
    {
        for(auto p:vonatok_adott_allomason[allomas])
        {
            if(p.second.size()>1)
            {
                return true;
            }
        }
        return false;
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
        for(auto b:p.second.get_termekek())
        {
            out<<"\t"<<b.first<<"\t"<<b.second<<endl;
        }
    }
    return out;
}

ostream& operator<<(ostream& out, map<string, kocsi>& a)
{
    for(auto p:a)
    {
        out<<p.first<<":"<<endl;
        for(auto b:p.second.get_rakomany())
        {
            out<<"\t"<<b.first<<"\t"<<b.second<<endl;
        }
    }
    return out;
}
class vasuti_halozat
{
    map<string, allomas> allomasok;
    map<string, int> vonatok_toltottsege;//valszeg ez sem kell-hasznaltam(luca)
    map<string, kocsi> kocsik;
public:
    vasuti_halozat(map<string, allomas> _allomasok)
    {
        allomasok=_allomasok;
    }
    map<string, kocsi>& get_kocsik()
    {
        return kocsik;
    }
    map<string, kocsi> get_kocsik()const
    {
        return kocsik;
    }
    map<string, allomas>& get_allomasok()
    {
        return allomasok;
    }
    map<string, allomas> get_allomasok()const
    {
        return allomasok;
    }
    map<string, int>& get_vonatok_toltottsege()
    {
        return vonatok_toltottsege;
    }
    void set_vonatok_toltottsege(string vonat, int valtozas)
    {
        vonatok_toltottsege[vonat]+=valtozas;
    }
    void set_kocsik( map<string,kocsi>m)
    {
        kocsik=m;
    }
    void set_allomasok_toltottsege(string all, string term, int i)
    {

        allomasok[all].termek_lerak(term, i);

    }
    vasuti_halozat() {};
    vasuti_halozat(database &d, string file="teszt.txt")
    {
        map<string, set<string>> global_terkep;
        vector<set<string>>reszgrafok;
        map<string, vector<string>> menetrendek;
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
            auto reszgrafok=reszosztas(global_terkep);
            for(auto p:vonatok)
            {
                vonatok_toltottsege.insert(pair<string, int>(p.first, 0));
                vonatok_kocsikapacitasa.insert(pair<string, int>(p.first, p.second.first));
                auto vend=p.second.second.end();
                --vend;
                int menetrendeksize=vend->first;
                for(int i=0; i<=menetrendeksize; i++)
                {
                    if(p.second.second.find(i)==p.second.second.end())
                    {
                        menetrendek[p.first].push_back("NIS");
                    }
                    else
                    {
                        menetrendek[p.first].push_back(p.second.second.find(i)->second);
                    }
                }
            }
            cout<<"MENETRENDEK:"<<endl<<menetrendek;
            kocsivalogatas(reszgrafok,_kocsik,menetrendek,vonatok_kocsikapacitasa,global_terkep, _termekek);
            termekvalogatas(_termekek, reszgrafok);
            kocsivalogatas(reszgrafok,_kocsik,menetrendek,vonatok_kocsikapacitasa,global_terkep, _termekek);
            for(auto p:_kocsik)
            {
                kocsik.insert(pair<string, kocsi>(p.second.first.get_nev(),p.second.first));
                kocsik_arukapacitasa.insert(pair<string, int>(p.first, p.second.second));
            }
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
            //cout<<global_terkep;
            cout<<"Allomasok:"<<endl<<allomasok;
            cout<<"CEL:"<<endl<<cel;
        }
    }
    bool operator<(const vasuti_halozat a)const
    {
        if(allomasok!=a.allomasok)
        {
            return allomasok<a.allomasok;
        }
        else if(vonatok_toltottsege!=a.vonatok_toltottsege)
        {
            return vonatok_toltottsege<a.vonatok_toltottsege;
        }
        else
        {
            return kocsik<a.kocsik;
        }
    }
    bool operator!=(const vasuti_halozat a)const
    {
        if(allomasok!=a.allomasok || vonatok_toltottsege!=a.vonatok_toltottsege || kocsik!=a.kocsik)
        {
            return true;
        }
        else
            return false;
    }
    bool operator==(const vasuti_halozat a)const
    {
        for(auto c:allomasok)
        {

            auto e=a.allomasok.find(c.first);
            if(e==a.allomasok.end() || c.second!=e->second)
            {
                return false;
            }
        }
        for(auto b:kocsik)
        {
            auto f=a.kocsik.find(b.first);
            if(f==a.kocsik.end() || b.second!=f->second)
                return false;
        }
        return true;}
};

ostream& operator<<(ostream& out, vasuti_halozat & a)
{
    cout<<endl;
    for(auto p:a.get_allomasok())
    {
        out<<p.first<<":"<<endl;
        for(auto b:p.second.get_termekek())
        {
            out<<"\t"<<b.first<<"\t"<<b.second<<endl;
        }
    }
    for(auto p:a.get_kocsik())
    {
        out<<p.first<<":"<<p.second.get_hely()<<endl;
        for(auto b:p.second.get_rakomany())
        {
            out<<"\t"<<b.first<<"\t"<<b.second<<endl;
        }
    }
    return out;
}

struct vasuti_halozat_allapot
{
    vasuti_halozat node;
    vector<vasuti_halozat> elozmenyek;
    database d;
    vasuti_halozat_allapot(database& _d, vasuti_halozat v)
    {
        node=v;
        d=_d;
    }
    vector<vector<pair<string,vector<pair<char,string>>>>> osszepakol(map<string,set<vector<pair<char,string>>>> lehetseges_akciok)
    {
        vector<vector<pair<string,vector<pair<char,string>>>>>osszes_megoldas;
        //Összesen hány féle kell hogy legyen
        unsigned long long szorzat = 1;
        for(auto i:lehetseges_akciok)
        {
            szorzat *= i.second.size();
        }
        //LEHETSÉGES KOMBINÁCIÓK SZÁMÍTÁSA
        vector<int> hol_tartok(lehetseges_akciok.size(),0);
        while(osszes_megoldas.size() != szorzat)
        {
            vector<pair<string,vector<pair<char,string>>>>megoldas;
            for(unsigned i=0; i<hol_tartok.size(); i++)
            {
                //Map i-edig eleme kell
                //NEM VÉGLEGES
                unsigned j=0;
                set<vector<pair<char,string>>>m;
                string m2;
                for(auto a:lehetseges_akciok)
                {
                    if(j==i)
                    {
                        m=a.second;
                        m2=a.first;
                    }
                    j++;
                }
                //
                vector<pair<char,string>> k;
                k=*next(m.begin(),hol_tartok[i]);
                pair<string,vector<pair<char,string>>> mo(m2,k);
                megoldas.push_back(mo);
            }
            osszes_megoldas.push_back(megoldas);
            for(int i=lehetseges_akciok.size()-1; i!=-1; i--)
            {
                //NEM VÉGLEGES
                int j=0;
                int merete=0;
                for(auto a:lehetseges_akciok)
                {
                    if(j==i)
                    {
                        merete=a.second.size();
                    }
                    j++;
                }
                if(hol_tartok[i]==merete-1)
                {

                    hol_tartok[i]=0;
                }
                else
                {
                    hol_tartok[i]+=1;
                    break;
                }
            }
        }
        //kiiratas osszes megoldas
        cout<<endl;
        for(auto a:osszes_megoldas) //a=1 megoldas
        {
            for(auto b:a)  // b 1pair
            {
                cout<<"kocsi"<<b.first<<" : ";
                for(auto c:b.second) // c 1 pair
                {
                    cout<<c.first<<"."<<c.second;
                }
                cout<<endl;
            }
            cout<<endl<<endl;
        }

        return osszes_megoldas;
    }
    vector<vasuti_halozat_allapot> gyerekek_general()
    {
        cout<<node;
        vector<vasuti_halozat_allapot> v;
        auto kocsik=node.get_kocsik();
        auto allomasok=node.get_allomasok();
        auto toltottseg=node.get_vonatok_toltottsege();
        map<string,set<vector<pair<char,string>>>> lehetseges_akciok;//lecsatol: a, felcsatol: b, lepakol: c, felpakol: d, semmi: "e" , lepakol(celba ert): z
        map<string,vector<pair<char,string>>> konkret_akciok;
        //kocsik lehetseges allapotai
        for(auto k:kocsik)
        {
            lehetseges_akciok[k.first];
            lehetseges_akciok[k.first].insert(vector<pair<char, string>> {pair<char, string>('e', "0")}); //lehet, hogy később kellene berakni
            //VONATON VAN
            if(k.second.csatlakoztatva_e())
            {
                //ÜRES
                if(k.second.get_toltottseg()==0)
                {
                    //JAR ITT MASIK VONAT IS
                    //(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))
                    if(d.keresztul_megy_e_masik(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())))
                    {
                        //lecsatol
                        lehetseges_akciok[k.first].insert(vector<pair<char, string>> {pair<char, string>('a', "0")});
                        //lecsatol-felcsatol
                        if(d.get_vonatok_allomason(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()), elozmenyek.size()).size()>1)
                        {
                            for(auto p:(d.get_vonatok_allomason(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()), elozmenyek.size())))
                            {
                                if(p!=k.second.get_hely())
                                {
                                    lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('b', p)}));
                                }
                            }
                        }
                    }
                    //VAN AZ ALLOMASON TERMEK
                    if(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].get_termekek().size()>0)
                    {
                        //lecsatol-felpakol
                        for(auto a:(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].get_termekek()))
                        {
                            lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('d', a.first)}));
                        }
                    }
                }
                //NEM URES
                else
                {
                    //JAR ITT MASIK VONAT IS
                    if(d.keresztul_megy_e_masik((d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))))
                    {
                        //lecsatol
                        lehetseges_akciok[k.first].insert(vector<pair<char, string>> {pair<char, string>('a', "0")});
                        //lecsatol-felcsatol
                        if(d.get_vonatok_allomason((d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())), elozmenyek.size()).size()>1)
                        {
                            for(auto p:(d.get_vonatok_allomason((d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())), elozmenyek.size())))
                            {
                                if(p!=k.second.get_hely())
                                {
                                    lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('b', p)}));

                                }
                            }
                        }
                        //lecsatol-felpakol
                        if(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].termek_mennyiseg()>0) //a kocsi kapacitás ellenőrzése nem lenne jobb itt?
                        {
                            for(auto a:(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].get_termekek()))
                            {
                                if((d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())>0){
                                    //cout<<"KKKKKKKKKKKKKKKKKKKK"<<(d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())<<endl;
                                lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('d', a.first)}));
                                }
                             }
                        }
                        //lecsatol-lepakol -- további feltételek kellenének, mert alapból a lepakolásnak nem sok értelme, ahhoz több termék kell, vagy kevés cucc nagy kocsiban
                        for(auto i:(k.second.get_rakomany()))
                        {
                            lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('c', i.first)}));
                            //lecsatol-lepakol-felcsatol -- nem engeded meg, hogy visszacsatolhassuk arra, amiről lecsatoltuk
                            if(d.get_vonatok_allomason((d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())), elozmenyek.size()).size()>1)
                            {
                                for(auto p:(d.get_vonatok_allomason((d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())), elozmenyek.size())))
                                {
                                    if(p!=k.second.get_hely())
                                    {
                                        lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('c', i.first), pair<char, string>('b', p)}));
                                    }
                                }
                            }
                            //leccsatol-lepakol-felpakol
                            if(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].termek_mennyiseg()>0)
                            {
                                for(auto b:(allomasok[(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()))].get_termekek()))
                                {
                                    if((i.second+d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())>0){
                                        //cout<<"KKKKKKKKKKKKKKKKKKKK"<<(d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())<<endl;
                                    lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('c', i.first), pair<char, string>('d', b.first)}));
                                    }
                                 }
                            }
                        }


                    }
                    //VAN TERMEK A KOCSIN AMINEK EZ A HELY A CELJA
                    //lecsatol-lepakol
                    //cout<<d.get_cel()[d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())].get_termekek()["cucc1"];
                    map<string, int> nem_ertem=k.second.get_rakomany();
                    for(auto a:(d.get_cel()[d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())].get_termekek()))//celban itt levo termekek
                    {

                        //cout<<"talalt termek:"<<a.first<<((nem_ertem.find(a.first))==nem_ertem.end());
//                        for(auto seged:k.second.get_rakomany())
//                        {
//                            cout<<seged.first<<endl;
//                        }
                        if(nem_ertem.find(a.first)!=nem_ertem.end() && a.second>(allomasok[d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())].get_termekek()[a.first]))
                        {
                           // cout<<"BAAAAAAAAJ--------------------------------"<<endl;
                            lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('z', a.first)}));
                            //+felcsatol
                                for(auto p:(d.get_vonatok_allomason(d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size()), elozmenyek.size())))
                                {
                                        //cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                                        lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('z', a.first), pair<char, string>('b', p)}));
                                }
                            //+felpakol
                            if(allomasok[d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())].termek_mennyiseg()!=0)
                            {
                                for(auto b:(allomasok[d.get_vonat_allomas(k.second.get_hely(),elozmenyek.size())].get_termekek()))
                                {

                                    if((d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())>0){
                                        //cout<<"KKKKKKKKKKKKKKKKKKKK"<<(d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())<<endl;
                                    lehetseges_akciok[k.first].insert(vector<pair<char, string>>({pair<char, string>('a',"0"), pair<char, string>('z', a.first), pair<char, string>('d', b.first)}));
                                    }
                                }
                            }
                        }
                    }


                }
            }
            //ALLOMASON VAN
             else
            {
                //felcsatol
                if(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()).size()>0)
                {
                    for(auto p:(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size())))
                    {
                            lehetseges_akciok[k.first].insert(vector<pair<char, string>> {pair<char, string>('b', p)});
                    }
                }
                //felpakol
                if(allomasok[k.second.get_hely()].termek_mennyiseg()!=0)
                {
                    for(auto a:(allomasok[k.second.get_hely()].get_termekek()))
                    {
                        if((d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())>0){
                            //cout<<"KKKKKKKKKKKKKKKKKKKK"<<(d.get_kocsi_arukapacitasa(k.first)-k.second.get_toltottseg())<<endl;
                        lehetseges_akciok[k.first].insert(vector<pair<char, string>> {pair<char, string>('d', a.first)});
                        }

                    }
                }
            }
        }
        //kiiratas
        for(auto a:lehetseges_akciok)
        {
            cout<<"Kocsi:"<<a.first<<endl;
            for(auto b:a.second)
            {
                for(auto c:b)
                {
                    cout<<c.first<<" : ";
                    cout<<c.second<<"  ";
                    cout<<endl;
                }
            }
            cout<<endl;
        }
        vector<vector<pair<string,vector<pair<char,string>>>>> osszes_kombinacio=osszepakol(lehetseges_akciok);
        //gyerekek eloallitasa
        for(auto a:osszes_kombinacio)
        {
            vasuti_halozat_allapot uj(*this);
            uj.elozmenyek.push_back(this->node);
            map<string,kocsi>m=uj.node.get_kocsik();
            for(auto egy_kocsi:a) //egy_kocsi=pair<string,vector<pair<char,string>>>
            {

                auto k1=m.find(egy_kocsi.first);
                //cout<<egy_kocsi.first;
                for(unsigned i=0; i<egy_kocsi.second.size(); i++)
                {
                    //lecsatol
                    if(egy_kocsi.second[i].first=='a')
                    {
                        uj.node.set_vonatok_toltottsege(k1->second.get_hely(),-1);
                        k1->second.lecsatol(d.get_vonat_allomas(k1->second.get_hely(), elozmenyek.size()));
                        cout<<"lecsatol"<<k1->second;
                    }
                    //felcsatol
                    if(egy_kocsi.second[i].first=='b' && d.get_vonat_kocsikapacitas(egy_kocsi.second[i].second)>(uj.node.get_vonatok_toltottsege()[egy_kocsi.second[i].second]))
                    {
                        k1->second.felcsatol(egy_kocsi.second[i].second);
                        uj.node.set_vonatok_toltottsege(k1->second.get_hely(),1);
                        cout<<"felcsatol"<<k1->second;
                    }
                    //lepakol(celbajovot)
                    if(egy_kocsi.second[i].first=='z')
                    {
                        map<string , int>kocsin_levo_cuccok=k1->second.get_rakomany();
                        for(auto a2:(d.get_cel()[k1->second.get_hely()].get_termekek()))//celban itt levo termekek
                        {
                            if(kocsin_levo_cuccok.find(a2.first)!=kocsin_levo_cuccok.end() && a2.second>(allomasok[k1->second.get_hely()].get_termekek()[a2.first]))
                            {
                                //tobb kell mint ami a kocsin van
                                //stop=true;
                                //cout<<"EZT KERESSUK________________________________________________________: "<<((a2.second-allomasok[k1->second.get_hely()].get_termekek()[a2.first])>(k1->second.get_rakomany().find(a2.first)->second))<<endl;
                                if((a2.second-(allomasok[k1->second.get_hely()].get_termekek()[a2.first]))>(k1->second.get_rakomany().find(a2.first)->second))
                                {
                                    uj.node.set_allomasok_toltottsege(k1->second.get_hely(),a2.first,k1->second.get_rakomany().find(a2.first)->second);
                                    k1->second.lerak(a2.first, k1->second.get_rakomany().find(a2.first)->second);
                                    cout<<"lepakol"<<k1->second;
                                }
                                else
                                {
                                    uj.node.set_allomasok_toltottsege(k1->second.get_hely(),a2.first,(a2.second-(allomasok[k1->second.get_hely()].get_termekek()[a2.first])));
                                    k1->second.lerak(a2.first, (a2.second-(allomasok[k1->second.get_hely()].get_termekek()[a2.first])));
                                    cout<<"lepakol"<<k1->second;
                                }
                            }
                        }
                    }
                    //lepakol-siman
                    if(egy_kocsi.second[i].first=='c')
                    {
                        uj.node.set_allomasok_toltottsege(k1->second.get_hely(),egy_kocsi.second[i].second,((k1->second.get_rakomany().find(egy_kocsi.second[i].second))->second));
                        k1->second.lerak(egy_kocsi.second[i].second, (k1->second.get_rakomany().find(egy_kocsi.second[i].second)->second));
                        cout<<"lepakol"<<k1->second;
                    }
                    //felpakol
                    if(egy_kocsi.second[i].first=='d')
                    {
                        int allomason_termek_mennyisege=uj.node.get_allomasok()[k1->second.get_hely()].get_termek(egy_kocsi.second[i].second);
                        int celban_levo_termek_mennyisege=d.get_cel()[k1->second.get_hely()].get_termek(egy_kocsi.second[i].second);
                        int elviheto_termek=allomason_termek_mennyisege-celban_levo_termek_mennyisege;
                        if(elviheto_termek>0)
                        {
                            //tobb hely van a kocsin szabadon mint amit el tudok vinni
                            if(((d.get_kocsi_arukapacitasa(k1->first))-(k1->second.get_toltottseg()))> elviheto_termek)
                            {

                                k1->second.felvesz(egy_kocsi.second[i].second,elviheto_termek);
                                uj.node.set_allomasok_toltottsege(k1->second.get_hely(),egy_kocsi.second[i].second,-elviheto_termek);

                            }
                            else
                            {
                                int m= ((d.get_kocsi_arukapacitasa(k1->first))-(k1->second.get_toltottseg()));
                                k1->second.felvesz(egy_kocsi.second[i].second,m);
                                uj.node.set_allomasok_toltottsege(k1->second.get_hely(),egy_kocsi.second[i].second,-m);
                            }
                            cout<<"felpakol"<<k1->second;
                         }

                    }

                    //semmi
                    if(egy_kocsi.second[i].first=='e')
                    {
                        cout<<"semmi"<<endl;
                    }
                }
                m[k1->first]=k1->second;
            }
            uj.node.set_kocsik(m);
            v.push_back(uj);
            cout<<uj.node;
            for( auto ell:uj.node.get_kocsik())
            {
                for(auto ell2:ell.second.get_rakomany())
                {
                    if(ell2.second<0)
                        stop=true;
                }
            }
        }
        cout<<v.size()<<endl;
        return v;
    }
};

double heur1(const vasuti_halozat_allapot& a, database& d)
{
    //feltételek
    auto aktualis = a.node.get_allomasok();
    auto cel=d.get_cel();
    int value=1;
    for(auto p:cel)
    {
        for(auto t:p.second.get_termekek())
        {
//            cout<<"HEUR: "<<t.second-aktualis[p.first].get_termekek()[t.first]<<endl;
            value+=t.second-aktualis[p.first].get_termekek()[t.first];
        }
    }
    value+=a.elozmenyek.size()*2;
    return (1000/value);
}

double heur2(const vasuti_halozat_allapot&a, database& d)
{
    auto kocsik = a.node.get_kocsik();
    int value=0;
    for(auto a:kocsik)
    {
        value+=a.second.get_toltottseg();
    }
    return value;
}

template<typename T>
class NodeSet
{
    public:
    set<vasuti_halozat_allapot> t;
    database d;
public:
    NodeSet(database& _d)
    {
        d=_d;
    }
    vasuti_halozat_allapot pop_max()
    {
        auto it_max = t.begin();
        for (auto it=t.begin(); it!=t.end(); it++)
        {
            if (heur1(*it, d) != heur1(*it_max, d))
            {
                if(heur1(*it, d) > heur1(*it_max, d))
                {
                    it_max = it;
                }
            }
            else
            {
                if(heur2(*it, d)>heur2(*it_max, d))
                {
                    it_max=it;
                }
            }
        }
        vasuti_halozat_allapot res = *it_max;
        t.erase(it_max);
        return res;
    }
    void put(vasuti_halozat_allapot a)
    {
        t.insert(a);
    }
    int size() const
    {
        return t.size();
    }
//    void dump(ostream &out = cout) {
//        for (auto it=t.begin();it!=t.end();it++) {
//            out << *it;
//        }
//    }
};

bool operator<(vasuti_halozat_allapot a, vasuti_halozat_allapot b)
{
/*    if(heur(a, a.d)!=heur(b, b.d))
    {
        return heur(a, a.d)<heur(b, b.d);
    }
    else */if(a.elozmenyek.size()!=b.elozmenyek.size())
        return a.elozmenyek.size()<b.elozmenyek.size();
    else if(a.node!=b.node)
    {
        return a.node<b.node;
    }
    else
        return a.elozmenyek<b.elozmenyek;
}
bool operator==(vasuti_halozat_allapot a, vasuti_halozat_allapot b)
{
    return a.node==b.node;
}



//priority_queue<vasuti_halozat_allapot, vector<vasuti_halozat_allapot>, std::function<bool(vasuti_halozat_allapot, vasuti_halozat_allapot)>> q;
////    auto cmp=([](vasuti_halozat_allapot left, vasuti_halozat_allapot right){return heur(left)<heur(right);});
int main()
{
    database d;
    vasuti_halozat k(d,"teszt5.txt");
    vasuti_halozat_allapot a(d, k);
//    cout<<"LEGELSO"<<endl;
//    cout<<a.elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<endl;
//    auto gyerekek=a.gyerekek_general();
//    cout<<"MASODIK"<<endl;
//    cout<<gyerekek[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<endl;
//    auto gyerekek1=gyerekek[1].gyerekek_general();
//    cout<<"HARMADIK"<<endl;
//    cout<<gyerekek1[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek1[0],d)<<endl;
//    auto gyerekek2=gyerekek1[0].gyerekek_general();
//    cout<<"NEGYEDIK"<<endl;
//    cout<<gyerekek2[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek2[0],d)<<endl;
//    auto gyerekek3=gyerekek2[0].gyerekek_general();
//    cout<<"OTODIK"<<endl;
//    cout<<gyerekek3[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek3[0],d)<<endl;
//    auto gyerekek4=gyerekek3[0].gyerekek_general();
//    cout<<"HATODIK"<<endl;
//    cout<<gyerekek4[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek4[1],d)<<endl;
//    auto gyerekek5=gyerekek4[1].gyerekek_general();
//    cout<<"HETEDIK"<<endl;
//    cout<<gyerekek5[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek5[0],d)<<endl;
//    auto gyerekek6=gyerekek5[0].gyerekek_general();
//    cout<<"HETEDIK"<<endl;
//    cout<<gyerekek6[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek6[1],d)<<endl;
//    auto gyerekek7=gyerekek6[1].gyerekek_general();
//    cout<<"NYOLCADIK"<<endl;
//    cout<<gyerekek7[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek7[0],d)<<endl;
//    auto gyerekek8=gyerekek7[0].gyerekek_general();
//    cout<<"KILENCEDIK"<<endl;
//    cout<<gyerekek8[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek8[0],d)<<endl;
//    auto gyerekek9=gyerekek8[0].gyerekek_general();
//    cout<<"TIZEDIK"<<endl;
//    cout<<gyerekek9[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek9[1],d)<<endl;
//    auto gyerekek10=gyerekek9[1].gyerekek_general();
//    cout<<"TIZENEGYEDIK"<<endl;
//    cout<<gyerekek10[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek10[1],d)<<endl;
//    auto gyerekek11=gyerekek10[1].gyerekek_general();
//    cout<<"TIZENKETTEDIK"<<endl;
//    cout<<gyerekek11[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek11[0],d)<<endl;
//    auto gyerekek12=gyerekek11[0].gyerekek_general();
//    cout<<"TIZENHARMADIK"<<endl;
//    cout<<gyerekek12[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek12[0],d)<<endl;
//    auto gyerekek13=gyerekek12[0].gyerekek_general();
//    cout<<"TIZENNEGYEDIK"<<endl;
//    cout<<gyerekek13[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek13[0],d)<<endl;
//    auto gyerekek14=gyerekek13[0].gyerekek_general();
//    cout<<"TIZENÖTÖDIK"<<endl;
//    cout<<gyerekek14[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek14[0],d)<<endl;
//    auto gyerekek15=gyerekek14[0].gyerekek_general();
//    cout<<"TIZENHATODIK"<<endl;
//    cout<<gyerekek15[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek15[0],d)<<endl;
//    auto gyerekek16=gyerekek15[0].gyerekek_general();
//    cout<<"TIZENHETEDIK"<<endl;
//    cout<<gyerekek16[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek16[0],d)<<endl;
//    auto gyerekek17=gyerekek16[0].gyerekek_general();
//    cout<<"TIZENNYOLCADIK"<<endl;
//    cout<<gyerekek17[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek17[1],d)<<endl;
//    auto gyerekek18=gyerekek17[1].gyerekek_general();
//    cout<<"TIZENKILENCEDIK"<<endl;
//    cout<<gyerekek18[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek18[0],d)<<endl;
//    auto gyerekek19=gyerekek18[0].gyerekek_general();
//    cout<<"HUSZADIK"<<endl;
//    cout<<gyerekek19[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek19[0],d)<<endl;
//    auto gyerekek20=gyerekek19[0].gyerekek_general();
//    cout<<"HUSZONEGY"<<endl;
//    cout<<gyerekek20[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek20[0],d)<<endl;
//    auto gyerekek21=gyerekek20[0].gyerekek_general();
//    cout<<"HUSZONKETTO"<<endl;
//    cout<<gyerekek21[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek21[0],d)<<endl;
//    auto gyerekek22=gyerekek21[0].gyerekek_general();
//    cout<<"HUSZONHAROM"<<endl;
//    cout<<gyerekek22[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek22[0],d)<<endl;
//    auto gyerekek23=gyerekek22[0].gyerekek_general();
//    cout<<"HUSZONNEGY"<<endl;
//    cout<<gyerekek23[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek23[0],d)<<endl;
//    auto gyerekek24=gyerekek23[0].gyerekek_general();
//    cout<<"HUSZONÖT"<<endl;
//    cout<<gyerekek24[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek24[0],d)<<endl;
//    auto gyerekek25=gyerekek24[0].gyerekek_general();
//    cout<<"HUSZONHAT"<<endl;
//    cout<<gyerekek25[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek25[1],d)<<endl;
//    auto gyerekek26=gyerekek25[1].gyerekek_general();
//    cout<<"HUSZONHET"<<endl;
//    cout<<gyerekek26[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek26[1],d)<<endl;
//    auto gyerekek27=gyerekek26[1].gyerekek_general();
//    cout<<"HUSZONNYOLC"<<endl;
//    cout<<gyerekek27[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek27[0],d)<<endl;
//    auto gyerekek28=gyerekek27[0].gyerekek_general();
//    cout<<"HUSZONKILENC"<<endl;
//    cout<<gyerekek28[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek28[0],d)<<endl;
//    auto gyerekek29=gyerekek28[0].gyerekek_general();
//    cout<<"HARMINC"<<endl;
//    cout<<gyerekek29[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek29[0],d)<<endl;
//    auto gyerekek30=gyerekek29[0].gyerekek_general();
//    cout<<"HARMINCEGY"<<endl;
//    cout<<gyerekek30[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek30[0],d)<<endl;
//    auto gyerekek31=gyerekek30[0].gyerekek_general();
//    cout<<"HARMINCKETTO"<<endl;
//    cout<<gyerekek31[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek31[0],d)<<endl;
//    auto gyerekek32=gyerekek31[0].gyerekek_general();
//    cout<<"HARMINCHAROM"<<endl;
//    cout<<gyerekek32[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek32[0],d)<<endl;
//    auto gyerekek33=gyerekek32[0].gyerekek_general();
//    cout<<"HARMINCNEGY"<<endl;
//    cout<<gyerekek33[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek33[1],d)<<endl;
//    auto gyerekek34=gyerekek33[1].gyerekek_general();
//    cout<<"HARMINCOT"<<endl;
//    cout<<gyerekek34[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek34[1],d)<<endl;
//    auto gyerekek35=gyerekek34[1].gyerekek_general();
//    cout<<"HARMINCHAT"<<endl;
//    cout<<gyerekek35[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek35[1],d)<<endl;
//    auto gyerekek36=gyerekek35[1].gyerekek_general();
//    cout<<"HARMINCHET"<<endl;
//    cout<<gyerekek36[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek36[0],d)<<endl;
//    auto gyerekek37=gyerekek36[0].gyerekek_general();
//    cout<<"HARMINCNYOLC"<<endl;
//    cout<<gyerekek37[2].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek37[2],d)<<endl;
//    auto gyerekek38=gyerekek37[2].gyerekek_general();
//    cout<<"HARMINCKILENC"<<endl;
//    cout<<gyerekek38[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek38[1],d)<<endl;
//    auto gyerekek39=gyerekek38[1].gyerekek_general();
//    cout<<"NEGYVEN"<<endl;
//    cout<<gyerekek39[1].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek39[1],d)<<endl;
//    auto gyerekek40=gyerekek39[1].gyerekek_general();
//    cout<<"NEGYVENEGY"<<endl;
//    cout<<gyerekek40[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek40[0],d)<<endl;
//    auto gyerekek41=gyerekek40[0].gyerekek_general();
//    cout<<"NEGYVENKETTO"<<endl;
//    cout<<gyerekek41[0].elozmenyek.size()<<" ENNYI AZ IDO EBBEN A PILLANATBAN___________"<<heur(gyerekek41[0],d)<<endl;
//    auto gyerekek42=gyerekek41[0].gyerekek_general();
        int i=0;
        NodeSet<vasuti_halozat_allapot> ns(d);
        ns.put(a);
        bool megtalaltuk=false;
        vasuti_halozat_allapot legjobb=a;
        while(!megtalaltuk  && !stop && ns.size()>0)
        {
            cout<<"AZ "<<i<<". KOR:"<<endl;
            vasuti_halozat_allapot aktualis=ns.pop_max();
            if(d.cel_e(aktualis.node.get_allomasok()))
            {
                legjobb=aktualis;
                megtalaltuk=true;
            }
            vector<vasuti_halozat_allapot> gyerekek=aktualis.gyerekek_general();
            for(vasuti_halozat_allapot b:gyerekek)
            {
//                bool kell=true;
//                for(auto c:ns.t)
//                {
//                    if(c==b)
//                    {
//                        kell=false;
//                    }
//                }
//                if(kell)
//                {
                    ns.put(b);
//                }
            }
            i++;
        }
        if(megtalaltuk)
        {
            cout<<"============================================"<<endl;
            cout<<"JUHUUUUU"<<endl;
            int i=-1;
            for(auto a:legjobb.elozmenyek)
            {
                cout<<i<<". lepes"<<endl;
                cout<<a<<endl;
                i++;
            }
            cout<<"CEL:"<<endl;
            cout<<legjobb.node<<endl;
            cout<<(i+1)<<"ido alatt lehet megoldani";
        }
    return 0;
}
