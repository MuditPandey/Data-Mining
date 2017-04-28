#include<bits/stdc++.h>
#include<string>
#include<fstream>
#include<limits>
#define BINS 5
#define CONTINOUS_ATTR 8
#define BOOL_ATTR 1
using namespace std;
void get_transaction_data(string file);
map<int,vector<float> > transactions;
vector< set<int> > transactions_in;
vector< set<int> > transactions_fn;
vector< pair<int,int> > sort_list;
map<int,int> one_item_set;
map<int,int> mapping_on;
map<int,int> mapping_no;

int main()
{
    for(int i=1;i<=CONTINOUS_ATTR*BINS+2*BOOL_ATTR;i++)
 	{
     one_item_set.insert(pair<int,int> (i,0));
 	}
    string input_file="pima.txt";
    get_transaction_data(input_file);
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

           cout<<inp<<" ";
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
        cout<<endl;
        tid++;
    }
    fil.close();
    pair<float,float> max_min[9];
    for(int i=0;i<9;i++)
    {
        max_min[i].first=INT_MIN;
        max_min[i].second=INT_MAX;
    }
    cout<<endl<<"No. of transactions="<<tid<<" in map="<<transactions.size()<<endl<<endl;;

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
     for(int i=0;i<9;i++)
    {
        cout<<"MAX="<<max_min[i].first<<" MIN="<<max_min[i].second<<endl;
    }
    for(map<int,vector<float> >::iterator it=transactions.begin();it!=transactions.end();it++)
    {
        //cout<<it->second.size()<<"**"<<endl;
        set<int> add;
        for(int i=0;i<it->second.size();i++)
            {
                int bin_no=it->second[i]*BINS/(max_min[i].first-max_min[i].second)+1;
                if(bin_no>BINS)
                	bin_no-=1;
                if(i==it->second.size()-1 && bin_no==5)
                	bin_no=2;
                int item_no=i*BINS+bin_no;
                add.insert(item_no);
                one_item_set.find(item_no)->second++;	
      	    //cout<<it->second[i]<<" is in bin "<<bin_no<<" for max="<<max_min[i].first<<" min="<<max_min[i].second<<" interval-size="<<float(max_min[i].first-max_min[i].second)/BINS<<" Alloted item no="<<item_no<<endl;		
            }
            transactions_in.push_back(add);
            add.clear();
        //cout<<endl;
    }	
}
