#include<iostream>
#include<bits/stdc++.h>

#define N 435
#define MIN_SUP 0.45
#define MIN_CONF 0.7
#define CHILD 30
#define CAPACITY 30

using namespace std;
/* 34 ITEMS

 1.handicapped-infants=n
 2.handicapped-infants=y
 3.water-project-cost-sharing=n
 4.water-project-cost-sharing=y
 5.adoption-of-the-budget-resolution=n
 6.adoption-of-the-budget-resolution=y
 7.physician-fee-freeze=n
 8.physician-fee-freeze=y
 9.el-salvador-aid=n
 10.el-salvador-aid=y
 11.religious-groups-in-schools=n
 12.religious-groups-in-schools=y
 13.anti-satellite-test-ban=n
 14.anti-satellite-test-ban=y
 15.aid-to-nicaraguan-contras=n
 16.aid-to-nicaraguan-contras=y
 17.mx-missile=n
 18.mx-missile=y
 19.immigration=n
 20.immigration=y
 21.synfuels-corporation-cutback=n
 22.synfuels-corporation-cutback=y
 23.education-spending=n
 24.education-spending=y
 25.superfund-right-to-sue=n
 26.superfund-right-to-sue=y
 27.crime=n
 28.crime=y
 29.duty-free-exports=n
 30.duty-free-exports=y
 31.export-administration-act-south-africa=n
 32.export-administration-act-south-africa=y
 33.Class=republican
 34.Class=democrat

*/
struct node
{
    vector< set<int> > data;
    bool visited;
    vector<node> children;
};
int find_hashval(int );
void Print(node *, int tabs=0);
node generate_hashtree(vector< set<int> > );
bool check_exist(set<int> ,set<int> );
void reset(node* root);
void update_support(map< set<int>,int > *,node *root, set<int> transaction,set<int> check_set, int k, int level = 0);

vector< map< set<int>,int > > freq_item;
map<int,int> one_item_set;
void load_training_data(char*);
vector< set<int> > transactions;
map< set<int>,int > apriori_gen(vector< set<int> >);
bool sim_fk_2(set<int>,set<int>);
vector< set<int> > get_vector(map< set<int>,int> a);
void gen_freq_itemset();
map< set<int>,int> get_1_freq();



int main()
{
 char file[]="vote.arff";
 for(int i=1;i<=34;i++)
 {
     one_item_set.insert(pair<int,int> (i,0));
 }
 load_training_data(file);
 cout<<"done\n";
 cout<<"Transactions="<<transactions.size()<<endl;
 gen_freq_itemset();

 return 0;
}

void gen_freq_itemset()
{
  int k=0;
  //Get all frequent 1 item-set
  map< set<int>,int> frequent_one=get_1_freq();
  freq_item.push_back(frequent_one);
  while(1)
  {
      k++;
//Print  freq 1 item sets with their support count and support

     /*for(map< set<int>,int>::iterator it=freq_item[0].begin();it!=freq_item[0].end();it++)
     {
         for(set<int>::iterator it2=it->first.begin();it2!=it->first.end();it2++)
            cout<<*it2;
         cout<<" "<<it->second<<" Support="<<(float)it->second/N<<endl;
     }*/

     //Generates a map of all candidate k item-sets from k-1 item set
     map< set<int>,int> Ck=apriori_gen(get_vector(freq_item[k-1]));
     for(map< set<int>,int>::iterator it=Ck.begin();it!=Ck.end();it++)
     {
         for(set<int>::iterator it2=it->first.begin();it2!=it->first.end();it2++)
            cout<<*it2<<" ";
         cout<<" "<<it->second<<endl;
     }
     node root=generate_hashtree(get_vector(Ck));
     Print(&root);
     for(int i=0;i<transactions.size();i++)
     {
         cout<<i<<"th transaction"<<endl;
         reset(&root);
         update_support(&Ck,&root,transactions[i],transactions[i],k+1,0);
     }
      map< set<int>,int> add;
      for(map< set<int>,int>::iterator it=Ck.begin();it!=Ck.end();it++)
     {
         if((float)it->second/N>=MIN_SUP)
         {
            add.insert(pair< set<int>,int >(it->first,it->second));
         }

     }
     freq_item.push_back(add);
     //Prints the candidate k item-set map
     for(map< set<int>,int>::iterator it=freq_item[k].begin();it!=freq_item[k].end();it++)
     {
         for(set<int>::iterator it2=it->first.begin();it2!=it->first.end();it2++)
            cout<<*it2<<" ";
         cout<<" "<<it->second<<endl;
     }
     if(Ck.empty())
        break;
  }
}

