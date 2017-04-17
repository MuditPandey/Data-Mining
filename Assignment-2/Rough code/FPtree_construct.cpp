#include<fstream>
#include<bits/stdc++.h>
#define MIN_SUP 0.45

using namespace std;

void load_training_data(char*);
void load_example();
vector< set<int> > transactions;
void generate_fptree();

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
void Print_list();
node *assign_links(int item, node *curr, node *prev = NULL);
node *root = new node();
stack < map <int, node *> > link_start;
map <int, node *> link_start_orig;
void reset(node *n);
bool mark_path(int item, node *n);
node* make_sub(node *n);

int main()
{
    char file[]="vote.txt";
    //load_training_data(file);
    load_example();
    cout << "done\n";
    cout << "Transactions=" << transactions.size() << endl;
    generate_fptree();
    Print_list();
    //reset(root);
    mark_path(5, root); //e = 5, d = 4
    node *sub = make_sub(root);
    Print_tree(root, 0);
    Print_tree(sub, 0);
    Print_list();
    return 0;
}

void generate_fptree()
{
    map <int, node *> first_links;
    link_start.push(first_links);
    for(int i=0;i<transactions.size();i++)
    {
        node *next=root;
        cout<<"Transaction "<<i<<endl;
        for(set<int>::iterator it2=transactions[i].begin();it2!=transactions[i].end();it2++)
        {
        //          cout << "next iteration" << endl;
            if(next->children.find(*it2)!=next->children.end())
            {
                next = next->children.find(*it2)->second;
                cout << "Updating count of "<<next->item<<" from:"<<next->item_count;
                next->item_count++;
                cout << " to:"<<next->item_count<<endl;
            }
            else
            {
                cout<<"Adding:"<<*it2<<endl;
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
cout << "starting item: " << it->first << endl;
        assign_links(it->first, root);
    }
    link_start_orig = link_start.top();
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
    map <int, node *> new_links;
    link_start.push(new_links);
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

node* make_sub(node *n)
{
//cout << "(" << n->item << ", " << n->item_count << ", " << n->path << ")" << endl;
    if (link_start.top().find(n->item) == link_start.top().end())
        link_start.top().insert(pair<int, node*> (n->item, n));
    node *next_link = n->link;
    while (next_link != NULL && next_link->path == false)
        next_link = next_link->link;
    int counter = 0;
    node *x = new node(n->item, 0, next_link);
    map <int, node *>::iterator it;
    for (it = n->children.begin(); it != n->children.end(); ++it)
        if (it->second->path)
            x->children.insert(pair<int, node*> (it->first, make_sub(it->second)));
    for (it = x->children.begin(); it != x->children.end(); ++it)
        counter += it->second->item_count;
    if (counter == 0)
        x->item_count = n->item_count;
    else
        x->item_count = counter;
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
    transactions.push_back(s1);
    transactions.push_back(s2);
    transactions.push_back(s3);
    transactions.push_back(s4);
    transactions.push_back(s5);
    transactions.push_back(s6);
    transactions.push_back(s7);
    transactions.push_back(s8);
    transactions.push_back(s9);
    transactions.push_back(s10);
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
                }
           else if(str[i]=='y')
                {
                    add.insert(k+1);
                }
           else if(str[i]=='r')
               {
                 add.insert(k);
                 break;
               }
           else if(str[i]=='d')
                {
                    add.insert(k+1);
                    break;
                }
           k+=2;
        }
        transactions.push_back(add);
    }
    f.close();

}
