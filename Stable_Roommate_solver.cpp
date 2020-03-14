/************************************************
 * Authors: R Raghu Raman and Arnhav Datar      *
 * Professor: Dr. Meghana Nasre                 *
 *                                              *
 * Input Format:                                *
 * @Names                                       *
 * <name1>, <name2>, <name3>, <name4>;          *
 * @End                                         *
 *                                              *
 * @PreferenceLists                             *
 * <name1>: <name2>, <name3>;                   *
 * <name2>: <name1>;                            *
 * <name3>: <name1>, <name4>;                   *
 * <name4>: <name3>;                            *
 * @End                                         *
 *                                              *
 *@Comments                                     *
 *(if any this section is optional              *
 *@End                                          *
 *                                              *1
 * Output Format:                               *
 * @Roommates                                   *
 * <name1>~<name2>                              *
 * <name3>~<name4>                              *
 * @End                                         *
 ************************************************/
#include<bits/stdc++.h>
typedef long long ll;
#define ff first
#define ss second
using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
int linecount=0;
inline string ltrim( const string& s )
{
	size_t start = s.find_first_not_of( WHITESPACE );
	return ( start == string::npos ) ? "" : s.substr( start );
}
inline string rtrim( const string& s )
{
	size_t end = s.find_last_not_of( WHITESPACE );
	return ( end == string::npos ) ? "" : s.substr( 0, end + 1 );
}
inline string trim( const string& s )
{
	return rtrim( ltrim( s ) );
}
int checkiftxt( char a[] )
{
	int len=strlen( a );
	string s;
	if( len<4 )
		return 0;
	else
	{
		s+=a[len-1];
		s+=a[len-2];
		s+=a[len-3];
		s+=a[len-4];
		return !( s.compare( "txt." ) );
	}
	//Function takes a character array and determines if the last 4 characters are ".txt"
}
class StableRoommates
{
	void advance( int &b, int &w, string x );
	void deleteFromTable( string x, string y );
	string change( string x ){
		string temp=trim( x );
		transform( temp.begin(), temp.end(), temp.begin(), ::toupper );
		return temp;
	}
	void notPossible();
	void update( string w, vector <string>:: iterator &bst, vector <string>:: iterator &wst, vector <string>:: iterator &rst );
    void deleteFrom( int st, int en, string x );
public:

	fstream fin, fout;

	//Vertex stores all the names, Table stores the preference table, and Rank store the rank of person b in a's Preference List
	set <string> Vertex;
  	map <string, vector<string> > Table;
	map < pair <string, string>, ll> Rank;
  	set <pair<string, string>> mates;
  	map <pair<string, string>, bool> Exist;

