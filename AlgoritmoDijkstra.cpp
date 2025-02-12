#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;

struct Vertice {
    int valor;
    double distanciaMinima;
    Vertice* anterior;
    vector<pair<Vertice*, double>> vizinhos; // Lista de vizinhos com o peso da aresta

    Vertice(int val) : valor(val), distanciaMinima(numeric_limits<double>::infinity()), anterior(nullptr) {}
    
    // Comparador para fila de prioridade (min-heap)
    bool operator>(const Vertice& other) const {
        return distanciaMinima > other.distanciaMinima;
    }
};

class Grafo {
public:
    map<int, Vertice*> vertices;

    Grafo() {}
    void Inserir(int origem, int destino, double peso);
    void Dijkstra(Vertice* origem);
    vector<Vertice*> getMenorCaminho(Vertice* destino);
};

void Grafo::Inserir(int origem, int destino, double peso) {
    if (vertices.find(origem) == vertices.end()) {
        vertices[origem] = new Vertice(origem);
    }
    if (vertices.find(destino) == vertices.end()) {
        vertices[destino] = new Vertice(destino);
    }

    vertices[origem]->vizinhos.push_back({vertices[destino], peso});
    vertices[destino]->vizinhos.push_back({vertices[origem], peso}); // Grafo não direcionado
}

void Grafo::Dijkstra(Vertice* origem) {
    origem->distanciaMinima = 0;
    priority_queue<Vertice*, vector<Vertice*>, greater<Vertice*>> filaPrioridade;
    filaPrioridade.push(origem);

    while (!filaPrioridade.empty()) {
        Vertice* atual = filaPrioridade.top();
        filaPrioridade.pop();

        // Para cada vizinho do vértice atual, verifica se a distância pode ser atualizada
        for (auto& vizinho : atual->vizinhos) {
            Vertice* v = vizinho.first;
            double custo = vizinho.second;
            double novaDistancia = atual->distanciaMinima + custo;

            if (novaDistancia < v->distanciaMinima) {
                v->distanciaMinima = novaDistancia;
                v->anterior = atual;
                filaPrioridade.push(v);
            }
        }
    }
}

vector<Vertice*> Grafo::getMenorCaminho(Vertice* destino) {
    vector<Vertice*> caminho;
    for (Vertice* v = destino; v != nullptr; v = v->anterior) {
        caminho.push_back(v);
    }
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

int main() {
    cout << "Algoritmo de Dijkstra - Teoria dos Grafos\n";
    
    Grafo grafo;
    int noSuperior, noInferior;
    double peso;

    cout << "Digite as conexões (noSuperior noInferior peso). Digite 0 0 0 para encerrar:\n";
    
    while (true) {
        cin >> noSuperior;
        
        if (cin.fail()) { // Caso o usuário insira algo inválido
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida! Tente novamente.\n";
            continue;
        }

        cin >> noInferior >> peso;
        if (noSuperior == 0 && noInferior == 0) break;

        grafo.Inserir(noSuperior, noInferior, peso);
    }

    int inicio, objetivo;
    cout << "\nDigite o nó inicial para a busca: ";
    cin >> inicio;

    Vertice* verticeInicio = grafo.vertices[inicio];
    if (verticeInicio == nullptr) {
        cout << "Erro: O nó inicial não existe no grafo.\n";
        return 1;
    }

    grafo.Dijkstra(verticeInicio);

    cout << "Digite o nó objetivo para buscar: ";
    cin >> objetivo;

    Vertice* verticeDestino = grafo.vertices[objetivo];
    if (verticeDestino == nullptr) {
        cout << "Erro: O nó objetivo não existe no grafo.\n";
        return 1;
    }

    vector<Vertice*> caminho = grafo.getMenorCaminho(verticeDestino);

    cout << "\nMenor caminho de " << verticeInicio->valor << " até " << verticeDestino->valor << ": ";
    for (Vertice* v : caminho) {
        cout << v->valor << " ";
    }
    cout << "\nDistância mínima: " << verticeDestino->distanciaMinima << endl;

    return 0;
}
