#include<bits/stdc++.h>
using namespace std;
#define n 100
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

	fout<<"@PreferenceLists"<<endl;
	For(i,n){
		std::vector<int> v;
		For(j,n){
			v.push_back((i+j)%n);
		}
		random_shuffle(v.begin()+1,v.end());
		int sz=rand()%(n-2);
		sz++;
		sz++;
		fout<<v[0]<<":";
		For(j,sz-2){
			fout<<v[j+1]<<",";
		}
		fout<<v[sz-1]<<" ;"<<endl;
	}
	fout<<"@End"<<endl;
}
