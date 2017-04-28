#include<bits/stdc++.h>
#include<string>
#include<fstream>
#include<limits>
#define BINS 5
using namespace std;
void get_transaction_data(string file);
map<int,vector<float> > transactions;
int main()
{
    string input_file="pima.txt";
    get_transaction_data(input_file);
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
        for(int i=0;i<it->second.size();i++)
            {
                int bin_no=it->second[i]*BINS/(max_min[i].first-max_min[i].second)+1;
                if(bin_no>BINS)
                	bin_no-=1;
      	    cout<<it->second[i]<<" is in bin "<<bin_no<<" for max="<<max_min[i].first<<" min="<<max_min[i].second<<" interval size="<<float(max_min[i].first-max_min[i].second)/BINS<<endl;		
            }
        //cout<<endl;
    }	
}
