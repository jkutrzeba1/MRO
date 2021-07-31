#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

struct Graph {

	struct Ed {

		int v;
		int r;

		Ed(int v, int r = -1) : v(v), r(r) {}

	};

	struct Ve : vector<Ed> {
		int d;
		int p;
		int m;
		int path_distance;
		int m_on_path;

		Ve() : d(-1), p(-1), m(-1), path_distance(-1), m_on_path(-1) {}
	};

	struct M {
		int v; //indeks wierzchołka gdzie znajduje się mrówka
		int c; //ile razy mrówka przegoniła biedronkę
		int mov; //odległość jaką pokona mrówka aby przegonić biedronkę
		M(int v) : v(v), c(0), mov(0) {}
	};

	vector<Ve> g;
	vector<M> m;

	vector<int> ants;

	Graph(int n = 0) : g(n) {}

	void EdgeD(int b, int e){

		g[b].push_back(Ed(e));

	}

	void EdgeU(int b, int e){

		g[b].push_back(Ed(e, g[e].size() + (b==e) ));
		g[e].push_back(Ed(b, g[b].size()-1 ));

	}

	void Write(){

		for(int x = 0; x<g.size(); x++){

			cout<<x<<": ";

			for(auto ed = g[x].begin(); ed!=g[x].end(); ed++){
				cout<<ed->v<<" ";
			}

			cout<<endl;

		}

	}

	void Bfs(int r){

		// biedronka ląduje na wierzchołku gdzie aktualnie znajduje się mrówka
		if(g[r].m != -1){
			m[g[r].m].c++;
			return;
		}


		int distance = g[r].d = 0;
		int parent;

		vector<int> que;

		que.push_back(r);

		int qs = 0;

		int shortest_distance = -1;
		int m_idx_first = -1;

		while(qs<que.size()){

			distance = g[que[qs]].d + 1;
			parent = que[qs];

			// bfs przetwarza wierzchołek na następnym poziomie
			// zapisz odległość jaką mają pokonać mrówki na niższym poziomie

			if(qs>0 && g[que[qs-1]].d < g[que[qs]].d  ){

				SaveAntMoves(shortest_distance, m_idx_first, g[que[qs]].d);

			}

			for(auto ed = g[que[qs]].begin(); ed!=g[que[qs]].end(); ed++){

				if( g[ed->v].d != -1 ){
					continue;
				}

				g[ed->v].d = distance;
				g[ed->v].p = parent;


				// na wierzchołku do którego prowadzi krawędź znajduje się mrówka

				int m_idx = g[ed->v].m;

				if( m_idx != -1 ){

					ants.push_back(m_idx);

					if(shortest_distance==-1){
						shortest_distance = distance;
					}

				}

				// jeśli na wierzchołku nie ma mrówki to dodaj wierzchołek do kolejki
				// mrówki położone dalej na ścieżce nie wykonają ruchu, dlatego dalsze wierzchołki nie są odwiedzane
				else{
					que.push_back(ed->v);
				}

			}

			qs++;

		}

		// jeśli w buforze są mrówki których dystans nie został ustalony to oblicz ich dystans
		// będą to mrówki położone na najdalszych wierzchołkach

		SaveAntMoves(shortest_distance, m_idx_first, g[que[qs-1]].d);

	}

	void SaveAntMoves(int shortest_distance, int &m_idx_first, int distance){

		sort(ants.begin(), ants.end());

		for(auto ant = ants.begin(); ant!=ants.end(); ant++){

			// zapisz przebytą odległość na ścieżce

			int ve_on_path = m[*ant].v;
			int path_distance = 0;

			while(ve_on_path!=-1){


				if(path_distance == shortest_distance && m_idx_first == -1){

					m_idx_first = *ant;

					m[m_idx_first].c++;

					path_distance = shortest_distance;

					g[ve_on_path].path_distance = path_distance;
					g[ve_on_path].m_on_path = *ant;

					break;

				}


				// jeśli wierzchołek jest odwiedzony przez mrówkę we wcześniejszym czasie i ścieżka nie została skrócona

				if(g[ve_on_path].path_distance != -1 && m[g[ve_on_path].m_on_path].mov >= g[ve_on_path].path_distance ){

					// przypisz dystans tego wierzchołka do dystansu jaki ma pokonać mrówka

					path_distance = g[ve_on_path].path_distance;

					// jeśli odległość mrówek od miejsca lądowania biedronki jest taka sama, skróć ten dystans o 1.

					if(distance == g[m[g[ve_on_path].m_on_path].v].d){
						path_distance--;
					}

					break;

				}

				if(path_distance == shortest_distance){

					path_distance = shortest_distance;

					g[ve_on_path].path_distance = path_distance;
					g[ve_on_path].m_on_path = *ant;

					break;

				}

				g[ve_on_path].path_distance = path_distance;
				g[ve_on_path].m_on_path = *ant;

				path_distance++;
				ve_on_path = g[ve_on_path].p;

			}

			// dotarcie do korzenia -
			if(ve_on_path==-1)
				path_distance--;

			m[*ant].mov = path_distance;

		}

		ants.clear();

	}

	void AntMoves(){

		for(auto ant = m.begin(); ant!=m.end(); ant++){

			int m_idx = ant-m.begin();

			int vertice_before_move_idx = ant->v;
			int vertice_after_move_idx = ant->v;

			for(int x=0; x < ant->mov; x++){

				vertice_after_move_idx = g[vertice_after_move_idx].p;

			}

			ant->v = vertice_after_move_idx;

			g[vertice_before_move_idx].m = -1;
			g[vertice_after_move_idx].m = m_idx;

		}

	}

	void ClearBfsMeta(){

		for(auto ant = m.begin(); ant!=m.end(); ant++){

			ant->mov = 0;

		}

		for(auto vertice = g.begin(); vertice!=g.end(); vertice++){

			vertice->m_on_path = -1;
			vertice->path_distance = -1;
			vertice->d = -1;
			vertice->p = -1;

		}

	}

	void WriteBfs(){

		for(int x = 0; x<g.size(); x++){

			cout<<x<<": "<<"   D: "<<g[x].d<<"   P: "<<g[x].p<<endl;

		}

	}


	void WriteAnts(){

		for(auto ant = m.begin(); ant!=m.end(); ant++){

			cout<<(ant->v)+1<<" "<<ant->c<<endl;

		}

	}

};

int main(){

	int n; // liczba liści i rozgałęzień ponumerowanych od 1 do n
	cin>>n;

	Graph g(n);

	for(int x = 1; x<n; x++){
		int a,b;
		cin>>a;
		cin>>b;
		g.EdgeU(a-1,b-1);
	}

	int k; //liczba mrówek patrolujących drzewo ponumerowane od 1 do k
	cin>>k;
	for(int x = 1; x<=k; x++){
		int v; //numer wierzchołka gdzie znajduje się mrówka
		cin>>v;
		g.m.push_back(Graph::M(v-1));
		g.g[v-1].m = x-1;
	}

	int l; // ilość lądowań biedronki
	cin>>l;

	for(int x = 1; x<=l; x++ ){

		int n; // wierzchołek w którym ląduje biedronka
		cin>>n;

		g.Bfs(--n);
		g.AntMoves();
		g.ClearBfsMeta();

	}

	g.WriteAnts();

}
