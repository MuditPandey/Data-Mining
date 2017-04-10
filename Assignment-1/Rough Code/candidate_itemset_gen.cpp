#include<bits/stdc++.h>

using namespace std;

vector< set<int> > apriori_gen(vector< set<int> >);
bool sim_fk_2(set<int>,set<int>);

int main()
{
 vector< set<int> > fk;
 int a[]={1,2,3,4,5,6,7,1,7,5};
 set<int> add1(a,a+3);
 set<int> add2(a+1,a+4);
 set<int> add4(a+2,a+4);
 add4.insert(1);
 set<int> add5(a+6,a+8);
  set<int> add6(add5);
 add5.insert(3);
 add6.insert(4);
 set<int> add7(a+2,a+4);
 add7.insert(7);
 set<int> add8(a,a+2);
 add8.insert(5);
 set<int> add9(a,a+2);
 add9.insert(6);
 set<int> add10(a+4,a+6);
 add10.insert(2);
 set<int> add11(a+4,a+6);
 add11.insert(1);
 set<int> add12(a+2,a+5);
 set<int> add13(a+8,a+10);
 add13.insert(3);
 set<int> add14(a+8,a+10);
 add14.insert(4);
 fk.push_back(add1);
 fk.push_back(add2);
 fk.push_back(add4);
 fk.push_back(add5);
 fk.push_back(add6);
 fk.push_back(add7);
 fk.push_back(add8);
 fk.push_back(add9);
 fk.push_back(add10);
 fk.push_back(add11);
 fk.push_back(add12);
 fk.push_back(add13);
 fk.push_back(add14);
 sort(fk.begin(),fk.end());
 cout<<"START\n";
 cout<<fk.size()<<endl;
 for(int j=0;j<fk.size();j++)
    {
        for(set<int>::iterator it=fk[j].begin();it!=fk[j].end();++it)
            cout<<*it<<" ";
        cout<<endl;
    }
 vector< set<int> > ck=apriori_gen(fk);
 cout<<"CANDIDATE SETS\n";
 for(int j=0;j<ck.size();j++)
    {
        for(set<int>::iterator it=ck[j].begin();it!=ck[j].end();++it)
            cout<<*it<<" ";
        cout<<endl;
    }
 return 0;

}

vector< set<int> > apriori_gen(vector< set<int> > fk)
{
    vector< set<int> > candidate;
    for(int i=0;i<fk.size()-1;i++)
    {
        for(int j=i+1;i<fk.size();j++)
        {
          if(sim_fk_2(fk[i],fk[j]))
          {
              set<int> add(fk[i]);
              add.insert(*fk[j].rbegin());
              candidate.push_back(add);
          }
          else
            break;
        }
    }
    return candidate;
}

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

