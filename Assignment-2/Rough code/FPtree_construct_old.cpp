#include<fstream>
#include<bits/stdc++.h>
#define MIN_SUP 0.45
using namespace std;
void load_training_data(char*);
map<int,int> one_item_set;
map<int,int> mapping_on;
map<int,int> mapping_no;
vector< pair<int,int> > sort_list;
vector< set<int> > transactions_in;
vector< set<int> > transactions_fn;
struct node
{
    int item;
    int item_count;
    map<int,node*> children;
    node *next;
    node(int val,int ct)
    {
        item=val;
        item_count=ct;
    }
};
node *root=new node(0,0);
void generate_fptree();
int main()
{
 for(int i=1;i<=34;i++)
 {
     one_item_set.insert(pair<int,int> (i,0));
 }
 char file[]="vote.arff";
 load_training_data(file);
 for(map<int,int>::iterator it=one_item_set.begin();it!=one_item_set.end();it++)
 {
     sort_list.push_back(pair<int,int>(it->second,it->first));
     cout<<"item no.="<<it->first<<" count="<<it->second<<endl;
 }
 sort(sort_list.begin(),sort_list.end());
 one_item_set.clear();
 int k=1;
 for(int i=sort_list.size()-1;i>=0;i--)
 {
     one_item_set.insert(pair<int,int>(k,sort_list[i].first));
     //change meaning map here too
     mapping_on.insert(pair<int,int>(sort_list[i].second,k));
     mapping_no.insert(pair<int,int>(k,sort_list[i].second));
     cout<<"inserted="<<k<<" corresponding to "<<sort_list[i].second<<endl;
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
 generate_fptree();

 return 0;
}
void generate_fptree()
{

  for(int i=0;i<transactions_fn.size();i++)
 {
     node *next=root;
     cout<<"Transaction "<<i<<endl;
     for(set<int>::iterator it2=transactions_fn[i].begin();it2!=transactions_fn[i].end();it2++)
     {
         if(next->children.find(*it2)!=next->children.end())
         {
            next=next->children.find(*it2)->second;
            cout<<"Updating count of "<<next->item<<" from:"<<next->item_count;
            next->item_count++;
            cout<<" to:"<<next->item_count<<endl;
         }
         else
         {
            cout<<"Adding:"<<*it2<<endl;
            node *temp=new node(*it2,1);
            next->children.insert(pair<int,node*>(*it2,temp));
            next=next->children.find(*it2)->second;
         }
     }
 }
}
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
        transactions_in.push_back(add);
    }
    f.close();

}
