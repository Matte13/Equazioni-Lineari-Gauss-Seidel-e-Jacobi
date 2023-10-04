//Metodo di risoluzione Jacobi

#include <cstdlib>  //questa libreria include le funzioni standard di C per la gestione della memoria, la conversione di stringhe in numeri e altre funzioni di utilità generale
#include <math.h> //questa libreria include le funzioni matematiche di base come ad esempio la radice quadrata, la funzione esponenziale, il logaritmo naturale e altre.
#include <stdio.h>  //questa libreria include le funzioni standard di input/output di C, come la printf e la scanf.
#include <stdlib.h> //questa libreria include funzioni di utilità generale come la generazione di numeri casuali, la gestione dell'ambiente di esecuzione e altre.
#include <string.h> //questa libreria include funzioni per la manipolazione di stringhe come la copia, la concatenazione e altre.

#define DIM1 30
#define MAX_ITERATION 50 //Definizione costante per il numero di iterazioni massime

float gnuplot_memory[MAX_ITERATION][2];
float a[DIM1][DIM1], b[DIM1], x_old[DIM1];
int total_iteration = 0;

//Dichiarazione prototipi funzioni
void jacobi(int N, float acc);
void write_gnu_plot(char *fname);

//Main
int main(int argc, char *argv[]) {

  // Controllo sugli argomenti passati da linea di comando
  if (argc != 2) {
    printf("Errore specificare: jacobi [Nome_File]\n");
  }

  FILE *fp;
  float tmp, acc;
  int i, j, flag, dim = 0;

  printf("--------------------Jacobi-------------------- \n");

  // Apertura del file dati
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Errore: Impossibile aprire il file");
    exit(1);
  }

  // Recupero la dimensione del sistema
  fscanf(fp, "%d", &dim);

  printf("\n Matrice A | b\n\n");

  // Riempio la matrice A e il vettore b con i valori recuperati dal file
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      fscanf(fp, "%f", &tmp);

      a[i][j] = tmp;
      printf(" %.2f ", a[i][j]);
    }
    fscanf(fp, "%f", &tmp);
    b[i] = tmp;
    printf(" | %f \n", b[i]);
  }

  // Stampo della dimensione del sistema
  printf("\nDimensione del sistema : %d\n", dim);

  // Chiudo il file dei dati
  fclose(fp);

  // Richiedo all'utente l'accuratezza da utilizzare
  printf("Accuratezza desiderata (esempio: 0.1) : ");
  scanf("%f", &acc);

  // Calcolo le soluzioni - metodo Jacobi
  jacobi(dim, acc);

  // Stampo i risultati
  printf("\nSoluzioni:\n");
  for (i = 0; i < dim; i++)
    printf("x[%d]\t\t", i);
  printf("\n");
  for (i = 0; i < dim; i++)
    printf("%f\t", x_old[i]);
  printf("\n");

  // Salvo i dati tramite il file specificato a riga di comando
  write_gnu_plot(argv[1]);
}

// Funzione che implementa l'algoritmo di Jacobi
void jacobi(int N, float acc) {
  
  // Inizializzo l'array per le soluzioni stimate x_new
  float x_new[N];

  // Inizializzo la soluzione inziale x_old a 0s
  for (int _i = 0; _i < N; _i++) {
    x_new[_i] = 1;
    x_old[_i] = 1;
  }

  // Controllo se il coefficiente diagonale di A è 0 i.e. A(i,i) è 0
  for (int i = 0; i < N; i++) {
    if (a[i][i] == 0.0) {
      printf("Il coefficiente diagonale A(%d,%d) e' zero\n", i, i);
      exit(EXIT_FAILURE);
    }
  }

  // Stampa dell'header della tabella
  printf("Iter\tError\t\t");
  for (int _i = 0; _i < N; _i++)
    printf("x[%d]\t\t", _i);
  printf("\n");

  // Inizializzo errori e iterazioni a 0
  float err = 0.0;
  int k = 0;

  // Stimo le soluzioni utilizzando il processo iterativo di Jacobi
  do {

    // Calcolo x stimato nell'iterazione k
    for (int i = 0; i < N; i++) {

      float sum = 0;

      // Calcolo la somma dei termini rimanenti
      for (int j = 0; j < N; j++) {
        if (j != i)
          sum += a[i][j] * x_old[j];
      }

      // Calcolo x_new dall'equazione
      x_new[i] = (b[i] - sum) / a[i][i];

      // Calcolo gli errori
      err = fabs(x_old[i] - x_new[i]);
      x_old[i] = x_new[i];
    }

    // Stampo i risultati dell'iterazione corrente
    printf("%d\t", k);
    printf("%f\t", err);
    for (int _i = 0; _i < N; _i++) {
      printf("%5f\t", x_old[_i]);
    }
    printf("\n");

    // Aggiorno e archivio la memoria gnuplot per il plot
    gnuplot_memory[k][0] = k;
    gnuplot_memory[k][1] = err;

    // Aggiorno il conteggio delle iterazioni
    k++;
  } while (err > acc);

  // Memorizzo il conteggio totale delle iterazioni nella variabile globale
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

  strcat(path, fname); // Concatena il percorso e il nome del file insieme e lo memorizza in path

  fptr = fopen(path, "w"); // Apre il file nel percorso specificato con i permessi di scrittura

  // Scorre l'array bidimensionale della memoria gnuplot e scrive l'iterazione e il valore dell'errore nel file di output
  for (int i = 0; i < total_iteration; i++)
    fprintf(fptr, "%d %f\n", (int)gnuplot_memory[i][0], gnuplot_memory[i][1]);

  fclose(fptr); // Chiude il file al termine della scrittura
}