/*  Marcio Vicente da Silva Filho            */
/*  Turma 27.4                               */
/*  Exercicio 10: VESTIBULAR                 */
/*                                           */
/*  Programa compilado com CodeBlocks 20.03  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// variaveis globais
char buffer[100];

typedef struct Candidato
{
   int id;
   char nome[30];
   float nota;
   char curso;
   char status;
   char respostas[51];
} candidato;

candidato candidatos[70];

void lixo(FILE *f, int n)
{
   for (int i = 0; i < n; i++)
   {
      fgets(buffer, 100, f);
   }
}

int main()
{
   // Declaração de variáveis
   char gabarito[50];
   int ni = 0, np = 0;         // nisncritos e npresentes
   int vc, va, vt;             // vagas por curso
   float ntc, nta, ntt;        // notas minimas
   int cc = 0, ca = 0, ct = 0; // classificados de cada curso
   int i = 0, j = 0;
   int aux;
   candidato cAux;
   candidato *notaOrdem[70]; // Array de ponteiros ordenados por nota
   candidato *nomeOrdem[70]; // Array de ponteiros ordenados por nome

   FILE *pI = fopen("1.txt", "r");
   FILE *pR = fopen("2.txt", "r");
   FILE *pG = fopen("Lab10_gabarito.txt", "r");
   FILE *pS = fopen("Lab10_Marcio_Vicente_da_Silva_Filho.txt", "w");

   // leitura dos inscritos
   lixo(pI, 4);
   fscanf(pI, "Numero de candidatos inscritos: %d", &ni);
   printf("Numero de candidatos inscritos: %d", ni);
   lixo(pI, 4);
   for (i = 0; i < ni; i++)
   {
      fscanf(pI, " %d", &candidatos[i].id);
      fgets(candidatos[i].nome, 30, pI);
      fscanf(pI, " %c", &candidatos[i].curso);
      printf("\n i = %d id = %d nome = %s curso = %c\n", i, candidatos[i].id, candidatos[i].nome, candidatos[i].curso);
   }
   getchar();
   // Coloca valores padrão nos candidatos
   for (i = 0; i < ni; i++)
   {
      candidatos[i].nota = 0;
      candidatos[i].status = 'X'; // ausente
   }

   // leitura das respostas (le antes do programa interagir com o gabarito por questoes de segurança)

   // leitura das respostas
   lixo(pR, 4);
   fscanf(pR, "Folhas de respostas recebidas: %d", &np);
   printf("Numero de folhas re respostas: %d\n", np); // debugging
   lixo(pR, 4);
   for (i = 0; i < np; i++)
   {
      fscanf(pR, " %d", &aux);
      printf("Response sheet ID: %d\n", aux); // debugging

      bool found = false;
      for (j = 0; j < ni && !found; j++)
      {
         if (candidatos[j].id == aux)
         {
            fscanf(pR, " %s", &candidatos[j].respostas);
            printf("Achou! Responses: %s\n", candidatos[j].respostas); // debugging

            candidatos[j].status = 'P';
            found = true;
         }
      }

      if (!found)
      {
         printf("ID nao encontrado\n"); // debugging
      }

      printf("\nloopou\n");
   }
   // leitura do gabarito oficial
   lixo(pG, 6);
   fscanf(pG, " %s", &gabarito);
   lixo(pG, 4);
   fscanf(pG, "%s %d %f", &buffer, &vc, &ntc);
   fscanf(pG, "%s %d %f,", &buffer, &va, &nta);
   fscanf(pG, "%s  %d %f", &buffer, &vt, &ntt);

   // fechamento dos arquivos
   fclose(pI);
   fclose(pR);
   fclose(pG);

   // atribuicao das notas
   for (i = 0; i < ni; i++)
   {
      if (candidatos[i].status != 'A')
         for (j = 0; j < 50; j++)
         {
            if (gabarito[j] == candidatos[i].respostas[j])
               candidatos[i].nota += 0.2;
         }
      printf("\n\n %s %.2f\n\n", candidatos[i].nome, candidatos[i].nota);
   }

   // odernação dos candidatos por nota
   for (i = 0; i < ni; i++)
   {
      notaOrdem[i] = &candidatos[i];
      nomeOrdem[i] = &candidatos[i];
   }

   for (i = 0; i < ni - 1; i++)
   {
      for (j = 0; j < ni - i - 1; j++)
      {
         if (notaOrdem[j]->nota < notaOrdem[j + 1]->nota)
         {
            candidato *temp = notaOrdem[j];
            notaOrdem[j] = notaOrdem[j + 1];
            notaOrdem[j + 1] = temp;
         }
      }
   }

   printf("\nOrdem por Nota:\n");
   for (i = 0; i < ni; i++)
   {
      printf("ID: %d, Nota: %.2f\n", notaOrdem[i]->id, notaOrdem[i]->nota);
   }
   getchar();

   // Ordena os candidatos por nome
   for (i = 0; i < ni - 1; i++)
   {
      for (j = 0; j < ni - i - 1; j++)
      {
         if (strcmp(nomeOrdem[j]->nome, nomeOrdem[j + 1]->nome) > 0)
         {
            candidato *temp = nomeOrdem[j];
            nomeOrdem[j] = nomeOrdem[j + 1];
            nomeOrdem[j + 1] = temp;
         }
      }
   }

   printf("\nOrdem por nome:\n");
   for (i = 0; i < ni; i++)
   {
      printf("ID: %d, Nome: %s\n", nomeOrdem[i]->id, nomeOrdem[i]->nome);
   }
   getchar();

   // verifica quem passou noque (utilizando a ordem de nota)
   for (i = 0; i < np; i++)
   {
      switch (notaOrdem[i]->curso)
      {
      case 'C':
         if (notaOrdem[i]->nota >= ntc)
         {
            notaOrdem[i]->status = 'E'; // espera
            if (vc > 0)
            {
               vc--;
               cc++;
               notaOrdem[i]->status = 'C'; // classificado
            }
         }
         else
            notaOrdem[i]->status = 'R';
         break;
      case 'A':
         if (notaOrdem[i]->nota >= nta)
         {
            notaOrdem[i]->status = 'E'; // espera
            if (va > 0)
            {
               va--;
               ca++;
               notaOrdem[i]->status = 'C'; // classificado
            }
         }
         else
            notaOrdem[i]->status = 'R';
         break;
      case 'T':
         if (notaOrdem[i]->nota >= ntt)
         {
            notaOrdem[i]->status = 'E'; // espera
            if (vt > 0)
            {
               vt--;
               ct++;
               notaOrdem[i]->status = 'C'; // classificado
            }
         }
         else
            notaOrdem[i]->status = 'R'; // reprovado
         break;
      }
   }

   printf("\nStatus final:\n");
   for (i = 0; i < ni; i++)
   {
      printf("ID: %d, Status: %c\n", notaOrdem[i]->id, notaOrdem[i]->status);
   }
   getchar();

   fprintf(pS, "New day shines, fallen angels will arise\n"
               "Nova Era brings the ashes back to life\n"
               "----------------------------------\n"
               "a) Classificados no vestibular\n"
               "-----------------------------------\n\n");

   fprintf(pS, "Sub-lista Computacao:\n\n");
   for (i = 0; i < ni; i++)
   {
      if (nomeOrdem[i]->status == 'C' && nomeOrdem[i]->curso == 'C')
      {
         fprintf(pS, "    %s\n", nomeOrdem[i]->nome);
      }
   }
   fprintf(pS, "\n\t\tNumero de classificados: %d\n"
               "\t\tVagas remanescentes:     %d\n\n",
           cc, vc);

   // ...

   fprintf(pS, "Sub-lista Astronomia:\n\n");
      for (i = 0; i < ni; i++)
   {
      if (nomeOrdem[i]->status == 'C' && nomeOrdem[i]->curso == 'A')
      {
         fprintf(pS, "    %s\n", nomeOrdem[i]->nome);
      }
   }
   fprintf(pS, "\t\tNumero de classificados: %d\n"
               "\t\tVagas remanescentes:     %d\n\n",
           ca, va);

   // ...

   fprintf(pS, "Sub-lista Turismo:\n\n");
   for (i = 0; i < ni; i++)
   {
      if (nomeOrdem[i]->status == 'C' && nomeOrdem[i]->curso == 'T')
      {
         fprintf(pS, "    %s\n", nomeOrdem[i]->nome);
      }
   }
   fprintf(pS, "\n\t\tNumero de classificados: %d\n"
               "\t\tVagas remanescentes:     %d\n\n",
           ct, vt);

   fprintf(pS, "----------------------------------\n"
               "b) Pontuacao Geral\n"
               "-----------------------------------\n\n");
   fprintf(pS, "Posicao       Nome                   Nota\n");
   for (int i = 0; i < np; i++)
   {
      fprintf(pS, "    %2d    %s", i + 1, notaOrdem[i]->nome);
      for (int j = 0; j < 30 - strlen(notaOrdem[i]->nome); j++)
      {
         fprintf(pS, " ");
      }
      fprintf(pS, "%.1f\n", notaOrdem[i]->nota);
   }
   fprintf(pS, "----------------------------------\n"
               "c) Relatorio completo\n"
               "-----------------------------------\n\n");

   fprintf(pS, " Num.          Nome                   Curso    Nota   Situacao\n\n");
   for (i = 0; i < ni; i++)
   {
      fprintf(pS, " %4d %s", nomeOrdem[i]->id, nomeOrdem[i]->nome);
      for (int j = 0; j < 30 - strlen(nomeOrdem[i]->nome); j++)
      {
         fprintf(pS, " ");
      }
      switch (nomeOrdem[i]->curso)
      {
      case 'C':
         fprintf(pS, "Computacao");
         break;
      case 'A':
         fprintf(pS, "Astronomia");
         break;
      case 'T':
         fprintf(pS, "Turismo   ");
         break;
      }
      fprintf(pS, "   %.1f", nomeOrdem[i]->nota);
      switch (nomeOrdem[i]->status)
      {
      case 'E':
         fprintf(pS, "   espera\n");
         break;
      case 'C':
         fprintf(pS, "   classificado\n");
         break;
      case 'R':
         fprintf(pS, "   reprovado\n");
         break;
      case 'X':
         fprintf(pS, "   ausente\n");
         break;
      }
   }
   fclose(pS);
   return 0;
}