def reconstruct_graph(distance_matrix):
    """
    Восстанавливает список смежности графа из заданной матрицы расстояний.
    Он проверяет наличие прямых ребер между каждой парой вершин, гарантируя, что
    прямое расстояние не равно сумме любых двух промежуточных расстояний.

    :параметр distance_matrix: Двумерный список, представляющий кратчайшие расстояния между вершинами
    :return: Словарь, представляющий список смежности графа
    """
    num_vertices = len(distance_matrix)
    adjacency_list = {vertex: [] for vertex in range(num_vertices)}

    for src in range(num_vertices):
        for dest in range(num_vertices):
            # Исключить самостоятельные петли и несуществующие ребра
            if src != dest and distance_matrix[src][dest] != float('inf'):
                direct_edge = True
                # Проверить, равно ли прямое расстояние любой сумме двух промежуточных расстояний
                for intermediate in range(num_vertices):
                    if (intermediate != src and intermediate != dest and
                            distance_matrix[src][dest] == distance_matrix[src][intermediate] + distance_matrix[intermediate][dest]):
                        direct_edge = False
                        break
                # Если существует прямое ребро, добавьте его в список смежности
                if direct_edge:
                    adjacency_list[src].append((dest, distance_matrix[src][dest]))

    return adjacency_list


# Пример
example_dist_matrix = [
    [0, 2, 3],
    [float('inf'), 0, 1],
    [float('inf'), float('inf'), 0]
]
reconstructed_graph = reconstruct_graph(example_dist_matrix)
print(reconstructed_graph)
