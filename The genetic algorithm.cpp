#include <bits/stdc++.h>

using namespace std;

void parent(vector<int>& a, int b) { //функция для получения родителей 1 поколения. 1 стоит на месте, совпадающем, с номером предмета
    int l = a.size();
    for (int i = 0; i < l; i++) {
        if (i == b - 1) {
            a[i] = 1;
        } else {
            a[i] = 0;
        }
    }
}

int cost(vector<int>& a, vector<int>& v) { //подсчет цены представителя
    int c = 0;
    int l = a.size();
    for (int i = 0; i < l; i++) {
        if (a[i] == 1) {
            c += v[i];
        }
    }
    return c;
}

int weight(vector<int>& a, vector<int>& w) { //подсчет веса представителя
    int c = 0;
    int l = a.size();
    for (int i = 0; i < l; i++) {
        if (a[i] == 1) {
            c += w[i];
        }
    }
    return c;
}

int selection(vector<int>& solution, int C, vector<int>& values) { //функция селекции
    int c = cost(solution, values);
    return (c >= C) ? 1 : 0; 
}

vector<int> two_point_crossover(vector<int>& a, vector<int>& b) { //двухточечный кроссинговер
    int l = a.size();
    vector<int> c(l);
    int k1 = rand() % l; 
    int k2 = rand() % l; 
    if (k1 > k2) swap(k1, k2); 
    for (int i = 0; i < l; i++) {
        if (i < k1 || i >= k2) {
            c[i] = a[i];
        } else {
            c[i] = b[i];
        }
    }
    return c;
}

vector<vector<int>> new_population(vector<vector<int>>& parents, vector<vector<int>>& offspring, int population_size, vector<int>& weights, int max_weight, vector<int>& values) {
    vector<vector<int>> new_pop;
    new_pop.insert(new_pop.end(), parents.begin(), parents.end());
    new_pop.insert(new_pop.end(), offspring.begin(), offspring.end());
    vector<vector<int>> valid_pop; //проверка на максимальный вес
    for (auto& solution : new_pop) {
        if (weight(solution, weights) <= max_weight) {
            valid_pop.push_back(solution);
        }
    }
    sort(valid_pop.begin(), valid_pop.end(), [&](vector<int>& a, vector<int>& b) { //сортировка по стоимости
        return cost(a, values) > cost(b, values);
    });
    while (valid_pop.size() < population_size) {//вдруг не добрали до изначального размера
        if (!offspring.empty()) { 
            int random_index = rand() % offspring.size(); 
            valid_pop.push_back(offspring[random_index]); 
        }
        else if (!parents.empty()) { 
            int random_index = rand() % parents.size(); 
            valid_pop.push_back(parents[random_index]); 
        }
    }
    if (valid_pop.size() > population_size) { //если уже больше чем нужно
        valid_pop.resize(population_size);
    }
    return valid_pop;
}


vector<int> genetic_algorithm(vector<int>& weights, vector<int>& values, int max_weight, int population_size, int generations) {
    srand(time(0)); 
    int n = weights.size();
    vector<vector<int>> population(population_size, vector<int>(n, 0)); //первое поколение
    for (int i = 0; i < population_size; i++) {
        parent(population[i], (i % n) + 1); 
    }
    for (int gen = 0; gen < generations; gen++) {
        vector<int> costs(population_size);
        vector<int> weights_pop(population_size);
        for (int i = 0; i < population_size; i++) {
            costs[i] = cost(population[i], values);
            weights_pop[i] = weight(population[i], weights);
        }
        int total_cost = 0;
        for (auto& p : population) {
            total_cost += cost(p, values);
        }
        int C = total_cost / population_size;
        vector<vector<int>> parents;
        for (int i = 0; i < population_size; i++) {
            if (weights_pop[i] <= max_weight && selection(population[i], C, values)) {
                parents.push_back(population[i]);
            }
        }
        while (parents.size() < 2) {
            vector<int> random_solution(n, 0);
            int random_index = rand() % n;
            random_solution[random_index] = 1;
            parents.push_back(random_solution);
        }
        vector<vector<int>> offspring;
        for (size_t i = 0; i < parents.size() / 2; i++) {
            int index1 = rand() % parents.size();
            int index2 = rand() % parents.size();
            vector<int> parent1 = parents[index1];
            vector<int> parent2 = parents[index2];
            vector<int> child1 = two_point_crossover(parent1, parent2);
            vector<int> child2 = two_point_crossover(parent2, parent1);
            offspring.push_back(child1);
            offspring.push_back(child2);
        }
        population = new_population(parents, offspring, population_size, weights, max_weight, values);
    }
    return population.empty() ? vector<int>(n, 0) : population[0];
}

int main() {
    ifstream in;
    in.open("ks_60_0");
    int n, max_weight;
    in >> n >> max_weight;
    vector<int> weights(n);
    vector<int> values(n);
    for (int i = 0; i < n; i++) {
        in >> values[i] >> weights[i];
    }
    int population_size = n;
    int generations = 100;
    vector<int> result = genetic_algorithm(weights, values, max_weight, population_size, generations);
    cout << "Best result: ";
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    cout << "Our Cost: " << cost(result, values) << endl;
    cout << "Our Weight: " << weight(result, weights) << endl;
    return 0;
}
