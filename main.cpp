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

class database;

int fv(int a, int b)
{
    if(a==0)
        return b;
    return fv(b%a, a);
}

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
    int megerkezes_ota_valtozott_az_allapota; // 0ha vonaton, 1 ha lepakolt, 2 ha felpakolt
public:
    kocsi(){}
    kocsi(string _nev, string _kiindulo_pont)
    {
        nev=_nev;
        hely.first=false;
        hely.second=_kiindulo_pont;
        megerkezes_ota_valtozott_az_allapota=0;
    }
    string get_nev()
    {return nev;}
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
    {return hely.second;}
//    int get_aktualis_kapacitas()
//    {return (kapacitas-toltottseg);}
    bool csatlakoztatva_e()
    {return hely.first;}
    int get_megerkezes_ota_allapot()
    {
        return megerkezes_ota_valtozott_az_allapota;
    }
    map<string, int> get_rakomany()
    {return rakomany;}
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
           {return nev<a.nev;}
        else if(rakomany!=a.rakomany)
        {return rakomany<a.rakomany;}
        else
            return hely<a.hely;
    }
    bool operator==(kocsi a)const
    {
        if(a.nev==nev && a.hely==hely && a.rakomany==rakomany)
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
    int termek_mennyiseg()
    {
        int osszeg=0;
        for(auto a:termekek)
        {
            osszeg+=a.second;
        }
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
    }
    void termek_elvisz(string termek_neve, int termek_mennyisege)
    {
        termekek[termek_neve]-=termek_mennyisege;
    }
    bool operator<(allomas a)const
    {
        if(nev!=a.get_name())
        {return nev<a.get_name();}
        else
        {return termekek<a.get_termekek();}
    }
    bool operator==(allomas a)const
    {
        if(nev==a.get_name() && termekek==a.get_termekek())
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
        for(int j=0;j<db;j++)
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
                {mind=false;}
            }
        }
        i++;
    }
    cout<<"Reszek:"<<endl;
    for(auto r: reszek)
    {
        for(auto a: r)
        {cout<<a;}
        cout<<endl;
    }
    cout<<endl;
    return reszek;
}

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
                    kocsik.erase(it);
                }
                //terkep torlese
                terkep.erase(terkep.find(b));
                //termekek torlese
                vector<vector<pair<vector<string>, int>>::iterator> torlendo;
                for(auto it=termekek.begin();it!=termekek.end();++it)
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
    for(auto it=resz.begin();it!=resz.end();++it)
    {
        for(auto it2=torlendo_resz.begin();it2!=torlendo_resz.end();++it2)
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
    for(auto it=termekek.begin();it!=termekek.end();++it)
    {
        for(auto i:resz)
        {
            if(i.find(it->first[1])!=i.end()){
                if(i.find(it->first[2])==i.end()){
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
        for(size_t i=0;i<p1.second.size();i++)
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
    database(){};
    database(map<string, set<string>> _terkep, map<string, vector<string>> _menetrendek, map<string, int> _vonatok_kocsikapacitasa, map<string, int> _kocsik_arukapacitasa, map<string, allomas> _cel)
    {
        terkep=_terkep;
        menetrendek=_menetrendek;
        vonatok_kocsikapacitasa=_vonatok_kocsikapacitasa;
        kocsik_arukapacitasa=_kocsik_arukapacitasa;
        cel=_cel;
        map<string, set<int>> m;
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
            for(int i=0;i<allomasok.second;i++)
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
        for(auto p1:vonatok_adott_allomason)
        {
            cout<<p1.first<<endl;
            for(auto p2:p1.second)
            {
                cout<<"\t"<<p2.first<<"\t";
                for(auto p3:p2.second)
                {
                    cout<<p3<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
    string get_vonat_allomas(string vonat, int ido)
    {return menetrendek[vonat][ido%menetrendek[vonat].size()];}
    map<string,allomas> get_cel()
    {
        return cel;
    }
    int get_vonat_kocsikapacitas(string vonat)
    {return vonatok_kocsikapacitasa[vonat];}
    map<string, map<int, vector<string>>> get_vonatok_adott_allomason()
    {return vonatok_adott_allomason;}
    bool cel_e(map<string, allomas>& c)
    {
        if(c==cel)
            return true;
        return false;
    }
    int get_kocsi_arukapacitasa(string kocsi)
    {return kocsik_arukapacitasa[kocsi];}
    vector<string> get_vonatok_allomason(string allomas, int ido)
    {
        auto it=vonatok_adott_allomason[allomas].find(ido);
        if(it==vonatok_adott_allomason[allomas].end())
            return vector<string>({""});
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

class vasuti_halozat
{
    map<string, allomas> allomasok;
    map<string, int> vonatok_toltottsege;//valszeg ez sem kell
    map<string, kocsi> kocsik;
public:
    vasuti_halozat(map<string, allomas> _allomasok)
    {
        allomasok=_allomasok;
    }
    map<string, kocsi>& get_kocsik()
    {return kocsik;}
    map<string, allomas>& get_allomasok()
    {return allomasok;}
    map<string, int>& get_vonatok_toltottsege()
    {return vonatok_toltottsege;}
    vasuti_halozat(){};
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
                for(int i=0;i<=menetrendeksize;i++)
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
            cout<<menetrendek;
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
            cout<<global_terkep;
            cout<<allomasok;
            cout<<cel;
        }
    }
    bool operator<(const vasuti_halozat a)const
    {
        if(allomasok!=a.allomasok)
        {    return allomasok<a.allomasok;}
        else if(vonatok_toltottsege!=a.vonatok_toltottsege)
        {    return vonatok_toltottsege<a.vonatok_toltottsege;}
        else
        {   return kocsik<a.kocsik;}
    }
    bool operator!=(const vasuti_halozat a)const
    {
        if(allomasok!=a.allomasok && vonatok_toltottsege!=a.vonatok_toltottsege && kocsik!=a.kocsik)
        {    return true;}
        else
            return false;
    }
};

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
    vector<vasuti_halozat_allapot> gyerekek_general()
    {
        vector<vasuti_halozat_allapot> v;
        auto kocsik=node.get_kocsik();
        auto allomasok=node.get_allomasok();
        auto toltottseg=node.get_vonatok_toltottsege();
        map<string,set<vector<pair<char,set<string>>>>> lehetseges_akciok;//lecsatol: a, felcsatol: b, lepakol: c, felpakol: d, semmi: "e"
        map<string,vector<pair<char,string>>> konkret_akciok;
        for(auto k:kocsik)
        {
            lehetseges_akciok[k.first];
            lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>{pair<char, set<string>>('e', {"0"})});
            //VONATON VAN
            if(k.second.csatlakoztatva_e())
            {
                //ÜRES
                if(k.second.get_toltottseg()==0)
                {
                    //JAR ITT MASIK VONAT IS
                    if(d.keresztul_megy_e_masik(k.second.get_hely()))
                    {
                        //lecsatol
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"})}));
                        //lecsatol-felcsatol
                        if(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()).size()>1)
                        {
                            set<string>s;
                            for(auto p:d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()))
                            {
                                if(p!=k.second.get_hely())
                                {
                                    s.insert(p);

                                }
                            }
                             lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}), pair<char, set<string>>('b', s)}));
                        }
                    }
                    //VAN AZ ALLOMASON TERMEK
                    if(allomasok[k.second.get_hely()].get_termekek().size()>0)
                    {
                        //lecsatol
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"})}));
                        //lecsatol-felpakol
                        set<string>s;
                        for(auto a:allomasok[k.second.get_hely()].get_termekek())
                        {
                            s.insert(a.first);

                        }
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}), pair<char, set<string>>('d', s)}));

                    }
                }
                //NEM URES
                else
                {
                    //JAR ITT MASIK VONAT IS
                    if(d.keresztul_megy_e_masik(k.second.get_hely()))
                    {
                        //lecsatol
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"})}));
                        //lecsatol-felcsatol
                        if(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()).size()>1)
                        {
                            set<string>s;
                            for(auto p:d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()))
                            {
                                if(p!=k.second.get_hely())
                                {
                                    s.insert(p);

                                }
                            }
                             lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}), pair<char, set<string>>('b', s)}));
                        }
                        //lecsatol-felpakol
                        if(allomasok[k.second.get_hely()].termek_mennyiseg()!=0)
                        {
                            set<string>s;
                            for(auto a:allomasok[k.second.get_hely()].get_termekek())
                            {
                                s.insert(a.first);

                            }
                            lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}), pair<char, set<string>>('d', s)}));
                        }

                     }
                    //VAN TERMEK A KOCSIN AMINEK EZ A HELY A CELJA
                    //lecsatol-lepakol
                    set<string>s1;
                    bool van_cel=false;
                    for(auto a:d.get_cel()[k.second.get_hely()].get_termekek())//celban itt levo termekek
                    {
                        if(k.second.get_rakomany().find(a.first)!=k.second.get_rakomany().end() && a.second>allomasok[k.second.get_hely()].get_termekek()[a.first])
                        {
                            van_cel=true;
                            s1.insert(a.first);
                        }
                    }
                    if(van_cel){
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}), pair<char, set<string>>('c', s1)}));
                        }
                    //+felcsatol
                    if(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()).size()>1)
                    {
                        set<string>s;
                        for(auto p:d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()))
                        {
                            if(p!=k.second.get_hely())
                            {
                                s.insert(p);

                            }
                        }
                         lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}),pair<char, set<string>>('c', s1), pair<char, set<string>>('b', s)}));
                    }
                    //+felpakol
                    if(allomasok[k.second.get_hely()].termek_mennyiseg()!=0)
                    {
                        set<string>s;
                        for(auto a:allomasok[k.second.get_hely()].get_termekek())
                        {
                            s.insert(a.first);

                        }
                        lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char, set<string>>('a',{"0"}),pair<char, set<string>>('c', s1), pair<char, set<string>>('d', s)}));
                    }


                }
            }
            //ALLOMASON VAN
            if(!k.second.csatlakoztatva_e())
            {

                    //felcsatol
                if(d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()).size()>1)
                {
                    set<string>s;
                    for(auto p:d.get_vonatok_allomason(k.second.get_hely(), elozmenyek.size()))
                    {
                        if(p!=k.second.get_hely())
                        {
                            s.insert(p);

                        }
                    }
                     lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({pair<char,  set<string>>('b', s)}));
                }
                    //felpakol
                if(allomasok[k.second.get_hely()].termek_mennyiseg()!=0)
                {
                    set<string>s;
                    for(auto a:allomasok[k.second.get_hely()].get_termekek())
                    {
                        s.insert(a.first);

                    }
                    lehetseges_akciok[k.first].insert(vector<pair<char, set<string>>>({ pair<char, set<string>>('d', s)}));
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
                    for(auto d:c.second)
                    {
                        cout<<d<<"  ";
                    }
                    cout<<endl;
                }
            }
            cout<<endl;
        }
