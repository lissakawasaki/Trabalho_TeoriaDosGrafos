#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>

using namespace std;

struct Node {
    int valor;
    vector<Node*> vizinhos; // Lista de vizinhos
    
    Node(int val) : valor(val) {}
};

class Grafo {
public:
    map<int, Node*> node; 

    Grafo() {} 
    void Inserir(int noSuperior, int noInferior);
    void BuscaLargura(int inicio, vector<int>& listaAcesso, vector<int>& percurso, int objetivo);
    bool Busca(int inicio, int objetivo, vector<int>& caminho);
};

void Grafo::Inserir(int noSuperior, int noInferior) {
    if (node.find(noSuperior) == node.end()) {
        node[noSuperior] = new Node(noSuperior);
    }
    if (node.find(noInferior) == node.end()) {
        node[noInferior] = new Node(noInferior);
    }

    node[noSuperior]->vizinhos.push_back(node[noInferior]);
}

void Grafo::BuscaLargura(int inicio, vector<int>& listaAcesso, vector<int>& percurso, int objetivo) {
    if (node.find(inicio) == node.end()) return;

    queue<Node*> fila;
    map<int, bool> visitado;

    fila.push(node[inicio]);
    visitado[inicio] = true;

    while (!fila.empty()) {
        Node* atual = fila.front();
        fila.pop();

        listaAcesso.push_back(atual->valor);
        percurso.push_back(atual->valor); 

        if (atual->valor == objetivo){
            break;
        }

        for (Node* vizinho : atual->vizinhos) {
            if (!visitado[vizinho->valor]) {
                fila.push(vizinho);
                visitado[vizinho->valor] = true;
            }
        }
    }
}

bool Grafo::Busca(int inicio, int objetivo, vector<int>& caminho) {
    if (node.find(inicio) == node.end()) {
        return false;
    }

    queue<Node*> fila;
    map<int, bool> visitado;
    map<int, int> anterior; // Para reconstrução do caminho

    fila.push(node[inicio]);
    visitado[inicio] = true;
    anterior[inicio] = -1; // O nó inicial não tem um nó anterior

    while (!fila.empty()) {
        Node* atual = fila.front();
        fila.pop();

        if (atual->valor == objetivo) {
            int temp = objetivo;
            while (temp != -1) {
                caminho.insert(caminho.begin(), temp);
                temp = anterior[temp];
            }
            return true;
        }

        for (Node* vizinho : atual->vizinhos) {
            if (!visitado[vizinho->valor]) {
                fila.push(vizinho);
                visitado[vizinho->valor] = true;
                anterior[vizinho->valor] = atual->valor; // Registra quem veio antes
            }
        }
    }

    return false;
}

// Função principal
int main() {

    cout << "Trabalho de Teoria dos Grafos - Lissa Guirau Kawasaki - Busca Cega por Largura\n";
    
    Grafo grafo;
    int noSuperior, noInferior;

    cout << "Digite as conexões (noSuperior noInferior). Digite 0 0 para encerrar:\n";
    
    while (true) {
        cin >> noSuperior;
        
        if (cin.fail()) { // Caso o usuário insira algo inválido
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida! Tente novamente.\n";
            continue;
        }

        cin >> noInferior;
        if (noSuperior == 0 && noInferior == 0) break;

        grafo.Inserir(noSuperior, noInferior);
    }

    int inicio, objetivo;
    cout << "\nDigite o nó inicial para a busca: ";
    cin >> inicio;

    if (grafo.node.find(inicio) == grafo.node.end()) {
        cout << "Erro: O nó inicial não existe no grafo.\n";
        return 1;
    }

    cout << "Digite o nó objetivo para buscar: ";
    cin >> objetivo;

    vector<int> listaAcesso;
    vector<int> percurso;
    vector<int> caminho;

    grafo.BuscaLargura(inicio, listaAcesso, percurso, objetivo);

    cout << "\nLista de acessos (todos os nós visitados): ";
    for (int val : listaAcesso) cout << val << " ";
    cout << endl;

    cout << "Percurso (ordem da busca em largura): ";
    for (int val : percurso) cout << val << " ";
    cout << endl;

    if (grafo.Busca(inicio, objetivo, caminho)) {
        cout << "Caminho até o objetivo: ";
        for (int val : caminho) cout << val << " ";
        cout << endl;
    } else {
        cout << "Valor " << objetivo << " não encontrado no grafo.\n";
    }

    cout << "Programa encerrado!\n";
    return 0;
}