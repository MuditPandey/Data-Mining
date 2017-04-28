#include<bits/stdc++.h>
#include<string>
#include<fstream>
#include<limits>
#define BIN 5
using namespace std;
void get_transaction_data(string file);
map<int,vector<float> > transactions;
int main()
{
    string input_file="pima-indians-diabetes.data";
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
    pair<float,float> min_max[9];
    for(int i=0;i<9;i++)
    {
        min_max[i].first=INT_MIN;
        min_max[i].second=INT_MAX;
    }
    cout<<endl<<"No. of transactions="<<tid<<" in map="<<transactions.size()<<endl<<endl;;

    for(map<int,vector<float> >::iterator it=transactions.begin();it!=transactions.end();it++)
    {
        //cout<<it->second.size()<<"**"<<endl;
        for(int i=0;i<it->second.size();i++)
            {
                if(it->second[i]>min_max[i].first)
                    min_max[i].first=it->second[i];
                if(it->second[i]<min_max[i].second)
                    min_max[i].second=it->second[i];
                //cout<<it->second[i]<<" ";
            }
        //cout<<endl;
    }
     for(int i=0;i<9;i++)
    {
        cout<<"MAX="<<min_max[i].first<<" MIN="<<min_max[i].second<<endl;
    }

}
