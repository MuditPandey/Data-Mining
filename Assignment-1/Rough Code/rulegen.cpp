#include<bits/stdc++.h>
#define N 435
#define MIN_SUP 0.45
#define MIN_CONF 0.7
#define CHILD 3
#define CAPACITY 3

using namespace std;

vector< map< set<int>,int > > freq_item;
map<int,int> one_item_set;
void load_training_data(char*);
vector< set<int> > transactions;
map< set<int>,int > apriori_gen(vector< set<int> >);
bool sim_fk_2(set<int>,set<int>);
vector< set<int> > get_vector(map< set<int>,int> a);
void gen_freq_itemset();
map< set<int>,int> get_1_freq();
map< pair< set<int>, set<int> >, int> rules;

vector <set <int> > conv_map_to_vec(map< set<int>,int > m)
{
    vector <set <int> > v;
    map< set<int>, int>::iterator it = m.begin();
    for (; it != m.end(); ++it)
        v.push_back(it->first);
    return v;
}

set <int> setdiff(set <int> a, set <int> b)
{
    set <int>::iterator it;
    for (it = b.begin(); it != b.end(); ++it)
        a.erase(*it);
    return a;
}

int find_suppcount(set <int> itemset)
{
    int k = itemset.size();
    map< set<int>,int >::iterator it;
    for (it = freq_item[k].begin(); it != freq_item[k].end(); ++it)
    {
        if(it->first == itemset)
            return it->second;
    }
    return -1;
}

void ap_genrules(set <int> fk, int Ofk, vector <set <int> > Hm)
{
    int k = freq_item[k].size();
    int m = Hm.size();
    if (k > m + 1)
    {
        map< set<int>,int > Hm1 = apriori_gen(Hm);
        map< set<int>, int>::iterator it = Hm1.begin();
        for (; it != Hm1.end(); ++it)
        {
            set <int> consequent = setdiff(fk, it->first);
            double conf = (double) (Ofk / find_suppcount(consequent));
            if (conf >= MIN_CONF)
            {
                pair< set <int>, set <int> > rule;
                rule.first = consequent;
                rule.second = it->first;
                rules.insert(pair< pair< set<int>, set<int> >, int>(rule, conf));
            }
            else
                Hm1.erase(it);
        }
        vector <set <int> > Hm1_vec = conv_map_to_vec(Hm1);
        ap_genrules(fk, Ofk, Hm1_vec);
    }
}

vector < set <int> > conv_set_to_vector_set(set <int> s)
{
    vector < set <int> > v;
    set<int>::iterator it;
    it = s.begin();
    set<int> p;
    for (; it != s.end(); ++it)
    {
        p.insert(*it);
        v.push_back(p);
        p.clear();
    }
    return v;
}

void rulegen()
{
    map<set<int>, int>::iterator it2;
    set<int> H;
    set<int>::iterator it;
    for (int k = 2; k < freq_item.size(); ++k)
    {
        for (it2 = freq_item[k].begin(); it2 != freq_item[k].end(); ++it2)
        {
            it = (it2->first).begin();
            for (; it != (it2->first).end(); ++it)
                H.insert(*it);
            vector < set <int> > H1 = conv_set_to_vector_set(H);
            ap_genrules(it2->first, it2->second, H1);
        }
    }
}

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
