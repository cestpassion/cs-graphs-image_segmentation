#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>

/**
 * Fila de prioridade minima usada pela Image Foresting Transform.
 *
 * A IFT propaga caminhos otimos em ordem nao-decrescente de custo (estrategia
 * tipo Dijkstra). Esta fila entrega sempre o pixel de menor custo ainda nao
 * processado.
 *
 * Detalhe de implementacao: nao removemos entradas obsoletas da heap quando o
 * custo de um pixel melhora. Em vez disso, inserimos uma nova entrada com o
 * custo menor e, ao desempilhar, descartamos qualquer entrada cujo custo nao
 * corresponda mais ao melhor custo conhecido do pixel (lazy deletion). Isso
 * mantem o codigo simples sem precisar de uma heap com "decrease-key".
 */
class PriorityQueue {
public:
    struct Item {
        double cost;  // custo do caminho ate o pixel no momento da insercao
        int vertex;   // id do pixel (id = y * width + x)
    };

    /** Insere (ou reinsere) um pixel com o custo informado. */
    void push(double cost, int vertex);

    /** Indica se nao ha mais itens validos a processar. */
    bool empty() const;

    /**
     * Remove e devolve o pixel de menor custo ainda valido.
     *
     * "current" deve apontar para o vetor de custos correntes (cost[]). Entradas
     * obsoletas (cujo custo guardado e maior que o custo atual do pixel) sao
     * descartadas automaticamente.
     */
    Item pop(const std::vector<double>& current);

private:
    struct Compare {
        bool operator()(const Item& a, const Item& b) const {
            return a.cost > b.cost;  // menor custo no topo
        }
    };

    std::priority_queue<Item, std::vector<Item>, Compare> heap;
};

#endif  // PRIORITY_QUEUE_HPP
