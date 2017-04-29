#include<bits/stdc++.h>
#include<string>
#include<sstream>
#include<fstream>
#include<limits>
#define BINS 5
#define CONTINOUS_ATTR 8
#define BOOL_ATTR 1
#define MIN_CONF 0.9
#define MIN_SUP 0.2
using namespace std;

//For reading transactions
map<int,vector<float> > transactions;
vector< set<int> > transactions_in;
vector< set<int> > transactions_fn;
vector< pair<int,int> > sort_list;
map<int,int> one_item_set;
map<int,int> mapping_on;
map<int,int> mapping_no;
map< set<int>,int> get_1_freq();
//For creating bins
pair<float,float> max_min[9];
//Mapping
vector<string> mapping_for_items;
//Frequent item sets
vector< map< set<int>,int > > freq_item(100);
map< pair< set<int>, set<int> >, double> rules;
//Misc Functions
string remap(int);
void init_mapping_for_items();
vector< set<int> > get_vector(map< set<int>,int> a);
void get_transaction_data(string file);
//RULE GENERATION RELATED
set<int> setdiff(set <int>, set <int>) ;
vector < set <int> > conv_set_to_vector_set(set <int> );
int find_suppcount(set <int> );
void ap_genrules(set <int> , vector <set <int> >, int inc = 0);
void rulegen();
bool sim_fk_2(set<int> a,set<int> b);
void print_rules(string);
void print_set(set<int>, string);
map< set<int>,int> apriori_gen(vector< set<int> > fk);
//fptree related functions
void load_example();
int total_items = CONTINOUS_ATTR * BINS + BOOL_ATTR * 2;

struct node
{
    int item;
    int item_count;
    map<int,node*> children;
    node *link;
    bool path;
    node(int val = 0, int ct = 0, node *l = NULL)
    {
        item = val;
        item_count = ct;
        link = l;
        path = false;
    }
};

void Print_tree(node *, int x = 0);
node *generate_fptree();
void Print_list();
node *assign_links(int item, node *curr, node *prev = NULL);
stack < map <int, node *> > link_start;
map <int, node *> link_start_orig;
void reset(node *n);
bool mark_path(int item, node *n);
node *make_sub(node *n, int);
node *conditional_fp(node *root, int item);
void remove_unfrequent(node *root);
void remove_items(node *root, int item);
void join_tree(node *root, node *append);
set <int> f_item;
void fp_growth(node *root);
void one_freq(node *root);


int main()
{
    init_mapping_for_items();
    for(int i=1;i<=CONTINOUS_ATTR*BINS+2*BOOL_ATTR;i++)
 	{
     one_item_set.insert(pair<int,int> (i,0));
 	}
    string input_file="pima-indians-diabetes.data";
    string output= "fp_output.txt";
    get_transaction_data(input_file);

    for(map<int,int>::iterator it=one_item_set.begin();it!=one_item_set.end();it++)
    {
     sort_list.push_back(pair<int,int>(it->second,it->first));
     //cout<<"item no.="<<it->first<<" count="<<it->second<<endl;
    }
    sort(sort_list.begin(),sort_list.end());
    one_item_set.clear();
    int k=1;
    cout<<"REINIT"<<endl;
    for(int i=sort_list.size()-1;i>=0;i--)
    {
     one_item_set.insert(pair<int,int>(k,sort_list[i].first));
     //change meaning map here too
     mapping_on.insert(pair<int,int>(sort_list[i].second,k));
     mapping_no.insert(pair<int,int>(k,sort_list[i].second));
     //cout<<"inserted="<<k<<" corresponding to "<<sort_list[i].second<<endl;
     k++;
    }
    //FIX transaction data
    for(int i=0;i<transactions_in.size();i++)
    {
         //cout<<i<<endl;
         set<int> add;
         for(set<int>::iterator it=transactions_in[i].begin();it!=transactions_in[i].end();it++)
         {
            add.insert(mapping_on.find(*it)->second);
         }
         transactions_fn.push_back(add);
         add.clear();
    }
    cout<<"done\n";
    cout<<"Transactions="<<transactions_fn.size()<<endl;
//    for(int i=0;i<transactions_fn.size();i++)
//    {
//         //cout<<i<<endl;
//         for(set<int>::iterator it=transactions_fn[i].begin();it!=transactions_fn[i].end();it++)
//         {
//            remap(*it);
//            cout<<endl;
//         }
//        cout<<"***********"<<endl;
//    }

    node *root = generate_fptree();
    //Print_list();
    //reset(root);
    //Print_tree(root, 0);
    fp_growth(root);
    fstream fout;
    fout.open(output, ios::out);
    fout<<"FREQUENT ITEM-SETS:"<<endl;
    fout.close();
    freq_item[0]=get_1_freq();
    //cout<<freq_item.size();
    for(int i = 0; i < freq_item.size(); i++)
    {
        if(freq_item[i].size()==0)
            break;
        //fout << i+1 << "th item-set:-\n";
//        cout<<"freq item set"<<i << " = "<<freq_item[i].size()<<endl;
        for(map< set<int>,int >::iterator it = freq_item[i].begin(); it != freq_item[i].end(); it++)
        {
            fout.open(output, ios::app);
            fout << endl;
            fout << "Support = " << (float)it->second/transactions.size() << ": ";
            fout.close();
            print_set(it->first, output);
        }
    }
    fout.open(output, ios::app);
    //fout<<"*****\n";
    fout.close();
    rulegen();
    print_rules(output);
    return 0;
}

