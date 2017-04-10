#include <iostream>
#include <set>
#include <vector>
#include <map>
#define CHILD 3
#define CAPACITY 3

using namespace std;
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
void reset(node root);
void update_support(node *root, set<int> transaction,set<int> check_set, int k, int level = 0);



int main()
{
    set<int> s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15;
    s1.insert(1); s1.insert(4); s1.insert(5);
    s8.insert(1); s8.insert(5); s8.insert(9);
    s11.insert(5); s11.insert(6); s11.insert(7);
    s12.insert(3); s12.insert(5); s12.insert(7);
    s3.insert(1); s3.insert(3); s3.insert(6);
    s4.insert(2); s4.insert(3); s4.insert(4);
    s7.insert(3); s7.insert(4); s7.insert(5);
    s5.insert(3); s5.insert(5); s5.insert(6);
    s9.insert(3); s9.insert(6); s9.insert(7);
    s6.insert(4); s6.insert(5); s6.insert(7);
    s2.insert(1); s2.insert(2); s2.insert(5);
    s15.insert(1); s15.insert(2); s15.insert(4);
    s10.insert(4); s10.insert(5); s10.insert(8);
    s13.insert(3); s13.insert(6); s13.insert(8);
    s14.insert(6); s14.insert(8); s14.insert(9);
    vector <set <int> > v1;
    v1.push_back(s1);
    v1.push_back(s8);
    v1.push_back(s11);
    v1.push_back(s12);
    v1.push_back(s3);
    v1.push_back(s4);
    v1.push_back(s7);
    v1.push_back(s5);
    v1.push_back(s9);
    v1.push_back(s6);
    v1.push_back(s2);
    v1.push_back(s15);
    v1.push_back(s10);
    v1.push_back(s13);
    v1.push_back(s14);

    node nn;
    nn = generate_hashtree(v1);
    reset(nn);
    Print(&nn);

    set<int> transaction;
    transaction.insert(1);
    transaction.insert(2);
    transaction.insert(4);
    transaction.insert(5);
    transaction.insert(6);
    transaction.insert(9);
    cout<<"****************************\n";
    update_support(&nn, transaction,transaction, 3);
    cout<<"****************************\n";
    cout<<"Check s1:"<<check_exist(s1,transaction)<<endl;
    cout<<"Check s2:"<<check_exist(s2,transaction)<<endl;
    cout<<"Check s3:"<<check_exist(s3,transaction)<<endl;
    cout<<"Check s4:"<<check_exist(s4,transaction)<<endl;
    cout<<"Check s5:"<<check_exist(s5,transaction)<<endl;
    cout<<"Check s6:"<<check_exist(s6,transaction)<<endl;
    cout<<"Check s7:"<<check_exist(s7,transaction)<<endl;
    cout<<"Check s8:"<<check_exist(s8,transaction)<<endl;
    cout<<"Check s9:"<<check_exist(s9,transaction)<<endl;
    cout<<"Check s10:"<<check_exist(s10,transaction)<<endl;
    cout<<"Check s11:"<<check_exist(s11,transaction)<<endl;
    cout<<"Check s12:"<<check_exist(s12,transaction)<<endl;
    cout<<"Check s13:"<<check_exist(s13,transaction)<<endl;
    cout<<"Check s14:"<<check_exist(s14,transaction)<<endl;
    cout<<"Check s15:"<<check_exist(s15,transaction)<<endl;


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
    cout<<"Checking set=";
    for(set<int>::iterator pl=a.begin();pl!=a.end();pl++)
                    cout<<*pl<<" ";
                cout<<"with transaction"<<endl;
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

void reset(node root)
{
    root.visited = false;
    for (int i = 0; i < root.children.size(); ++i)
        reset(root.children[i]);
}

void update_support(node *root, set<int> transaction,set<int> check_set, int k, int level)
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
                cout<<"+1 supp count for itemset=";
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
                cout<<endl;
        */
        update_support(&root->children[find_hashval(*it1)],transaction,new_check,k-1,level+1);
    }
    //cout<<"xxxxxxxxx"<<endl;
}
