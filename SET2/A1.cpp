#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <climits>

// Определение структуры для ребра графа
class Edge {
public:
    int start;
    int end;
    int cost;
    Edge() : start(-1), end(-1), cost(0) {}
    Edge(int start, int end, int cost) : start(start), end(end), cost(cost) {}

    bool operator==(const Edge& other) const {
        return start == other.start && end == other.end && cost == other.cost;
    }
};

// Класс для представления графа
class Graph {
public:
    std::vector<std::vector<Edge>> adjacencyList;
    std::vector<Edge> allEdges;
    std::vector<int> allVertices;
};

// Класс для реализации структуры Union-Find
class DisjointSets {
public:
    std::vector<int> leader;
    std::vector<int> rank;

    DisjointSets(int vertexCount): leader(vertexCount), rank(vertexCount, 0) {
        for(int i = 0; i < vertexCount; ++i) {
            leader[i] = i; // Изначально каждая вершина ссылается сама на себя
        }
    }

    // Находим "лидера" для вершины
    int find(int vertex) {
        if (vertex != leader[vertex])
            leader[vertex] = find(leader[vertex]);
        return leader[vertex];
    }

    // Объединение двух множеств по вершинам
    bool unionSets(int vertex1, int vertex2) {
        int leader1 = find(vertex1);
        int leader2 = find(vertex2);
        if (leader1 != leader2) {
            if (rank[leader1] < rank[leader2])
                std::swap(leader1, leader2);
            leader[leader2] = leader1;
            if (rank[leader1] == rank[leader2])
                rank[leader1]++;
            return true;
        }
        return false; // Нахождение в одном множестве указывает на цикл
    }
};

// Сравнение рёбер по весу
bool compareEdges(const Edge& edge1, const Edge& edge2) {
    return edge1.cost < edge2.cost;
}

// Хэш-функция для пары значений
struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

// Специализация хэш-функции для ребра
template<>
struct std::hash<Edge> {
    std::size_t operator()(const Edge& edge) const {
        return std::hash<int>()(edge.start) ^ std::hash<int>()(edge.end) ^ std::hash<int>()(edge.cost);
    }
};

// Функция для проверки связности графа после удаления ребра
bool isConnectedAfterRemoval(const Graph& graph, const std::unordered_set<Edge>& remainingEdges, int start, int end) {
    // Здесь должна быть реализация проверки связности, например, через BFS
    return true; // Текущая реализация - заглушка
}

// Функция для поиска цикла начиная с заданной вершины
std::pair<int, Edge> findCycle(int currentVertex, int parentVertex, const Graph& graph, 
                               const std::unordered_set<Edge>& remainingEdges, std::vector<bool>& visited) {
    if (visited[currentVertex]) {
        return {currentVertex, Edge()};
    }
    visited[currentVertex] = true;
    for (const auto& edge : graph.adjacencyList[currentVertex]) {
        if (remainingEdges.count(edge) && edge.end != parentVertex) {
            auto cycleCheck = findCycle(edge.end, edge.start, graph, remainingEdges, visited);
            if (cycleCheck.first != -1) {
                return {cycleCheck.first, cycleCheck.second.cost > edge.cost ? cycleCheck.second : edge};
            }
        }
    }
    return {-1, Edge()}; // Если цикл не найден
}

// Алгоритм для построения остовного дерева без создания циклов
std::unordered_set<Edge> buildSpanningTree(Graph& graph) {
    // Сортируем все рёбра графа по весу
    std::vector<Edge> sortedEdges = graph.allEdges;
    std::sort(sortedEdges.begin(), sortedEdges.end(), compareEdges);

    // Инициализация множества для результата
    std::unordered_set<Edge> spanningTree;

    // Обход всех отсортированных рёбер
    for (Edge& edge : sortedEdges) {
        if (isConnectedAfterRemoval(graph, spanningTree, edge.start, edge.end)) {
            spanningTree.insert(edge);
        }
    }

    return spanningTree;
}

// Алгоритм выбора рёбер для построения остова с использованием случайности
std::vector<Edge> randomSpanningTree(Graph& graph) {
    std::vector<Edge> selectedEdges;
    size_t edgeCount = graph.allEdges.size();
    if (edgeCount == 0) {
        return selectedEdges; // Если рёбер нет, возвращаем пустой список
    }

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, edgeCount - 1);

    // Массив для отслеживания посещенных рёбер
    std::vector<bool> visitedEdges(edgeCount, false);
    size_t visitedCount = 0;

    // Инициализация структуры для объединения множеств
    DisjointSets sets(graph.allVertices.size());

    // Пока не посетим все рёбра
    while (visitedCount < edgeCount) {
        size_t index = distribution(gen);
        if (!visitedEdges[index]) {
            visitedEdges[index] = true;
            ++visitedCount;
            Edge& edge = graph.allEdges[index];
            // Добавляем ребро, если оно не создает цикл
            if (sets.unionSets(edge.start, edge.end)) {
                selectedEdges.push_back(edge);
            }
        }
    }

    return selectedEdges;
}

// Алгоритм для построения остовного дерева с удалением максимальных рёбер циклов
std::unordered_set<Edge> minSpanningTree(Graph& graph) {
    std::unordered_set<Edge> result;
    size_t edgeCount = graph.allEdges.size();
    size_t vertexCount = graph.allVertices.size();

    if (edgeCount == 0) {
        return result; // Если рёбер нет, возвращаем пустое множество
    }

    // Инициализация структуры для объединения множеств
    DisjointSets sets(vertexCount);

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, edgeCount - 1);

    // Массив для отслеживания посещенных рёбер
    std::vector<bool> visitedEdges(edgeCount, false);
    size_t visitedCount = 0;

    std::vector<Edge> allEdges(graph.allEdges.begin(), graph.allEdges.end());

    // Пока не посетим все рёбра
    while (visitedCount < edgeCount) {
        size_t index = distribution(gen);
        if (!visitedEdges[index]) {
            visitedEdges[index] = true;
            ++visitedCount;
            Edge& edge = allEdges[index];
            result.insert(edge);
            if (!sets.unionSets(edge.start, edge.end)) {
                std::vector<bool> visitedVertices(vertexCount, false);                 // Если ребро создаёт цикл, находим максимальное ребро в этом цикле.
                Edge maxEdgeInCycle = findCycle(edge.start, -1, graph, result, visitedVertices).second;
                result.erase(maxEdgeInCycle);                                          // Удаляем максимальное ребро из цикла
            }
        }
    }

    return result;
}