void get_transaction_data(string file)
{
    fstream fil;
    fil.open(file,ios::in);
    string buf;
    char *pch;
    int tid=0;
    while(getline(fil,buf))
    {
       vector<float> add;

       for(int i=0;i<buf.length();i++)
       {
           char ret[20];
           int k=0;
           while(buf[i]!=',' && i<buf.length())
           {
               ret[k++]=buf[i++];
           }
           ret[k]='\0';
           float inp=atof(ret);
           //string inp(ret);

//           cout<<inp<<" ";
            if(transactions.find(tid)!=transactions.end())
            {
                transactions.find(tid)->second.push_back(inp);
            }
            else
            {
                vector<float> add;
                add.push_back(inp);
                transactions.insert(pair<int,vector<float> >(tid,add));
            }


        }
//        cout<<endl;
        tid++;
    }
    fil.close();
    for(int i=0;i<9;i++)
    {
        max_min[i].first=INT_MIN;
        max_min[i].second=INT_MAX;
    }
    //cout<<endl<<"No. of transactions="<<tid<<" in map="<<transactions.size()<<endl<<endl;;

    for(map<int,vector<float> >::iterator it=transactions.begin();it!=transactions.end();it++)
    {
        //cout<<it->second.size()<<"**"<<endl;
        for(int i=0;i<it->second.size();i++)
            {
                if(it->second[i]>max_min[i].first)
                    max_min[i].first=it->second[i];
                if(it->second[i]<max_min[i].second)
                    max_min[i].second=it->second[i];
                //cout<<it->second[i]<<" ";
            }
        //cout<<endl;
    }
    /* for(int i=0;i<9;i++)
    {
        cout<<"MAX="<<max_min[i].first<<" MIN="<<max_min[i].second<<endl;
    }*/
    for(map<int,vector<float> >::iterator it=transactions.begin();it!=transactions.end();it++)
    {

        set<int> add;
        for(int i=0;i<it->second.size();i++)
        {
            int bin_no=(it->second[i]-max_min[i].second)*BINS/(max_min[i].first-max_min[i].second)+1;
            if(bin_no>BINS)
                	bin_no-=1;
            if(i==it->second.size()-1 && bin_no==BINS)
                	bin_no=2;
            int item_no=i*BINS+bin_no;
            add.insert(item_no);
            one_item_set.find(item_no)->second++;
      	    //cout<<it->second[i]<<" is in bin "<<bin_no<<" for max="<<max_min[i].first<<" min="<<max_min[i].second<<" interval-size="<<float(max_min[i].first-max_min[i].second)/BINS<<" Alloted item no="<<item_no<<endl;
        }
        transactions_in.push_back(add);
        add.clear();
    }


}

