#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
int main(int argc, char *argv[]) {

  int dim_n1 = atoi(argv[1]);
  int dim_m1 = atoi(argv[2]);
  int dim_n2 = atoi(argv[3]);
  int dim_m2 = atoi(argv[4]);

  // criando arquivo para matriz 1
  fstream matriz1;
  matriz1.open("matriz1 resultado", ios::out);
  // criando a primeira matriz
  std::vector<std::vector<int>> m1;
  for (int i = 0; i < dim_n1; i++) {
    m1.push_back(vector<int>());
    for (int j = 0; j < dim_m1; j++) {
      m1[i].push_back(rand() % 10 + 1);
      matriz1 << m1[i][j] << " ";
    }
    matriz1 << "\n";
  }
  // fechando o arquivo da matriz 1
  matriz1.close();

  // criando arquivo para matriz 2

  fstream matriz2;
  matriz2.open("matriz2 resultado", ios::out);
  // criando a segunda matriz
  std::vector<std::vector<int>> m2;
  for (int i = 0; i < dim_n2; i++) {
    m2.push_back(vector<int>());
    for (int j = 0; j < dim_m2; j++) {
      m2[i].push_back(rand() % 10 + 1);
      matriz2 << m2[i][j] << " ";
    }
    matriz2 << "\n";
  }
  // fechando o arquivo da matriz 2
  matriz2.close();

  // criando arquivo para matriz 3
  fstream matriz3;
  matriz3.open("matriz3 resultado", ios::out);
  // criando matriz resultado
  std::vector<std::vector<int>> res;
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  matriz3 << dim_n1 << " " << dim_m2 << "\n";
  for (int i = 0; i < dim_n1; i++) {
    res.push_back(vector<int>());
    for (int j = 0; j < dim_m2; j++) {
      res[i].push_back(0);
      for (int k = 0; k < dim_m1; k++) {
        res[i][j] += m1[i][k] + m2[k][j];
      }
      res[i].push_back(res[i][j]);
      matriz3 << "c" << i << j << " " << res[i][j] << "\n";
    }
    // matriz3 << "\n";
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  matriz3 << "Tempo: "
          << chrono::duration_cast<chrono::milliseconds>(end - begin).count()
          << "(ms)" << endl;

  matriz3.close();

  return 0;
}