#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <windows.h>

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
    void BuscaProfundidade(int inicio, vector<int>& listaAcesso, vector<int>& percurso);
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

void Grafo::BuscaProfundidade(int inicio, vector<int>& listaAcesso, vector<int>& percurso) {
    if (node.find(inicio) == node.end()) return;

    map<int, bool> visitado;
    vector<Node*> pilha;
    vector<int> tempPercurso;  // Pilha auxiliar para inverter a ordem

    pilha.push_back(node[inicio]);
    visitado[inicio] = true;

    while (!pilha.empty()) {
        Node* atual = pilha.back();
        pilha.pop_back();

        listaAcesso.push_back(atual->valor);  
        tempPercurso.push_back(atual->valor);  // Armazena em ordem para inverter depois

        // Ordenando os vizinhos de forma reversa (maiores valores primeiro)
        vector<Node*> vizinhosOrdenados = atual->vizinhos;
        sort(vizinhosOrdenados.rbegin(), vizinhosOrdenados.rend(), [](Node* a, Node* b) {
            return a->valor < b->valor;  // Ordenação decrescente
        });

        for (Node* vizinho : vizinhosOrdenados) {
            if (!visitado[vizinho->valor]) {
                pilha.push_back(vizinho);
                visitado[vizinho->valor] = true;
            }
        }
    }

    // Inverter a ordem para armazenar os mais profundos primeiro
    percurso.assign(tempPercurso.rbegin(), tempPercurso.rend());
}


bool Grafo::Busca(int inicio, int objetivo, vector<int>& caminho) {
    if (node.find(inicio) == node.end()) {
        return false;
    }

    map<int, bool> visitado;
    map<int, int> anterior; 

    // Pilha para a busca por profundidade
    vector<Node*> pilha;
    pilha.push_back(node[inicio]);
    visitado[inicio] = true;
    anterior[inicio] = -1; 

    while (!pilha.empty()) {
        Node* atual = pilha.back();
        pilha.pop_back();

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
                pilha.push_back(vizinho);
                visitado[vizinho->valor] = true;
                anterior[vizinho->valor] = atual->valor; // Registra quem veio antes
            }
        }
    }

    return false;
}

int main() {

    SetConsoleOutputCP(65001);

    cout << "\t\tTrabalho de Teoria dos Grafos\n";
    cout << "\tBusca Cega por Profundidade - Lissa Guirau Kawasaki\n\n";
    
    Grafo grafo;
    int noSuperior, noInferior;

    cout << "Digite as conexões do grafo (nóSuperior nóInferior).\n";
    cout << "Para encerrar a entrada, digite '0 0'.\n";
    
    while (true) {
        cout << "Conexão (ex: 1 2): ";
        cin >> noSuperior >> noInferior;

        if (cin.fail()) { // Caso o usuário insira algo inválido
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida! Tente novamente.\n";
            continue;
        }

        if (noSuperior == 0 && noInferior == 0) break;
        if (noSuperior < 0 || noInferior < 0) {
            cout << "Os valores devem ser positivos.\n";
            continue;
        }
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

    grafo.BuscaProfundidade(inicio, listaAcesso, percurso);

    cout << "\nLista de acessos (todos os nós visitados): ";
    for (int val : listaAcesso) cout << val << " ";
    cout << endl;

    cout << "Percurso (ordem da busca em profundidade): ";
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