string remap(int item_num_tr)
{
    //cout<<"mew="<<item_num_tr<<" old=";
    int old_item=mapping_no.find(item_num_tr)->second;
    //cout<<old_item<< " Bin=";
    int bin_no=(old_item-1)%BINS+1;
    //cout<<bin_no<<" Attribute=";
    int i=(old_item-bin_no)/BINS;
   //cout<<i<<" max="<<max_min[i].first<<" min="<<max_min[i].second<<" ";
    if(i==8 && bin_no==1)
       return mapping_for_items[i];
    else if(i==8 && bin_no==2)
        return mapping_for_items[i+1];
    else
        {
            stringstream TEMP;
            TEMP<<mapping_for_items[i]<<": >="<<max_min[i].second+((max_min[i].first-max_min[i].second)/BINS)*(bin_no-1)<<" and < "<<max_min[i].second+((max_min[i].first-max_min[i].second)/BINS)*bin_no;
            return TEMP.str();
        }
}

void init_mapping_for_items()
{
    mapping_for_items.push_back("Number of times pregnant");
    mapping_for_items.push_back("Plasma glucose concentration a 2 hours in an oral glucose tolerance test");
    mapping_for_items.push_back("Diastolic blood pressure (mm Hg)");
    mapping_for_items.push_back("Triceps skin fold thickness (mm)");
    mapping_for_items.push_back("2-Hour serum insulin (mu U/ml)");
    mapping_for_items.push_back("Body mass index (weight in kg/(height in m)^2)");
    mapping_for_items.push_back("Diabetes pedigree function");
    mapping_for_items.push_back("Age (years)");
    mapping_for_items.push_back("tested negative for diabetes");
    mapping_for_items.push_back("tested positive for diabetes");
}

