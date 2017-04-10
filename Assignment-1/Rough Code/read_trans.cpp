#include<fstream>
#include<bits/stdc++.h>
#define MIN_SUP 0.45
using namespace std;
map<int,int> one_item_set;
void load_training_data(char*);
vector< set<int> > transactions;

map< set<int>,int >func();
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

 for(int i=0;i<transactions.size();i++)
 {
     for(set<int>::iterator it2=transactions[i].begin();it2!=transactions[i].end();it2++)
     {
         cout<<*it2<<" ";
     }
    cout<<endl;
 }
 /*for(map<int,int>::iterator it=one_item_set.begin();it!=one_item_set.end();it++)
 {
     if((float)it->second/435>=MIN_SUP)
        {
            set<int> add;
            add.insert(it->first);
            frequent_one.insert(pair< set<int>,int >(add,it->second));
        }
 }*/
 map< set<int>,int> frequent_one=func();
 for(map< set<int>,int>::iterator it=frequent_one.begin();it!=frequent_one.end();it++)
 {
     for(set<int>::iterator it2=it->first.begin();it2!=it->first.end();it2++)
        cout<<*it2;
     cout<<" "<<it->second<<" Support="<<(float)it->second/435<<endl;
 }
 return 0;
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
        transactions.push_back(add);
    }
    f.close();

}
map< set<int>,int> func()
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
