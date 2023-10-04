//Metodo di risoluzione Seidel

#include <math.h> //questa libreria include le funzioni matematiche di base come ad esempio la radice quadrata, la funzione esponenziale, il logaritmo naturale e altre.
#include <stdio.h>  //questa libreria include le funzioni standard di input/output di C, come la printf e la scanf.
#include <stdlib.h> //questa libreria include funzioni di utilità generale come la generazione di numeri casuali, la gestione dell'ambiente di esecuzione e altre.
#include <string.h> //questa libreria include funzioni per la manipolazione di stringhe come la copia, la concatenazione e altre.

#define DIM1 30
#define MAX_ITERATION 50  //Definizione costante per il numero di iterazioni massime

float a[DIM1][DIM1], b[DIM1], xn[DIM1], xv[DIM1]; // LR aggiunto xv[]
float gnuplot_memory[MAX_ITERATION][2];
int total_iteration = 0;

//Dichiarazione prototipi funzioni
void seidel(int N, float acc);
void write_gnu_plot(char *fname);

//Main
int main(int argc, char *argv[]) {

  // Controllo sugli argomenti passati da linea di comando
  if (argc != 2) {
    printf("Errore specificare: seidel [Nome_File]\n");
  }

  // Apertura del file dati
  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Errore: Impossibile aprire il file");
    exit(1);
  }

  // Leggo i valori dal file e li memorizzo nella matrice
  float tmp, acc;
  int i, j, flag, dim = 0;
  char filecoef[DIM1];

  // Recupero la dimensione del sistema
  fscanf(fp, "%d", &dim);

  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      fscanf(fp, "%f", &tmp);
      a[i][j] = tmp;
      printf("a[%d][%d] = %.2f ", i, j, a[i][j]);
    }
    fscanf(fp, "%f", &tmp);
    b[i] = tmp;
    printf(" b[%d] = %f \n", i, b[i]);
  }

  // Stampo della dimensione del sistema
  printf("\nDimensione del sistema : %d\n", dim);

  // Chiudo il file dei dati
  fclose(fp);

  // Richiedo all'utente l'accuratezza da utilizzare
  printf("Accuratezza desiderata (esempio: 0.1) : ");
  scanf("%f", &acc);

  // Calcolo le soluzioni - metodo Seidel
  seidel(dim, acc);

  // Stampo i risultati
  printf("\nSolutions:\n");
  for (i = 0; i < dim; i++)
    printf("x[%d]\t\t", i);
  printf("\n");
  for (i = 0; i < dim; i++)
    printf("%f\t", xn[i]);
  printf("\n");

  // Salvo i dati tramite il file specificato a riga di comando
  write_gnu_plot(argv[1]);
}

// Funzione che implementa l'algoritmo di Seidel
void seidel(int N, float acc) {

  int i, j, k = 0;
  float sum1 = 0, sum2 = 0;

  // Controllo se il coefficiente diagonale di A è 0 i.e. A(i,i) è 0
  for (i = 0; i < N; i++) {
    xv[i] = 0;
    if (a[i][i] == 0) {
      printf("Il coefficiente diagonale A(%d,%d) e' zero\n", i, i);
      break;
    }
  }

  // Calcolo le somme
  float err = 0.0;
  do {
    for (i = 0; i < N; i++) {
      sum1 = 0.0;
      // Se non è la prima riga
      if (i > 0) {
        // Sommo la colonna precedente
        for (j = 0; j < i; j++)
          sum1 += a[i][j] * xn[j];
      }
      // Se non è l'ultima riga
      sum2 = 0.0;
      if (i < (N - 1)) {
        // Sommo la colonna successiva
        for (j = i + 1; j < N; j++)
          sum2 = sum2 + a[i][j] * xv[j];
      }

      //Calcolo le successive iterazioni xn

      xn[i] = (b[i] - sum1 - sum2) / a[i][i];

      err = fabs(xn[i] - xv[i]);
    }

    // Creo un nuovo array
    for (i = 0; i < N; i++)
      xv[i] = xn[i];

    // Stampo i risultati
    printf("%d\t", k);
    printf("%f\t", err);
    for (int _i = 0; _i < N; _i++) {
      printf("%5f\t", xv[_i]);
    }
    printf("\n");

    // Aggiorno e archivio la memoria gnuplot per il plot
    gnuplot_memory[k][0] = k;
    gnuplot_memory[k][1] = err;

    // Aggiorno il conteggio delle iterazioni
    k++;

    // Controllo la precisione
  } while (err > acc);

  total_iteration = k;

  printf("\nIterazioni totali = %d", total_iteration);

  return;
}

/*
 * write_gnu_plot()
 *   Funzione per scrivere i dati in un file di output in un formato adatto alla stampa
 *   con gnuplot. Il file da scrivere è specificato nel primo argomento, e
 *   gnuplot_memory è usato come fonte per tracciare i dati.
 */
void write_gnu_plot(char *fname) {
  FILE *fptr; // Puntatore per il puntatore del file per l'elaborazione delle funzioni corrispondenti

  char path[14] = "gnuplot/"; // Array per contenere il percorso della directory in cui devono essere archiviati i dati

  strcat(path, fname);  // Concatena il percorso e il nome del file insieme e lo memorizza in path

  fptr = fopen(path, "w");  // Apre il file nel percorso specificato con i permessi di scrittura

  // Scorre l'array bidimensionale della memoria gnuplot e scrive l'iterazione e il valore dell'errore nel file di output
  for (int i = 0; i < total_iteration; i++)
    fprintf(fptr, "%d %f\n", (int)gnuplot_memory[i][0], gnuplot_memory[i][1]);

  fclose(fptr); // Chiude il file al termine della scrittura
}