/*
Rule generation driver function
*/
void rulegen()
{
    //For each item-set with k>=2
    for (int k = 1; k < freq_item.size(); ++k)
    {
        //Traverse map of k-th item-set
        for (map< set<int>, int >::iterator it2 = freq_item[k].begin(); it2 != freq_item[k].end(); ++it2)
        {
           //cout<<"ITEMSET=";
           vector< set<int> > one_item_consequent;
           //Get each set in k-th item set
           for (set<int>::iterator it = (it2->first).begin(); it != (it2->first).end(); ++it)
            {
                //cout<<*it<<" ";
                set<int> add;
                add.insert(*it);
                one_item_consequent.push_back(add);
            }
            /*cout<<"\n1 consequents=";
            for(int i=0;i<one_item_consequent.size();i++)
            {
                for(set<int>::iterator it3=one_item_consequent[i].begin();it3!=one_item_consequent[i].end();it3++)
                {
                    cout<<*it3<<",";
                }
            }
            cout<<endl;*/
            ap_genrules(it2->first,one_item_consequent);
        }
    }
}
/*
 Generates all candidate k item sets from k-1 frequent item set and returns a map with all candidate k item sets.The support count for all sets is set to 0.
  The k-1 frequent item-sets must be passed as vector.
*/
map< set<int>,int> apriori_gen(vector< set<int> > fk)
{
    map< set<int> ,int> candidate;
    for(int i=0;i<fk.size()-1;i++)
    {
        for(int j=i+1;j<fk.size();j++)
        {
          if(sim_fk_2(fk[i],fk[j]))
          {
              set<int> add(fk[i]);
              add.insert(*fk[j].rbegin());
              candidate.insert(pair< set<int>,int>(add,0));
          }
          else
            break;
        }
    }
    return candidate;
}
/*
 Helper function for apriori_gen() which checks whether k-2 items of 2 item-sets are equal.
 Used for handling cases where multiple copies of a candidate item set may be generated.
*/
bool sim_fk_2(set<int> a,set<int> b)
{
    if(a.size()!=b.size())
        return false;
    int count =0;
    for(set<int>::iterator it1=a.begin(),it2=b.begin();it1!=a.end() && it2!=b.end();it1++,it2++)
    {
        if(*it1!=*it2)
            break;
        else
            count++;
    }
    if(count==a.size()-1)
        return true;
    return false;
}
/*
Recursive function that generates all rules
*/
void ap_genrules(set <int> fk,vector <set <int> > Hm, int inc)
{
    int k = fk.size();
    if(Hm.empty())
        return;
    int m = Hm[0].size();
    //cout<<"K="<<k<<" M="<<m<<endl;
    if (k > m + inc)
    {
        vector< set<int> > Hm1;
        if (inc)
            Hm1 = get_vector(apriori_gen(Hm));
        else
            Hm1 = Hm;
        /*for(int i=0;i<Hm1.size();i++)
        {
            for(set<int>::iterator it=Hm1[i].begin();it!=Hm1[i].end();it++)
                cout<<*it<<" ";
            cout<<endl;
        }*/
        for (int i=0;i<Hm1.size();i++)
        {
            set <int> cause = setdiff(fk, Hm1[i]);
            double ofk=(double)freq_item[k-1].find(fk)->second;
            double conf = ofk/find_suppcount(cause);
            float epsilon = 0.0000001;
            if (conf >= MIN_CONF - epsilon)
            {
                //cout<<conf<<"\n";
                pair< set <int>, set <int> > rule;
                rule.first = cause;
                rule.second =Hm1[i];
                rules.insert(pair< pair< set<int>, set<int> >, double>(rule, conf));
            }
            else
                {
                    Hm1.erase(Hm1.begin()+i);
                    i--;
                }
        }
        ap_genrules(fk,Hm1,1);
    }
}
/*
Utility function that converts the key values of map of set<int>,int to a vector of set<int>. Returns the vector.
*/
vector< set<int> > get_vector(map< set<int>,int> a)
{
    vector< set<int> > return_vec;
    for(map<set <int>,int>::iterator it=a.begin();it!=a.end();it++)
    {
        return_vec.push_back(it->first);
    }
    return return_vec;
}
/*
/*
Returns the support count for a frequent item set
*/
int find_suppcount(set <int> itemset)
{
    int k=static_cast<int>(itemset.size())-1;
    if(k<0)
        return -1;
    if(freq_item[k].find(itemset)!=freq_item[k].end())
    {
        return freq_item[k].find(itemset)->second;
    }
    return -1;
}
/*
Computes and returns the set difference b/w two sets
*/
set <int> setdiff(set <int> a, set <int> b)
{
    set <int>::iterator it;
    for (it = b.begin(); it != b.end(); ++it)
       {
           if(a.find(*it)!=a.end())
                a.erase(*it);
       }
    return a;
}
/*
Function to print rules
*/
void print_rules(string file)
{
    fstream fout;
    fout.open(file, ios::app);
    fout<<"\n\nGENERATED RULES:\n";
    fout.close();
    for(map< pair<set<int>,set<int> >,double >::iterator it=rules.begin();it!=rules.end();it++)
    {
        fout.open(file, ios::app);
        fout << endl;
        fout << "Confidence = " << it->second << ": ";
        fout.close();
        print_set(it->first.first, file);
        fout.open(file, ios::app);
        fout << "-> ";
        fout.close();
        print_set(it->first.second, file);
    }
    fout.close();
}
/*
Function to print set
*/
void print_set(set<int> s, string file)
{
    fstream fout;
    fout.open(file, ios::app);
    fout << "{";
    set<int>::iterator it = s.begin();
    fout << remap(*it);
    for(++it; it != s.end();it++)
        fout << "," << remap(*it);
    fout << "}";
    fout.close();
}

void one_freq(node *root)
{
    map <int, node *>::iterator it = link_start.top().begin();
    for (; it != link_start.top().end(); ++it)
    {
        f_item.insert(it->first);
        int supp = 0;
        node *curr = it->second;
        while (curr != NULL)
        {
            supp += curr->item_count;
            curr = curr->link;
        }
//cout << "see" << endl;
        float epsilon = 0.0000001;
        if (supp >= MIN_SUP * transactions.size() - epsilon)
            freq_item[f_item.size() - 1].insert(pair <set <int>, int> (f_item, supp));
//cout << "don't see " << endl;
        f_item.erase(it->first);
    }
}