//        //                        vasuti_halozat_allapot uj(*this);
//        //                        uj.elozmenyek.push_back(this->node);
//        //                        auto k1=find(uj.node.get_kocsik().begin(), uj.node.get_kocsik().end(), k);
//        //                        k1->second.lecsatol(d.get_vonat_allomas(k.second.get_hely(), elozmenyek.size()));
//        //                        v.push_back(uj);
//        vector<vector<vector<pair<char, string>>>> x;
//        vector<string> kocsisorrend;
//        int k=0;
//        for(auto a:lehetseges_akciok)
//        {
//            kocsisorrend.push_back(a.first);
//            for(auto b:a.second)
//            {
//                for(int i=0;i<b.size();i++)
//                {
//                    for(auto d:b[i].second)
//                    {
////                        cout<<"debug "<<b[i].first<<" "<<d<<endl;
//                        if(i>0)
//                        {
//                            for(int j=0;j<x.size();j++)
//                            {
//                                x[j].push_back(pair<char, string>(b[i].first, d));
//                            }
//                        }
//                        else
//                        {
//                            x.push_back(vector<pair<char, string>>({pair<char, string>(b[i].first, d)}));
//                        }
//                        //aktualis.push_back([pair<char, string>(c.first, d)]);
//                        //konkret_akciok.push_back(aktualis);
//                    }
//                }
//            }
//            k++;
//        }
//        for(auto a:x)
//        {
//            cout<<a.size();
//            for(int i=0;i<a.size();i++)
//            {
//                cout<<kocsisorrend[i]<<": "<<a[i].first<<" "<<a[i].second<<endl;
//            }
//        }
        return v;
    }
};

template<typename T>
class NodeSet {
    set<T> t;
public:
    T pop_max() {
        auto it_max = t.begin();
        for (auto it=t.begin();it!=t.end();it++) {
            if (value(*it) > value(*it_max)) {
                it_max = it;
            }
        }
        T res = *it_max;
        t.erase(it_max);
        return res;
    }
    void put(T a) {
    t.insert(a);
    }
    int size() const {
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
    if(a.node!=b.node)
        return a.node<b.node;
    else
        return a.elozmenyek<b.elozmenyek;
}

double heur(vasuti_halozat_allapot& node)
{
    //feltételek
    return 1;
}


//priority_queue<vasuti_halozat_allapot, vector<vasuti_halozat_allapot>, std::function<bool(vasuti_halozat_allapot, vasuti_halozat_allapot)>> q;
////    auto cmp=([](vasuti_halozat_allapot left, vasuti_halozat_allapot right){return heur(left)<heur(right);});

int main()
{
    database d;
    vasuti_halozat k(d);
    vasuti_halozat_allapot a(d, k);
    a.gyerekek_general();
    return 0;
}
