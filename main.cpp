#include <vector>
#include <iostream>

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
        bool path;

        Ve() : d(-1), p(-1), m(-1), path(false) {}
    };

    struct M {
        int v; //indeks wierzchołka gdzie znajduje się mrówka
        int c; //ile razy mrówka przegoniła biedronkę
        int vt; //indeks wierzchołka gdzie mrówka zakończy wędrówkę
        M(int v) : v(v), c(0) {}
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

        que.push_back(r);

        int qs = 0;

        while(qs<que.size()){

            distance = g[que[qs]].d + 1;
            parent = que[qs];

            for(auto ed = g[que[qs]].begin(); ed!=g[que[qs]].end(); ed++){

                if( g[ed->v].d != -1 ){
                    continue;
                }

                g[ed->v].d = distance;
                g[ed->v].p = parent;

                // na wierzchołku do którego prowadzi krawędź znajduje się mrówka
                if( g[ed->v].m != -1 ){

                    // ustaw wierzchołki jako te które znajdują się na ścieżce

                    int parent_on_path = parent;
                    int child_on_path = ed->v;

                    while(parent_on_path!=-1){

                        if(!g[parent_on_path].path){
                            g[parent_on_path].path = true;
                        }
                        else{
                            m[g[ed->v].m].vt = child_on_path;
                            break;
                        }

                        child_on_path = parent_on_path;
                        parent_on_path = g[parent_on_path].p;

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
        g[v-1].m = k-1;
    }

    int l; // ilość lądowań biedronki
    cin>>l;

    for(int x = 1; x<=l; x++ ){

        int n; // wierzchołek w którym ląduje biedronka
        cin>>n;

        g.Bfs(--n);

    }

}