void fp_growth(node *root)
{
    for (int i = total_items; i >= 1; --i)
    {
        if (link_start.top().find(i) == link_start.top().end())
            continue;
        f_item.insert(i);
        node *sub = conditional_fp(root, i);
//        cout << "made conditional fp tree on " << i << endl;
        map <int, node *>::iterator it = link_start.top().begin();
        for (; it != link_start.top().end(); ++it)
        {
            f_item.insert(it->first);
            int supp = 0;
            node *curr = it->second;
            while (curr != NULL)
            {
                supp += curr->item_count;
                curr = curr->link;
            }
//cout << "see" << endl;
            freq_item[f_item.size() - 1].insert(pair <set <int>, int> (f_item, supp));
//cout << "don't see " << endl;
            f_item.erase(it->first);
        }
        fp_growth(sub);
        link_start.pop();
        f_item.erase(i);
    }
}

node *conditional_fp(node *root, int item)
{
    map <int, node *> new_links;
    link_start.push(new_links);
    mark_path(item, root);
    node *sub = make_sub(root, item);
    map <int, node *>::iterator it = link_start.top().begin();
    for (; it != link_start.top().end(); ++it)
        assign_links(it->first, sub);
    link_start.top().erase(0);
    remove_unfrequent(sub);
    return sub;
}

void remove_unfrequent(node *root)
{
    map <int, node *> curr_links = link_start.top();
//    cout << "no of elements " << link_start_orig.size() << endl;
    for (int i = total_items; i >= 1; --i)
    {

        if (curr_links.find(i) != curr_links.end())
        {
            node* curr = curr_links.find(i)->second;
            int counter = 0;
            while (curr != NULL)
            {
                counter += curr->item_count;
                curr = curr->link;
            }
//            cout << "item = " << i << ", counter = " << counter << ", " << MIN_SUP * transactions_fn.size() << endl;
            float epsilon = 0.0000001;
            if (counter < MIN_SUP * transactions_fn.size() - epsilon)
            {
//                cout << "item " << i << " is unfrequent" << endl;
                link_start.top().erase(i);
                remove_items(root, i);
            }
        }
    }
}

void remove_items(node *root, int del)
{
    map <int, node *>::iterator it = root->children.begin();
    for (; it != root->children.end(); ++it)
    {
        if (it->second->item < del)
        {
//cout << "recursive call on " << it->second->item << endl;
            remove_items(it->second, del);
        }
        else if (it->second->item == del)
        {
//cout << "deleting " << it->second->item << ", parent = " << root->item << endl;
            join_tree(root, it->second);
            root->children.erase(it->first);
            break;
        }
    }
}

void join_tree(node *root, node *append)
{
//    cout << "joining " << root->item << ", and " << append->item << endl;
    map <int, node *>::iterator it = append->children.begin();
    for (; it != append->children.end(); ++it)
    {
        if (root->children.find(it->second->item) == root->children.end())
        {
            root->children.insert(pair <int, node *> (it->second->item, it->second));
            continue;
        }
        root->children.find(it->second->item)->second->item_count += 1;
        join_tree(root->children.find(it->second->item)->second, it->second);
    }
}

node *generate_fptree()
{
    map <int, node *> first_links;
    link_start.push(first_links);
    node *root = new node();
    for(int i=0;i<transactions_fn.size();i++)
    {
        node *next=root;
//        cout<<"Transaction "<<i<<endl;
        for(set<int>::iterator it2=transactions_fn[i].begin();it2!=transactions_fn[i].end();it2++)
        {
        //          cout << "next iteration" << endl;
            if(next->children.find(*it2)!=next->children.end())
            {
                next = next->children.find(*it2)->second;
//                cout << "Updating count of "<<next->item<<" from:"<<next->item_count;
                next->item_count++;
//                cout << " to:"<<next->item_count<<endl;
            }
            else
            {
//                cout<<"Adding:"<<*it2<<endl;
                node *temp = new node(*it2,1);
        //              cout << "inserting";
                (next->children).insert(pair<int,node*>(*it2,temp));
        //              cout << "made" << endl;
                next = temp;
                if (link_start.top().find(*it2) == link_start.top().end())
                    link_start.top().insert(pair<int, node *> (*it2, temp));
            }
        }
    }
    map <int, node *>::iterator it = link_start.top().begin();
    for (; it != link_start.top().end(); ++it)
    {
//cout << "starting item: " << it->first << endl;
        assign_links(it->first, root);
    }
    link_start_orig = link_start.top();
    return root;
}

