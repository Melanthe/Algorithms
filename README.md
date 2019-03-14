# Algorithms

Task 0.0

Дано бинарное поисковое дерево, в котором ключи вершин являются целыми числами в диапазоне от −231 до 231 − 1 включительно. 
Найдите сумму ключей всех вершин дереве.

Task 0.1

По набору ключей постройте бинарное поисковое дерево и выполните его прямой левый обход.

Task 0.2

По набору ключей постройте бинарное поисковое дерево. Удалите из него ключ (правым удалением), если он есть в дереве. 
Выполните прямой левый обход полученного дерева.

Task 1

Найти вершины, через которые проходит наибольшее число наибольших (по длине) полупутей, и удалить их (правым удалением) обратным левым обходом.

Task 2

Существует игра для одного игрока, которая начинается с задания Полигона с n вершинами. Для каждой вершины Полигона задаётся значение — целое число, а для каждого ребра — метка операции + (сложение) либо × (умножение). Ребра Полигона пронумерованы от 1 до n.
Первым ходом в игре удаляется одно из рёбер. 
Каждый последующий ход состоит из следующих шагов:
1.выбирается ребро e и две вершины v1 и v2, которые соединены ребром e;
2.ребро e стягивается, то есть вершины v1 и v2 удаляются и заменяются новой вершиной v со значением, равным результату выполнения операции, определённой меткой ребра e, над значениями вершин v1 и v2. В рёбрах, отличных от e и инцидентных вершинам v1 и v2, эти две вершины заменяются на вершину v.
Игра заканчивается, когда больше нет ни одного ребра. Результат игры — это число, равное значению оставшейся вершины.
Напишите программу, которая по заданному Полигону, вычисляет максимальное значение оставшейся вершины и выводит список всех тех рёбер, удаление которых на первом ходу игры позволяет получить это значение.

Формат входных данных
В первой строке записано число n (3 ≤ n ≤ 500). Вторая строка содержит метки рёбер с номерами 1, …, n, между которыми записаны через пробел значения вершин (первое из них соответствует вершине, инцидентной рёбрам 1 и 2, следующее — инцидентной рёбрам 2 и 3, и так далее, последнее — инцидентной рёбрам n и 1). Метка ребра — это буква t, соответствующая операции +, или буква x, соответствующая операции ×.
Гарантируется, что при любой последовательности ходов значения вершин находятся в пределах от −9 223 372 036 854 775 808 до 9 223 372 036 854 775 807.

Формат выходных данных
В первой строке выведите максимальное значение оставшейся вершины, которое может быть достигнуто для заданного Полигона. Во второй строке — список всех тех рёбер, удаление которых на первом ходу, позволяет получить это значение. Номера рёбер должны быть записаны в возрастающем порядке и отделяться друг от друга одним пробелом.
