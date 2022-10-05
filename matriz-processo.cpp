#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

// criação de matriz
void newMatrix(vector<vector<int>> *m, int dim_n, int dim_m, string filename) {
  fstream matriz;
  matriz.open(filename, ios::out);
  // criando a primeira matriz
  matriz << dim_n << "\n";
  matriz << dim_m << "\n";
  for (int i = 0; i < dim_n; i++) {
    (*m).push_back(vector<int>());
    for (int j = 0; j < dim_m; j++) {
      (*m)[i].push_back(rand() % 10 + 1);
      matriz << (*m)[i][j] << "\n";
    }
    // matriz1 << "\n";
  }
  // fechando o arquivo da matriz 1
  matriz.close();
}

// multiplicação sequencial
void seqProd(vector<vector<int>> *m1, vector<vector<int>> *m2, int d_n1,
            int d_n2, int d_m1, int d_m2) {
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  // criando arquivo para matriz 3
  fstream matriz3;
  matriz3.open("matriz3 resultado", ios::out);
  // criando matriz resultado
  std::vector<std::vector<int>> res;
  matriz3 << d_n1 << " " << d_m2 << "\n";
  for (int i = 0; i < d_n1; i++) {
    res.push_back(vector<int>());
    for (int j = 0; j < d_m2; j++) {
      res[i].push_back(0);
      for (int k = 0; k < d_m1; k++) {
        res[i][j] += (*m1)[i][k] * (*m2)[k][j];
      }
      // res[i].push_back(res[i][j]);
      matriz3 << "c" << i << j << " " << res[i][j] << "\n";
    }
    // matriz3 << "\n";
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  matriz3 << "Tempo: "
          << chrono::duration_cast<chrono::milliseconds>(end - begin).count()
          << "(ms)" << endl;

  matriz3.close();
}

// multiplicação parcialmente uma matriz
void partialMatrixProd(vector<vector<int>> *m1, vector<vector<int>> *m2,
                      vector<vector<int>> *mF, int d1nB, int d1nE, int d1m,
                      int d2m, int d1n, int nt, bool type) {
  chrono::steady_clock::time_point begint = chrono::steady_clock::now();
  string TorP;
  if (type == false) {
    TorP = "Thread ";
  } else {
    TorP = "Process ";
  }
  fstream threadx;
  threadx.open(TorP + to_string(nt), ios::out);
  threadx << d1n << "\n";
  threadx << d2m << "\n";

  for (int i = d1nB; i < d1nE; i++) {
    (*mF).push_back(vector<int>());
    for (int j = 0; j < d2m; j++) {
      (*mF)[i - d1nB].push_back(0);
      for (int k = 0; k < d1m; k++) {
        (*mF)[i - d1nB][j] += (*m1)[i][k] * (*m2)[k][j];
      }
      threadx << (*mF)[i - d1nB][j] << endl;
    }
  }
  chrono::steady_clock::time_point endt = chrono::steady_clock::now();
  threadx << "Tempo: "
          << chrono::duration_cast<chrono::milliseconds>(endt - begint).count()
          << "(ms)" << endl;
  threadx.close();
}

// separa a matriz em P partes e calcula cada parte em uma thread diferente
void threadProd(vector<vector<int>> *m1, vector<vector<int>> *m2, int d_n1,
               int d_n2, int d_m1, int d_m2, int P) {
  std::vector<std::vector<int>> m3;
  std::vector<std::vector<int>> m3p[P];
  thread th[P];

  for (int i = 0; i < P; i++) {
    th[i] = thread(partialMatrixProd, m1, m2, &m3p[i], i * (d_n1 / P),
                   (i + 1) * (d_n1 / P), d_m1, d_m2, d_n1, i, false);
  }
  for (int i = 0; i < P; i++) {
    th[i].join();
  }
}

// separa a matriz em P processos e calcula cada parte em um processo diferente
void processProd(vector<vector<int>> *m1, vector<vector<int>> *m2, int d_n1,
                int d_n2, int d_m1, int d_m2, int P) {
  std::vector<std::vector<int>> m3;
  std::vector<std::vector<int>> m3p[P];
  int pr[P];
  
  pr[0] = fork();
  for (int i = 0; i < P; i++) {
    if (pr[i] > 0) {
      partialMatrixProd(m1, m2, &m3p[i], i * (d_n1 / P), (i + 1) * (d_n1 / P),
                       d_m1, d_m2, d_n1, i, true);
      break;
    } else if (i + 1 < P) {
      pr[i + 1] = fork();
    } else {
      _exit(0);
    }
  }
  
}

// compilar com 'g++ -pthread -o main main.cpp'
int main(int argc, char *argv[]) {

  int size = 1800;
  int dim_n1 = size;
  int dim_m1 = size;
  int dim_n2 = size;
  int dim_m2 = size;

  std::vector<std::vector<int>> m1;
  std::vector<std::vector<int>> m2;

  newMatrix(&m1, dim_n1, dim_m1, "matriz1 resultado");
  newMatrix(&m2, dim_n2, dim_m2, "matriz2 resultado");
  
  fstream tempot;
  
  tempot.open("TempoTotal", ios::out);

  
  /*// soma sequencial
  chrono::steady_clock::time_point begins = chrono::steady_clock::now();
  
  seqProd(&m1, &m2, dim_n1, dim_n2, dim_m1, dim_m2);
  
  chrono::steady_clock::time_point ends = chrono::steady_clock::now();
  
  tempot << "TempoSeq: "
          << chrono::duration_cast<chrono::milliseconds>(ends - begins).count()
          << "(ms)" << endl;*/

  
  // soma com threads
  chrono::steady_clock::time_point begint = chrono::steady_clock::now();
  
  threadProd(&m1, &m2, dim_n1, dim_n2, dim_m1, dim_m2, 4);
  
  chrono::steady_clock::time_point endt = chrono::steady_clock::now();
  
  tempot << "TempoThread: "
          << chrono::duration_cast<chrono::milliseconds>(endt - begint).count()
          << "(ms)" << endl;
          
          
  // soma com processos
  chrono::steady_clock::time_point beginp = chrono::steady_clock::now();
  
  processProd(&m1, &m2, dim_n1, dim_n2, dim_m1, dim_m2, 4);
  
  chrono::steady_clock::time_point endp = chrono::steady_clock::now();
  
  tempot << "TempoProcess: "
          << chrono::duration_cast<chrono::milliseconds>(endp - beginp).count()
          << "(ms)" << endl;

  tempot.close();

  // time
  /*ofstream matriz3;
  matriz3.open("matriz3 resultado", ios::app);

  cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count()
       << endl;*/

  return 0;
}
