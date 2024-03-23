import networkx as nx

def disconnect_nodes(graph, source, target) -> int:
    """
    Функция вычисляет минимальное количество вершин, которые необходимо удалить,
    чтобы разорвать связь между двумя узлами в направленном графе.

    :param graph: Исходный граф.
    :param source: Начальный узел.
    :param target: Конечный узел.
    :return: Минимальное количество вершин, которые необходимо удалить.
    """
    # Проверяем, являются ли source и target соседями
    if target in graph[source]:
        return -1

    # Создаем копию графа с единичными емкостями ребер для поиска максимального потока
    graph_copy = nx.DiGraph()
    for edge in graph.edges():
        graph_copy.add_edge(edge[0], edge[1], capacity=1)

    # Вычисляем максимальное значение потока между source и target
    max_flow_value = nx.maximum_flow_value(graph_copy, source, target)

    return max_flow_value

# Примеры использования
graph1 = nx.DiGraph()
graph1.add_edge(1, 2)
graph1.add_edge(1, 3)
graph1.add_edge(2, 3)
graph1.add_edge(2, 4)
graph1.add_edge(3, 4)
print(f"Необходимо удалить {disconnect_nodes(graph1, 1, 4)} вершин, чтобы разорвать связь между 1 и 4")

graph2 = nx.DiGraph()
graph2.add_edge('S', 'A', capacity=3)
graph2.add_edge('S', 'B', capacity=2)
graph2.add_edge('A', 'C', capacity=3)
graph2.add_edge('A', 'T', capacity=2)
graph2.add_edge('B', 'T', capacity=3)
graph2.add_edge('C', 'B', capacity=1)
graph2.add_edge('C', 'T', capacity=1)
print(f"Необходимо удалить {disconnect_nodes(graph2, 'S', 'T')} вершин, чтобы разорвать связь между S и T")