node *assign_links(int item, node *curr, node *prev)
{
    if (curr->item > item) return prev;
    if (curr->item == item)
    {
        if (prev != NULL)
            prev->link = curr;
        else
            link_start.top().find(item)->second = curr;
        curr->link = NULL;
        return curr;
    }
    map <int, node *>::iterator it = curr->children.begin();
    for (; it != curr->children.end(); ++it)
        prev = assign_links(item, it->second, prev);
    return prev;
}

void reset(node *n)
{
    n->path = false;
    map <int, node *>::iterator it;
    for (it = n->children.begin(); it != n->children.end(); ++it)
        reset(it->second);
}

bool mark_path(int item, node *n)
{
//    map <int, node *> new_links;
//    link_start.push(new_links);
    if (n->item > item) return false;
    if (n->item == item)
    {
        n->path = false;
        return true;
    }
    bool val = false;
    map <int, node *>::iterator it;
    for (it = n->children.begin(); it != n->children.end(); ++it)
        val = mark_path(item, it->second) || val;
    n->path = val;
    return val;
}

node* make_sub(node *n, int item)
{
//cout << "(" << n->item << ", " << n->item_count << ", " << n->path << ")" << endl;
//    node *next_link = n->link;
//    while (next_link != NULL && next_link->path != true)
//        next_link = next_link->link;
    int counter = 0;
    node *x = new node(n->item, 0, NULL);
    if (link_start.top().find(x->item) == link_start.top().end())
        link_start.top().insert(pair<int, node*> (x->item, x));
    map <int, node *>::iterator it;
    for (it = n->children.begin(); it != n->children.end(); ++it)
        if (it->second->path == true)
            x->children.insert(pair<int, node*> (it->first, make_sub(it->second, item)));
    for (it = x->children.begin(); it != x->children.end(); ++it)
        counter += it->second->item_count;
    x->item_count = counter;
    for (map <int, node *>::iterator it2 = n->children.begin(); it2 != n->children.end(); ++it2)
        if (it2->second->item == item)
            x->item_count += it2->second->item_count;
    return x;
}

void Print_tree(node *n, int tabs)
{
    for (int i = 0; i < tabs; ++i)
        cout << "     ";
    cout << "(" << n->item << ", " << n->item_count << ", " << n->path << ")" << endl;
    map<int,node*>::iterator it = n->children.begin();
    for (; it != n->children.end(); ++it)
        Print_tree(it->second, tabs + 1);
}

void Print_list()
{
    map <int, node *>::iterator it = link_start.top().begin();
    for (; it != link_start.top().end(); ++it)
    {
        cout << "link for " << it->first << ": ";
        node *curr = it->second;
        while (curr != NULL)
        {
            cout << "(" << curr->item << ", " << curr->item_count << ") -> ";
            curr = curr->link;
        }
        cout << endl;
    }
}

void load_example()
{
    set <int> s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
    s1.insert(1); s1.insert(2);
    s2.insert(2); s2.insert(3); s2.insert(4);
    s3.insert(1); s3.insert(3); s3.insert(4); s3.insert(5);
    s4.insert(1); s4.insert(4); s4.insert(5);
    s5.insert(1); s5.insert(2); s5.insert(3);
    s6.insert(1); s6.insert(2); s6.insert(3); s6.insert(4);
    s7.insert(1);
    s8.insert(1); s8.insert(2); s8.insert(3);
    s9.insert(1); s9.insert(2); s9.insert(4);
    s10.insert(2); s10.insert(3); s10.insert(5);
    transactions_fn.push_back(s1);
    transactions_fn.push_back(s2);
    transactions_fn.push_back(s3);
    transactions_fn.push_back(s4);
    transactions_fn.push_back(s5);
    transactions_fn.push_back(s6);
    transactions_fn.push_back(s7);
    transactions_fn.push_back(s8);
    transactions_fn.push_back(s9);
    transactions_fn.push_back(s10);
}
