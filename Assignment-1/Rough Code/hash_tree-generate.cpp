#include <iostream>
#include <set>
#include <vector>
#include <map>
#define CHILD 7

using namespace std;

vector< map< set<int>, int > > freq_item;

struct node
{
    vector< set<int> > data;
    vector<node> children;
};

void Print(node *X, int tabs = 0)
{
//cout << "X1" << endl;
    if (!X->children.empty())
    {
//cout << "X2" << endl;
        for (int i = 0; i < CHILD; ++i)
            Print(&(X->children[i]), tabs + 1);
//cout << "X3" << endl;
    }
//cout << "X4" << endl;
    set<int>::iterator iterat;
//cout << "X5" << endl;
    for (int i = 0; i < X->data.size(); ++i)
    {
//cout << "X6" << endl;
        for (int j = 0; j < tabs; ++j) cout << "   ";
//cout << "X7" << endl;
        for (iterat = X->data[i].begin(); iterat != X->data[i].end(); ++iterat)
        {
//cout << "X8" << endl;
            cout << *iterat << " ";
        }
//cout << "X9" << endl;
        cout << endl;
    }
    cout<<"*";
//cout << "X10" << endl;
}


int find_hashval(int var)
{
    return (var - 1) % CHILD;
}

node generate_hashtree(vector< set<int> > Ck)
{
    node root, *curr;
    set<int>::iterator iterat, iterat2;
    int i, j;
    for (i = 0; i < Ck.size(); ++i)
    {
//cout << "C1" << endl;
        curr = &root;
//cout << "C2" << endl;
        int level = 0;
//cout << "C3" << endl;
        for (iterat = Ck[i].begin(); iterat != Ck[i].end(); iterat++, level++)
        {
//cout << "C4" << endl;
            int hashval = find_hashval(*iterat);
//cout << "C5" << endl;
            if ((curr->children).empty() && (curr->data).size() < CHILD)
            {
//cout << "C6" << endl;
                curr->data.push_back(Ck[i]);
//cout << "C7" << endl;
                break;
            }
//cout << "C8" << endl;
            if (!(curr->children).empty())
            {
//cout << "C9" << endl;
                curr = &curr->children[hashval];
//cout << "C10" << endl;
                continue;
            }
            while ((curr->children).size() < CHILD)
                curr->children.push_back(*(new node));
//cout << "C11" << endl;
            for (j = 0; j < CHILD; ++j)
            {
//cout << "C12" << endl;
                int o;
                for (iterat2 = curr->data[j].begin(), o = 0; o < level; ++iterat2, ++o);
//cout << "C13 checking" << *iterat2 << endl;
                ((curr->children[find_hashval(*iterat2)]).data).push_back(curr->data[j]);
//cout << "printing curr: ";
//Print(&(curr->children[j]));
            }
            curr->children[find_hashval(*(iterat))].data.push_back(Ck[i]);
//cout << "C14 checking" << *iterat << endl;
            curr->data.erase(curr->data.begin(), curr->data.begin() + CHILD);
            break;
        }
//cout << "C15" << endl;
//Print(&root);
//cout << "C16" << endl;
    }
//cout << "C17" << endl;
    curr = &root;
    return root;
}



int main()
{
    set<int> s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
    s1.insert(1); s1.insert(2); s1.insert(3);
    s2.insert(1); s2.insert(2); s2.insert(5);
    s8.insert(2); s8.insert(3); s8.insert(6);
    s3.insert(1); s3.insert(2); s3.insert(6);
    s4.insert(1); s4.insert(3); s4.insert(5);
    s7.insert(2); s7.insert(3); s7.insert(5);
    s5.insert(1); s5.insert(3); s5.insert(6);
    s9.insert(2); s9.insert(5); s9.insert(6);
    s6.insert(1); s6.insert(5); s6.insert(6);
    s10.insert(3); s10.insert(5); s10.insert(6);
    vector <set <int> > v1;
    v1.push_back(s1);
    v1.push_back(s2);
    v1.push_back(s8);
    v1.push_back(s3);
    v1.push_back(s4);
    v1.push_back(s7);
    v1.push_back(s5);
    v1.push_back(s9);
    v1.push_back(s6);
    v1.push_back(s10);
    node nn;
    nn = generate_hashtree(v1);
    Print(&nn);
//    node n1;
//    cout << "c1" << endl;
//    n1.data.push_back(1);
//    n1.data.push_back(2);
//    cout << "c2" << endl;
//    node n2;
//    n2.data.push_back(3);
//    n2.data.push_back(4);
//    cout << "c3" << endl;
//    node n3;
//    n3.data.push_back(5);
//    n3.data.push_back(6);
//    cout << "c4" << endl;
//    n1.children.push_back(n2);
//    cout << "c5" << endl;
//    n1.children.push_back(n3);
//    cout << "c6" << endl;
//    delete(n2);
//    delete(n3);
//    Printer(n1);
//    for(int i = 0; i < n1.children.size(); ++i)
//        Printer(n1.children[i]);
}