	StableRoommates( char in[], char out[] )
  	{
		if( !checkiftxt( in ) || !checkiftxt( out ) )
		{
	  		cerr << "Please give arguments in .txt form" << endl;
	  		exit( 0 );
		}
		fin.open( in, ios::in );
		fout.open( out, ios::out );
  	}
  	void getNames();
  	void phase1();
  	void getPreferences();
  	void print();
  	void debug();
  	void phase2();
};
inline void StableRoommates::deleteFromTable( string x, string y )
{
	Table[x][Rank[make_pair( x, y )]-1] = "";
	Exist[make_pair( x, y )] = false;
}
void StableRoommates::deleteFrom( int st, int end, string y ){
  for( int i = st; i < end; i++ )
  {
      if( Table[y][i] != "" )	//if has not been deleted
      {
          string z=Table[y][i];
          if( Exist[make_pair( z, y )] ) deleteFromTable( z, y );
          deleteFromTable( y, z );
      }

  }
}
void StableRoommates::update( string w, vector <string>:: iterator &bst, vector <string>:: iterator &wst, vector <string>:: iterator &rst  ){
	//Updating rot
    while( rst <=wst && *( rst ) == "" )
        rst++;
    if( rst > wst )
        rst = Table[w].end();

    //Updating worst
    while( wst >= bst && *( wst ) == "" )
        wst--;
    if( wst < bst )
        notPossible();

    //Updating the best
    if( *( bst ) == "" )
    {
        if( rst == Table[w].end() )
            notPossible();
        bst = rst;
        rst++;
        while( rst <= wst && *( rst ) == "" )
            rst++;
        if( rst > wst )
            rst = Table[w].end();
    }
}
void StableRoommates::advance( int &b, int &w, string x )
{
	while( b != w && b != Table[x].size() && Table[x][b] == "" )
		b++;
	if( b == w || b == Table[x].size() ){
		fout << "No Matching" << endl;
		exit( 0 );
	}
}
void StableRoommates::notPossible()
{
	fout << "For the given input there are no stable roommate matchings" << endl;
	exit( 0 );
}
void StableRoommates::getNames()
{
	string s;
	bool flag=false;
	//s holds the current name

	while( !fin.eof() )
	{
		getline( fin, s, '\n' );
		linecount++;
		if( s == "@End" )
			break;
		else if( s == "" )
			continue;
		else if( s == "@Names" )
		{
			flag=true;
			continue;
		}
		else if( flag )
		{
			//Stores locations of the commas/separators
			ll prevcom=-1, nextcom=-1;
			while( 1 )
		  	{
				//modifying from previous values
				prevcom=nextcom;
				nextcom=prevcom+1;

				//checking for the next separtor
				while( nextcom<s.size() && s[nextcom] != ',' && s[nextcom] != ';' )
			 		nextcom++;

				//condition implies 0 length name
				if( nextcom == prevcom+1 )
				{
			  		cerr << "Empty name/Repeated comma at line " << linecount << '\n';
			  		exit( 0 );
				}
				//implies that the name does not end in this line
				if( nextcom == s.size() )
				{
				    cerr << "Last name not completed at line " << linecount << '\n';
			  		exit( 0 );
				}

				//Takes a substring from s starting from prevcom to nextcom
				string temp = s.substr( prevcom+1, nextcom-prevcom-1 );

				//trims and converts to uppercase the name
				temp = change( temp );

				//checks for Duplicate names as the set size should increase after the addition of the name.
				int inisize = Vertex.size();
				Vertex.insert( temp );
				if( Vertex.size() == inisize ){
			  		cerr << "Duplicate Name " << temp << " on line " << linecount << '\n';
			  		exit( 0 );
				}

				//Terminating condition as the names end with a semicolon
				if( s[nextcom] == ';' )
			  		break;
		  	}
		}
		else
		{
		  	cerr << "Invalid Format" << '\n';
		  	exit( 0 );
		}
	}
	if( !flag )
	{
		cerr << "Missing Names" << '\n';
		exit( 0 );
	}
	//We can save some precomputaion as there is no stable matching for odd sized sets.
	if( Vertex.size()%2 == 1 )
	    notPossible();
}
void StableRoommates::getPreferences()
{
	bool flag = false;
	string s;
	//The code is to ensure no dupliate elements in the preference list and the same person not giving his own name.
	int sz=Vertex.size();
	map<string, set<string>> Lists;
	for( auto x: Vertex )
		Lists[x].insert( x );

	/*Initialising the Preference table count starts at 5 as the first 3 lines have been taken by the names and there is supposed to be an endl after that
	followed by a preference list*/
	ll lineno = 5;

	while( !fin.eof() )
	{
		getline( fin, s, '\n' );
		linecount++;

		if( s == "@End" )
			break;
		else if( s == "" )
			continue;
		else if( s == "@PreferenceLists" )
		{
			flag=true;
			continue;
		}
		else if( flag )
		{
			lineno++;
			ll col = 0, prevcom = 0, nextcom = 0, count = 0;
			//col denotes the index where the colon appears

			while( col<s.size() && s[col] != ':' ) col++;

			if( col == 0||col == s.size() )
			{
				cerr << "Empty name at line " << linecount << '\n';
			  	exit( 0 );
			}
			//Erroneous cases are dealt above in which the first character is colon and colon does not appear

			string curr = s.substr( 0, col );
			curr = change( curr );
			//trims and uppercases curr

			if( Vertex.count( curr ) == 0 )
			{
				cerr << "Input " << s.substr( 0, col ) << " Missing in Names" << '\n';
				exit( 0 );
			}
			//case where curr does not exist in names

			nextcom = col;
			while( 1 )
			{
				count++;
				//to store ranks

				prevcom = nextcom;
				nextcom = prevcom+1;

				while( nextcom<s.size() && s[nextcom] != ',' && s[nextcom] != ';' )
					nextcom++;

				if( nextcom == prevcom+1 )
				{
					cerr << "Empty name at " << linecount << '\n';
					exit( 0 );
				}
				//Dealing with empty strings and no semicolon or comma to complete the string
                if( nextcom == s.size() )
                {
                    cerr << "Name not ending on the same line " << linecount << '\n';
                    exit( 0 );
                }

				string temp = s.substr( prevcom+1, nextcom-prevcom-1 );
				temp = change( temp );

				int iniListSize = Lists[curr].size();
				Lists[curr].insert( temp );
				if( curr == temp )
				{
				    cerr << "The preference list of the person " << curr << " has the same person on it on line" << linecount << endl;
				    exit( 0 );
				}
				if( Lists[curr].size() == iniListSize )
				{
					cerr << "Repeated name in" << curr << "'s preference list." << temp << "was repeated on line" << lineno << '\n';
					exit( 0 );
				}
				//If the name has already been added to he preference list

				Table[curr].push_back( temp );
				Rank[make_pair( curr, temp )] = count;
				if( s[nextcom] == ';' )
					break;
			}
		}
		else
		{
			cerr << "Invalid Format" << '\n';
			exit( 0 );
		}
	}
	if( !flag )
	{
		cerr << "Missing Preference Lists" << '\n';
		exit( 0 );
	}
}
void StableRoommates::phase1()
{
  	/*
  	Initialising
  	Free vertices-> people who have not yet proposed to anyone.
  	Best-> Stores the index in Table to the proposed person in the person's preference list
  	Worst-> Stores next index to the best proposal the person has recieved so far.
  	Exist-> Given x, y does ( x, y ) exist in Table
  	*/
	stack <string> fr;
	map <string, int> best, worst;

	for( auto x:Vertex ) for( auto y:Vertex ) Exist[make_pair( x, y )] = false;
	for( auto x:Vertex ) for( auto y: Table[x] ) Exist[make_pair( x, y )] = true;

	for( auto x:Vertex ){
	 	fr.push( x );
	  	best[x] = 0;
	  	worst[x] = Table[x].size()+1;
	  	if( 0 == Table[x].size() )
	  		notPossible();
	}


	while( !fr.empty() )
	{

	  	string x = fr.top();
	  	//x is the proposer

	  	advance( best[x], worst[x], x );
	  	//Gives the next best x has
	  	//Does nothing if x is already at its best

		string y = Table[x][best[x]];
		//y is the person x proposes to.

	  	if( !Exist[make_pair( y, x )] )
	  	{
	  		deleteFromTable( x, y );
			advance( best[x], worst[x], x );
	  	}
	  	//If y's list does not contain x

	 	else if( worst[y] == Table[y].size()+1 )
	  	{
			fr.pop();
			//x has a successful proposal
			worst[y] = Rank[make_pair( y, x )];
			//Assigns worst as the positon after x
            deleteFrom( worst[y], Table[y].size(), y );
			//Symmetrically Deletes every man lower than x on y's list
	  	}
	  	//If y has not recieved any proposal so far.


	  	else if( Rank[make_pair( y, x )]<worst[y] )
	  	{
			string z = Table[y][worst[y]-1];
			//The ex proposer of y
			int prev = worst[y];
			//stores z's index
			worst[y] = Rank[make_pair( y, x )];
			//The new best proposal recieved by y
			deleteFrom( worst[y], prev, y );
			//Symmetrically deletes all elements in y's list from x to z
			fr.pop();
			fr.push( z );
	  	}
	  	//y recieves a proposal from x better than the one it currently had

	  	if( best[x] == worst[x] || best[x] == Table[x].size() )
	  		notPossible();
	}

    //Creating Reduced Prefernce Table
  	map <string, vector<string> > newTable;
  	for( auto x: Vertex )
  	{
		ll count=0;
		for( auto y=Table[x].begin();y != Table[x].end();++y )
		{
			if( *y != "" )
		  	{
				newTable[x].push_back( *y );
				count++;
				Rank[make_pair( x, *y )] = count;
	 		}
		}
  	}
  	Table = newTable;
}
void StableRoommates::debug()
{
  for( auto x:Vertex ){
	cout << x << " : ";
	for( auto y: Table[x] )
	  cout << y << ", ";
	cout << "\b;";
	cout << endl;
  }
}
void StableRoommates::phase2()
{
	//Rotation is a stack containing all the people with more than 2 preferences in their reduced list
    stack <string> rotation;
    /* Initialising
    Best-> Stores the iterator in Table to the best person in the person's preference list
    Worst-> Stores the iterator in Table to the worst person in the person's preference list
    Rot-> Stores the iterator in Table to the second best person in the person's preference list
    */
    map <string, vector <string>:: iterator> rot;
    map <string, vector <string>:: iterator> best, worst;
    //prev is an auxiliary variable used later
    std::vector<string>::iterator prev;

    //Sores whether the element is present in the stack rotation
    map <string, bool> inStack;

    //Initializing best, worst, rot and inStack
    for( auto x: Vertex )
    {
        best[x] = Table[x].begin();
        worst[x] = Table[x].end()-1;
        rot[x] = Table[x].begin()+1;
        inStack[x] = false;
        if( Table[x].size() == 0 )
        	notPossible();
    }
    int step=0;
    //Going through all the people
    for( auto x1 = Vertex.begin();x1 != Vertex.end(); )
    {
    	string x = *x1;
        //If the stack rotation is empty add the current person

        while( !rotation.empty() && best[rotation.top()] == worst[rotation.top()] )
        	rotation.pop();
       	if( rotation.empty() )
        {
        	while( x1 != Vertex.end() && rot[*x1] == Table[*x1].end() )
        		x1++;
        	if( x1 == Vertex.end() )
        		break;
        	x = *x1;
            rotation.push( x );
            inStack[x] = true;
        }
        //Iteratively push the next( x ) until an element repeats in the stack
        string p = rotation.top();
        p = *( worst[*( rot[p] )] );
        while( !inStack[p] )
        {
            rotation.push( p );
            inStack[p] = true;
            p = *( worst[*( rot[p] )] );
        }

        //Make a list of pairs storing the current all or nothing cycle - the person x( i ) and y( i+1 )
        string p1 = rotation.top();
        rotation.pop();
        inStack[p1] = false;
        vector <pair<string, string> > currrot;
        currrot.push_back( make_pair( p1, *( rot[p1] ) ) );
        while( p1 != p )
        {
            p1 = rotation.top();
            rotation.pop();
            inStack[p1] = false;
            currrot.push_back( make_pair( p1, *( rot[p1] ) ) );
        }

        //Eliminating the rotation
        for( auto r: currrot )
        {
        	//Going through the all or nothing cycle
            string x1 = r.ff;
            string y = r.ss;

            //prev stores the current partner of y
            prev = worst[y];
            if( worst[y] != Table[y].end() )
                prev++;

            //worst[y] is updated
            worst[y] = Table[y].begin() + Rank[make_pair( y, x1 )]-1;

            //Going through everyone worse than worst[y] in y's list
            for( auto z = worst[y]+1; z != prev; ++z )
            {
                if( *z != "" )
                {
                	/*If the cuurent person is not deleted
                	delete the cuurent pair and update the values of best worst and rot*/
                    string z1 = *z;
                    Table[z1][Rank[make_pair( z1, y )]-1] = "";

                    //Update best, worst and rot of z1
                    update( z1, best[z1], worst[z1], rot[z1] );

                    //Deleting z from y's list
                    *z = "";
                }
            }
            update( y, best[y], worst[y], rot[y] );
            //Update best, worst and rot of y
        }
    }
    //Finally pairs up all the roommates
    //Stores final sizes of preference table
	std::vector<int> szs;
	for( auto x:Vertex ) szs.push_back( Table[x].size() );
    //If any person's list has size == 0 || size>1
	if( *min_element( szs.begin(), szs.end() ) == 0 && *max_element( szs.begin(), szs.end() )>1 )
	  	notPossible();

    //mates stores all roommates with the alphabetically lower being assigned the first name
	for( auto x:Vertex ){
		if( x.compare( *best[x] )<0 )
		    mates.insert( make_pair( x, *best[x] ) );
		else
		    mates.insert( make_pair( *best[x], x ) );
	  }
}
void StableRoommates::print()
{
	fout << "@Roommates" << endl;
  	for( auto i: mates ){
		fout << i.ff << " ~ " << i.ss << endl;
  	}
  	fout << "@End" << endl;
}
int main( int argc, char *argv[] )
{
	//fastio
	ios_base::sync_with_stdio( false );
	cin.tie( NULL );
	cout.tie( NULL );

	if( argc != 3 )
	{
	  	cout << "Enter exactly 2 txt files as input" << endl;
		exit( 0 );
	}
	StableRoommates S( argv[1], argv[2] );


	/*
	Responisble for taking names one by one from the start of the text file.
	Names are to be seperated with commas and end with semicolon
	Outputs errors if there is any inconsistency
	Names cannot also contain , ;~
	*/
	S.getNames();


	/*
	Responsible for inputing the preference list after names have been taken
	Stores the preference list in Table and Ranklist in Rank
	Any names not occuring in this are considered unacceptable
	The Preference list need not be consistent A can have B on its list but the reverse need not be true
	Names are to be seperated with commas and end with semicolon
	Outputs errors if there is any inconsistency
	Names cannot also contain , ;~
	*/
	S.getPreferences();

	/*
	Executes Phase 1 of the algorithm
	set all men as free
	one by one make them propose to each other and hold while deleting blocking pairs symmetrically
	Deletions are accomplished by making string empty as
	a vector delete would also need us to modify the rank table and would have an O( n^3 ) bound practically slower.
	*/
	S.phase1();

	S.phase2();
	S.print();
	return 0;
}