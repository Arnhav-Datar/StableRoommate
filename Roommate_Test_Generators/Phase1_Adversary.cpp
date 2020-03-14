#include<bits/stdc++.h>
using namespace std;
#define n 1000
#define For(i,n) for(int i=0;i<(int)n;i++)
main(int argc, char *agrv[]){
	ios_base::sync_with_stdio(false);cin.tie(NULL);
  	cout.tie(NULL);srand(time(0));
	
	fstream fout;
	fout.open(agrv[1], ios::out);

	fout<<"@Names"<<endl;
	For(i,n-1) fout<<i<<",";
	fout<<n-1<<";"<<endl;
	fout<<"@End"<<endl<<endl;
	set<int> complete;
	For(i,n) complete.insert(i);
	fout<<"@PreferenceLists"<<endl;
	For(i,n)
	{
		fout<<i<<":";
		complete.erase(i);
		std::vector<int> v;
		for(auto x:complete){
			v.push_back(x);
		}
		For(j,n-2)
			fout<<v[j]<<",";
		fout<<v[n-2]<<" ;"<<endl;
		complete.insert(i);
	}
	fout<<"@End"<<endl;
}
