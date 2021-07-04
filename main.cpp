#include <vector>
#include <iostream>
#include <algorithm>

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
        vector<int> ants;

        que.push_back(r);

        int qs = 0;

        int shortest_distance = -1;

        while(qs<que.size()){

            distance = g[que[qs]].d + 1;
            parent = que[qs];

            // bfs przetwarza wierzchołek na następnym poziomie
            // zapisz odległość jaką mają pokonać mrówki na niższym poziomie

            if(qs>0 && g[que[qs-1]].d < g[que[qs]].d  ){

                sort(ants.begin(); ants.end());

                for(auto ant = ants.begin(); ant!=ants.end(); ant++){

                    // zapisz przebytą odległość na ścieżce

                    int ve_on_path = m[*ant].v;
                    int path_distance = 0;

                    while(ve_on_path!=-1){


                        if(path_distance > shortest_distance){

                            path_distance = shortest_distance-1;

                        }


                        // jeśli wierzchołek jest odwiedzony przez mrówkę we wcześniejszym czasie i ścieżka nie została skrócona
                        // ścieżki są skracane o dystans mrówki która dotarła tam wcześniej

                        if(g[ve_on_path].path_distance && m[g[ve_on_path].m_on_path].mov >= g[ve_on_path].path_distance ){

                            // skróć ściężkę o dystans mrówki która dotarła tam wcześniej

                            path_distance = g[ve_on_path].path_distance - 1;

                            break;

                        }

                        g[ve_on_path].path_distance = path_distance;
                        g[ve_on_path].m_on_path = *ant;

                        path_distance++;
                        ve_on_path = g[ve_on_path].p;
                    }

                    if(ve_on_path==-1)
                        path_distance--;

                    m[*ant].mov = path_distance;

                }

                ants.clear();

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

    }

    void WriteBfs(){

        for(int x = 0; x<g.size(); x++){

            cout<<x<<": "<<"   D: "<<g[x].d<<"   P: "<<g[x].p<<endl;

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

    }

}
