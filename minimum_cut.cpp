#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

const string input_fn = "karate-network.dat";
const string adj_fn = "karate-network.adj";
const string output_fn = "karate-network.mincut";

int nVertices, nEdges, source, sink, total_flow, min_cut;
vector < pair<int, int> > edges;
int **capacity;
int **flow;
int rear, front;
int *queue;
bool *visited;
int *trace;
int *label;

void input() {
	ifstream file(input_fn.c_str(), ios::in);
	file >> nVertices >> nEdges >> source >> sink;
	--source;
	--sink;
	edges.clear();
	for (int i = 0; i < nEdges; ++i) {
		int u, v;
		file >> u >> v;
		--u;
		--v;
		edges.push_back(make_pair(u, v));
	}
	file.close();
}

void create_graph() {
	capacity = new int* [nVertices];
	flow = new int* [nVertices];

	for (int i = 0; i < nVertices; ++i) {
		capacity[i] = new int [nVertices];
		flow[i] = new int [nVertices];
	}

	for (int u = 0; u < nVertices; ++u) {
		for (int v = 0; v < nVertices; ++v) {
			capacity[u][v] = 0;
			flow[u][v] = 0;
		}
	}

	queue = new int [nVertices];
	visited = new bool [nVertices];
	trace = new int [nVertices];
	label = new int [nVertices];

	for (int i = 0; i < nEdges; ++i) {
		int u = edges[i].first;
		int v = edges[i].second;
		capacity[u][v] = 1;
		capacity[v][u] = 1;
	}
}

void output_adj() {
	ofstream file(adj_fn.c_str(), ios::out);
	for (int u = 0; u < nVertices; ++u) {
		for (int v = 0; v < nVertices; ++v) {
			file << capacity[u][v] << " ";
		}
		file << endl;
	}
	file.close();
}

void push(int v) {
	queue[rear] = v;
	++rear;
	visited[v] = true;
}

int pop() {
	int v = queue[front];
	++front;
	return v;
}

bool BFS() {
	for (int v = 0; v < nVertices; ++v) {
		visited[v] = false;
	}

	rear = 0;
	front = 0;
	push(source);

	while (rear != front) {
		int u = pop();
		for (int v = 0; v < nVertices; ++v) {
			if ((!visited[v]) && (capacity[u][v] > flow[u][v])) {
				trace[v] = u;
				push(v);
				if (v == sink) {
					return true;
				}
			}
		}
	}

	return false;
}

void increase_flow() {
	int delta = INF;
	int v = sink;
	while (v != source) {
		int u = trace[v];
		delta = min(delta, capacity[u][v] - flow[u][v]);
		v = u;
	}

	v = sink;
	while (v != source) {
		int u = trace[v];
		flow[u][v] += delta;
		flow[v][u] -= delta;
		v = u;
	}
}

void maximum_flow() {
	while (BFS()) {
		increase_flow();
	}

	for (int v = 0; v < nVertices; ++v) {
		if (visited[v]) {
			label[v] = 1;
		} else {
			label[v] = -1;
		}
	}

	min_cut = 0;
	for (int u = 0; u < nVertices; ++u) {
		for (int v = u + 1; v < nVertices; ++v) {
			if ((label[u] != label[v]) && (capacity[u][v] > 0)) {
				min_cut += capacity[u][v];
			}
		}
	}

	total_flow = 0;
	for (int v = 0; v < nVertices; ++v) {
		total_flow += flow[source][v];
	}

	cout << "Maximum flow: " << total_flow << endl;
	cout << "Minimum cut: " << min_cut << endl;
}

void output() {
	ofstream file(output_fn.c_str(), ios::out);
	for (int v = 0; v < nVertices; ++v) {
		file << label[v] << endl;
	}
	file.close();
}

int main(int argc, char **argv) {
	input();
	create_graph();
	output_adj();
	maximum_flow();
	output();
	return 0;
}