/*
Returns a map with all 1 frequent item-sets along with their support count
*/
map< set<int>,int> get_1_freq()
{
    map< set<int>,int> frequent_one;
     for(map<int,int>::iterator it=one_item_set.begin();it!=one_item_set.end();it++)
 {
     if((float)it->second/435>=MIN_SUP)
        {
            set<int> add;
            add.insert(it->first);
            frequent_one.insert(pair< set<int>,int >(add,it->second));
        }
 }
 return frequent_one;
}
/*
 Reads the training file and loads all transaction details in a data structure and generates one-item sets
*/
void load_training_data(char* file)
{
    fstream f;
    f.open(file,ios::in);
    string str;
    while(getline(f,str))
    {
        if(str[0]=='%' || str[0]=='@')
            continue;
        int k=1;
        set<int> add;
        for(int i=0;i<str.length();i++)
        {
           if(str[i]==',' || str[i]=='\''|| str[i]==' ')
            continue;
           if(str[i]=='n')
                {
                    add.insert(k);
                    one_item_set.find(k)->second++;
                }
           else if(str[i]=='y')
                {
                    add.insert(k+1);
                    one_item_set.find(k+1)->second++;
                }
           else if(str[i]=='r')
               {
                 add.insert(k);
                 one_item_set.find(k)->second++;
                 break;
               }
           else if(str[i]=='d')
                {
                    add.insert(k+1);
                    one_item_set.find(k+1)->second++;
                    break;
                }
           k+=2;
        }
        transactions.push_back(add);
    }
    f.close();
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
        for(int j=i+1;i<fk.size();j++)
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

int find_hashval(int var)
{
    return (var - 1) % CHILD;
}

void Print(node *X, int tabs)
{
    if (!X->children.empty())
    {
        for (int i = 0; i < CHILD; ++i)
            Print(&(X->children[i]), tabs + 1);
        return;
    }
    set<int>::iterator iterat;
    for (int i = 0; i < X->data.size(); ++i)
    {
        for (int j = 0; j < tabs; ++j) cout << "   ";
        for (iterat = X->data[i].begin(); iterat != X->data[i].end(); ++iterat)
        {
            cout << *iterat << " ";
        }
        cout << endl;
    }
    cout << "**" << endl;
}

node generate_hashtree(vector< set<int> > Ck)
{
    node root, *curr;
    set<int>::iterator iterat, iterat2;
    int i, j;
    for (i = 0; i < Ck.size(); ++i)
    {
        curr = &root;
        int level = 0;
        for (iterat = Ck[i].begin(); ; iterat++, level++)
        {
            int hashval = find_hashval(*iterat);
            if ((curr->children).empty() && (curr->data).size() < CAPACITY)
            {
                curr->data.push_back(Ck[i]);
                break;
            }
            if (!(curr->children).empty())
            {
                curr = &curr->children[hashval];
                continue;
            }
            while ((curr->children).size() < CHILD)
                curr->children.push_back(*(new node));
            for (j = 0; j < CAPACITY; ++j)
            {
                int o;
                for (iterat2 = curr->data[j].begin(), o = 0; o < level; ++iterat2, ++o);

                ((curr->children[find_hashval(*iterat2)]).data).push_back(curr->data[j]);
            }
            curr->children[find_hashval(*(iterat))].data.push_back(Ck[i]);
            curr->data.erase(curr->data.begin(), curr->data.begin() + CAPACITY);
            break;
        }
    }
    curr = &root;
    return root;
}
bool check_exist(set<int> a,set<int> b)
{
    /*cout<<"Checking set=";
    for(set<int>::iterator pl=a.begin();pl!=a.end();pl++)
                    cout<<*pl<<" ";
                cout<<"with transaction"<<endl;*/
    set<int>::iterator it=a.begin();
    set<int>::iterator it2=b.find(*it);
    if(it2==b.end())
        return false;
    while(it!=a.end() && it2!=b.end())
    {
        if(*it==*it2)
        {
         it++;it2++;
        }
        else
            it2++;
    }
    if(it==a.end())
        return true;
    return false;
}

void reset(node* root)
{
    root->visited = false;
    for (int i = 0; i < root->children.size(); ++i)
        reset(&root->children[i]);
}

void update_support(map< set<int>,int > *Ck,node *root, set<int> transaction,set<int> check_set, int k, int level)
{
    if(k<0)
        return;
    if(root->children.empty() && root->visited==false)
    {
        root->visited=true;
        for(int i=0;i<root->data.size();i++)
        {
            if(check_exist(root->data[i],transaction))
            {
                Ck->find(root->data[i])->second++;
                //cout<<"+1 supp count for itemset=";
                //For Debugging
                /*for(set<int>::iterator pl=root->data[i].begin();pl!=root->data[i].end();pl++)
                    cout<<*pl<<" ";
                cout<<endl;*/
            }
        }
        return;
    }
    else if(root->children.empty() && root->visited==true)
        return;

    set<int>::iterator it1=check_set.begin(),it2;
    for(int i=0;i<check_set.size()-k+1;it1++,i++)
    {
        set<int> new_check(check_set);
        new_check.erase(new_check.begin(),++new_check.find(*it1));
        //For Debugging
        /*cout<<"Hashing on="<<*it1<<endl;
        cout<<"Level="<<level<<" k="<<k<<" New set=";
        for(set<int>::iterator pl=new_check.begin();pl!=new_check.end();pl++)
                    cout<<*pl<<" ";
                cout<<endl;*/

        update_support(Ck,&root->children[find_hashval(*it1)],transaction,new_check,k-1,level+1);
    }
    //cout<<"xxxxxxxxx"<<endl;
}
