#include<bits/stdc++.h>
#include <boost/algorithm/string.hpp>
typedef long long ll;
#define ff first
#define ss second
using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
#define For(i,n) for(int i=0;i<(int)n;i++)
string ltrim(const string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}
string rtrim(const string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
string trim(const string& s)
{
    return rtrim(ltrim(s));
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}
int checkiftxt(char a[])
{
    int len=strlen(a);
    string s;
    if(len<4)
        return 0;
    else
    {
        s+=a[len-1];
        s+=a[len-2];
        s+=a[len-3];
        s+=a[len-4];
        return !(s.compare("txt."));
    }
    //Function takes a character array and determines if the last 4 characters are ".txt"
}
class Checker
{
    string change(string x) {
        string temp=trim(x);
        transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
        return temp;
    }
    void getNames();
    void getPreferences();
    void getMates();

    fstream fin, fout;
    set <string> Vertex;
    map <string, vector<string> > Table;
    map < pair <string,string> , ll> Rank;
    vector <pair<string, string>> mates;
    map <pair<string, string>, bool> Exist;
    set <string> Vertex2;

public:
    Checker(char in[], char out[])
    {
        if(!checkiftxt(in) || !checkiftxt(out))
        {
            cerr<<"Please give arguments in .txt form"<<endl;
            exit(0);
        }
        fin.open(in, ios::in);
        fout.open(out, ios::in);
    }
    void check();
};
void Checker::getNames()
{
    string s;
    bool flag=false;
    //s holds the current name

    while(!fin.eof())
    {
        getline(fin,s,'\n');
        if(s=="@End")
            break;
        else if(s=="")
            continue;
        else if(s=="@Names")
        {
            flag=true;
            continue;
        }
        else if(flag)
        {
            //Stores locations of the commas/separators
            ll prevcom=-1,nextcom=-1;
            while(1)
            {
                //modifying from previous values
                prevcom=nextcom;
                nextcom=prevcom+1;

                //checking for the next separtor
                while(nextcom<s.size() && s[nextcom]!=',' && s[nextcom]!=';')
                    nextcom++;

                //first condition implies 0 length name second implies that the name does not end in this line
                if(nextcom==prevcom+1||nextcom==s.size())
                {
                    cerr<<"Invalid Vertices "<<'\n';
                    exit(0);
                }

                //Takes a substring from s starting from prevcom to nextcom
                string temp=s.substr(prevcom+1,nextcom-prevcom-1);

                //trims and converts to uppercase the name
                temp=change(temp);

                //checks for Duplicate names as the set size should increase after the addition of the name.
                int inisize=Vertex.size();
                Vertex.insert(temp);
                if(Vertex.size()==inisize) {
                    cerr<<"Duplicate Vertices "<<'\n';
                    exit(0);
                }

                //Terminating condition as the names end with a semicolon
                if(s[nextcom]==';')
                    break;
            }
        }
        else
        {
            cerr<<"Invalid Format"<<'\n';
            exit(0);
        }
        flag=!flag;
    }
    if(flag)
    {
        cerr<<"Missing Vertices"<<'\n';
        exit(0);
    }
    //We can save some precomputaion as there is no stable matching for odd sized sets.
    if(Vertex.size()%2==1) {
        fout<<"No Stable Roommates Matching Possible"<<'\n';
        exit(0);
    }
}
void Checker::getPreferences()
{
    bool flag=false;
    string s;
    //The code is to ensure no dupliate elements in the preference list and the same person not giving his own name.
    int sz=Vertex.size();
    map<string, set<string>> Lists;
    for(auto x: Vertex)
        Lists[x].insert(x);

    //Initialising the Preference table count starts at 5 as the first 3 lines have been taken by the names
    //there is supposed to be an endl after that followed by a preference list
    ll lineno=5;

    while(!fin.eof())
    {
        getline(fin,s,'\n');
        if(s=="@End")
            break;
        else if(s=="")
            continue;
        else if(s=="@PreferenceLists")
        {
            flag=true;
            continue;
        }
        else if(flag)
        {
            lineno++;
            ll col=0,prevcom=0,nextcom=0,count=0;
            //col denotes the index where the colon appears

            while(col<s.size()&&s[col]!=':') col++;

            if(col==0||col==s.size())
            {
                cerr<<"Invalid Preference List at "<<lineno<<'\n';
                exit(0);
            }
            //Erroneous cases are dealt above in which the first character is colon and colon does not appear

            string curr=s.substr(0,col);
            curr=change(curr);
            //trims and uppercases curr

            if(Vertex.count(curr)==0)
            {
                cerr<<"Input "<<s.substr(0,col)<<" Missing in Vertices"<<'\n';
                exit(0);
            }
            //case where curr does not exist in names

            nextcom=col;
            while(1)
            {
                count++;
                //to store ranks

                prevcom=nextcom;
                nextcom=prevcom+1;

                while(nextcom<s.size()&&s[nextcom]!=','&&s[nextcom]!=';')
                    nextcom++;

                if(nextcom==prevcom+1||nextcom==s.size())
                {
                    cerr<<"Invalid Preference List at "<<lineno<<'\n';
                    exit(0);
                }
                //Dealing with empty strings and no semicolon or comma to complete the string

                string temp=s.substr(prevcom+1,nextcom-prevcom-1);
                temp=change(temp);

                int iniListSize=Lists[curr].size();
                Lists[curr].insert(temp);
                if(Lists[curr].size()==iniListSize)
                {
                    cerr<<"Invalid Preference List at "<<lineno<<'\n';
                    exit(0);
                }
                //If the name has already been added to he preference list

                Table[curr].push_back(temp);
                Rank[make_pair(curr,temp)]=count;
                if(s[nextcom]==';')
                    break;
            }
        }
        else
        {
            cerr<<"Invalid Format"<<'\n';
            exit(0);
        }
    }
    if(!flag)
    {
        cerr<<"Missing Preference Lists"<<'\n';
        exit(0);
    }
}
void Checker::getMates()
{
    string s;
    while(getline(fout,s,'\n'))
    {
        if(s=="@End")
            break;
        else if(s=="@Roommates")
            continue;
        else
        {
            int cnt=0;
            string n1="",n2="";
            For(i, s.length()) if(s[i]=='~') cnt++;
            if(cnt!=1) {
                cout<<"Invalid Format"<<" "<<cnt<<endl;
                exit(0);
            }
            //checks for number of '~'

            For(i, s.length()) {
                if(s[i]!='~')
                    n1+=s[i];
                else
                    break;
            }
            for(int i=n1.length()+1; i<s.length(); i++) {
                n2+=s[i];
            }
            //sperates to two strings

            n1=trim(n1);
            n2=trim(n2);
            transform(n1.begin(), n1.end(), n1.begin(), ::toupper);
            transform(n2.begin(), n2.end(), n2.begin(), ::toupper);
            //trims andd uppercases

            if(n1.compare(n2)==0) {
                cout<<"Repeated Names in answer"<<endl;
                exit(0);
            }
            if(Vertex.count(n1)==0 || Vertex.count(n2)==0 ) {
                cout<<"Wrong Names in answer"<<endl;
                exit(0);
            }
            if(Vertex2.count(n1)!=0 || Vertex2.count(n2)!=0) {
                cout<<"Repeated Names in answer"<<endl;
                exit(0);
            }
            //checks for invaild input

            mates.push_back(make_pair(n1,n2));
            Vertex2.insert(n2);
            Vertex2.insert(n1);
            //modifies if correct

        }
    }
}
void Checker::check()
{
    getNames();
    getPreferences();
    getMates();
    for(auto x:Vertex) for(auto y:Vertex) Exist[make_pair(x,y)]=false;
    for(auto x:Vertex) for(auto y:Table[x]) Exist[make_pair(x,y)]=true;

    for(auto x:Vertex) for(auto y:Vertex) if(!Exist[make_pair(x,y)]) Rank[make_pair(x,y)]=2*Vertex.size()+3;

    For(i, mates.size()) {
        for(int j=i+1; j<mates.size(); j++) {

            string m11=mates[i].first;
            string m12=mates[i].second;
            string m21=mates[j].first;
            string m22=mates[j].second;

            if((Rank[make_pair(m11, m12)]>Rank[make_pair(m11, m22)] && Rank[make_pair(m22, m21)]>Rank[make_pair(m22, m11)]) ||
                    (Rank[make_pair(m12, m11)]>Rank[make_pair(m12, m21)] && Rank[make_pair(m21, m22)]>Rank[make_pair(m21, m12)]) ||
                    (Rank[make_pair(m11, m12)]>Rank[make_pair(m11, m21)] && Rank[make_pair(m21, m22)]>Rank[make_pair(m21, m11)]) ||
                    (Rank[make_pair(m12, m11)]>Rank[make_pair(m12, m22)] && Rank[make_pair(m22, m21)]>Rank[make_pair(m22, m12)]))
            {
                cout<<"Not a valid Stable Matching"<<endl;
                exit(0);
            }

        }
    }
    if(Vertex.size()==2*mates.size()) {
        cout<<"Valid Stable Matching"<<endl;
    }
    else {
        cout<<"Invalid Stable Matching as all vertices have not been exhausted"<<endl;
    }
}
int main(int argc, char *argv[])
{
    //fast io
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    if(argc!=3)
    {
        cout<<"Enter exactly 2 txt files as input"<<endl;
        exit(0);
    }
    Checker S(argv[1], argv[2]);
    S.check();
    return 0;
}