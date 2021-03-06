#include "all.h"



Matrix *get_data_from_file(const char *file, int M_data)
/* Retorna matriz alocada com os dados lidos em duas colunas
 * deve receber como parâmetros:
 * - o arquivo a ser lido 
 * - o número de elementos a serem lidos
 *
 *	--é mais fácil pedir o número de elementos do que bolar
 * um algoritmo que conte o númeor de elementos no arquivo
 * */
{
		FILE *fp;
		int n;
		Matrix *DATA;
		
		DATA =  new_M(M_data, 2);
		
		fp = fopen(file, "r");
	
		for (n = 0; n < M_data; n++)
			fscanf(fp, "%lf %lf", &DATA->a[n][0],\
			&DATA->a[n][1]); 	
			
		fclose(fp);
		
		return(DATA);
}


Matrix *LinearDataFittingBySum(const char *file, int M_data)
/* Retorna a matriz X alocada contendo (alpha, beta)
 * -O algortimo usa a fórmula:
 * beta = 
 *((t.y)_{avg} - t_{avg}.y_{avg}) /((t^2)_{avg} (t_{avg})^2)
 * 
 * alpha =
 * y_{avg} -t_{avg}.Beta
 * 
 *onde A_{avg} denot a média da variável A nos dados tomados
 *!não confundir (t.y)_{avg} com t_{avg}.y_{avg}
 * 
 * Mais descrições em:
 * Numerical Analysis Lectures Notes - Peter J. Oliver
 * cap: 13 Approxiation and Interpolation; pgs 221-223   
 * 
 * Código por :
 * 				Juarez A.S.F.
 * 
 * Motivação:
 * 			curso de	Cálculo Numérico UnB - 1º/2013
*/
{
	Matrix 	*DATA, *X;	
	double 	ty, t, y, t2;
	double 	beta, alpha;
	int 	n;

	DATA = get_data_from_file("data.dat", M_data);	

	ty	= 0;
	t	= 0;
	y 	= 0;
	t2	= 0;
	
	for(n = 0; n < M_data; n++)
		{
		//calculando a média dos valores a serem utilizados
		ty+= DATA->a[n][0] * DATA->a[n][1]/M_data; 
		t += DATA->a[n][0]/M_data;
		y += DATA->a[n][1]/M_data;
		t2+= DATA->a[n][0]*DATA->a[n][0]/M_data;
		}
		
	beta = (ty - t*y)/(t2 - t*t);
	alpha = y - t*beta;
	
	X = new_M(2, 1);
	
	X->a[0][0] = alpha;
	X->a[1][0] = beta;
	
	kill_M(&DATA);

	return(X);
}

char *MakePolynom_gnu(Matrix *X)
/*recebe vetor com os coeficients X e escreve o polinomio em
um string. Retorna esse string alocado
*/
{
    char *polynom, c;
    int  n;
    polynom = (char *)aloca(300*sizeof(char));
    FILE *fp;
    fp =fopen("temp.temp", "w");

    fprintf(fp, "(");
    for (n = 0; n < X->rows - 1; n++)
        fprintf(fp, "%.4lf * x**%d + ", X->a[n][0], n);

    fprintf(fp, "%.4lf * x**%d)", X->a[n][0], n);

    fp =freopen("temp.temp", "r", fp);
    n = 0;
    while((c = fgetc(fp)) != EOF)
        {
          polynom[n++] = c;
        }
          polynom[n] = '\0';
    fclose(fp);
    system("rm temp.temp");

    return(polynom);
}

char *MakePolynom_latex(Matrix *X)
/*recebe vetor com os coeficients X e escreve o polinomio em
um string. Retorna esse string alocado
*/
{
    char *polynom, c;
    int  n;
    polynom = (char *)aloca(300*sizeof(char));
    FILE *fp;
    fp =fopen("temp.temp", "w");

    fprintf(fp, "$y(t) = ");
    for (n = 0; n < X->rows - 1; n++)
        fprintf(fp, "%.4lf t^{%d} + ", X->a[n][0], n);

    fprintf(fp, "%.4lf t^{%d}$", X->a[n][0], n);

    fp =freopen("temp.temp", "r", fp);
    n = 0;
    while((c = fgetc(fp)) != EOF)
        {
          polynom[n++] = c;
        }
          polynom[n] = '\0';
    fclose(fp);
    system("rm temp.temp");

    return(polynom);
}

void writeGnuHeader(FILE *fp)
//imprime o cabeçalho para o script Gnuplot
//dividido dessa forma para reutilizacao de codigo
{
    fprintf(fp, "set terminal epslatex size 14cm,10cm \
color colortext 'phv,9'	\
header '\\definecolor{t}{rgb}{0.5,0.5,0.5}'\n\
\
\
set xlabel 't' \n \
set ylabel 'y(t)' norotate \n \n \
set key center bottom \n \
set style line 1 lt 1 lw 3 lc rgb 'red' \n \
set style line 2 lt 1 lw 3 lc rgb 'dark-red'\n \
set style line 3 lt 1 lw 3 lc rgb 'gray'\n \
set style line 4 lt 1 lw 3 lc rgb 'yellow'\n \
set style line 5 lt 1 lw 6 lc rgb 'orange'\n \
set style line 6 lt 1 lw 6 lc rgb 'yellow'\n \
set style line 7 lt 1 lw 6 lc rgb 'green'\n \
set style line 8 lt 1 lw 6 lc rgb 'sea-green'\n \
set style line 9 lt 1 lw 6 lc rgb 'blue'\n \
set style line 10 lt 1 lw 6 lc rgb 'dark-blue'\n \
set style line 11 lt 1 lw 6 lc rgb 'black'\n \
set key outside \n \n\
");
    //cabeçalho para imprimir colorido e em eps
    //e o nome nos eixos

}

void writeGnuPlotCommand(FILE *fp, char *polynom_gnu, \
                        char *polynom_latex, const char *eps_name)

//imprime os comandos de plot para o script do gnuplot
{
    fprintf(fp, "plot 'data.dat' \
u 1:2 w points t 'data points' lt 6 lc 0 lw 6 ,  \
            %s w lines lt 1 lc 0 lw 6 t '%s' \n \n", polynom_gnu, polynom_latex);
}

void MakeGnuplotScript(const char *file, Matrix *X, \
					   const char *eps_name, int n_ordem, \
					   int m_data)
//imprime em file os comandos no gnuplot para traçar
//os pontos juntos com o ajuste
{
		FILE *fp;
        char *polynom_gnu, *polynom_latex;
		fp = fopen(file, "w+");

        writeGnuHeader(fp);

        fprintf(fp, "set output \"%s.tex\" \n", eps_name);
		
		fprintf(fp, "set title 'Fitting Polinomial - \
n = %d m = %d' \n", n_ordem, m_data);
		//define o título 
	
        polynom_gnu = MakePolynom_gnu(X);
        polynom_latex = MakePolynom_latex(X);

        writeGnuPlotCommand(fp, polynom_gnu, polynom_latex,\
                            eps_name);
        fprintf(fp," ! mv './%s.eps' ./images \n \
! mv './%s.tex' ./images", eps_name, eps_name);

        libera(polynom_gnu);
        libera(polynom_latex);

}

void writeGnuMultPlotCommand(FILE *fp, char *polynom_gnu, \
                        char *polynom_latex, \
                        const char *eps_name, int lc_color)

//imprime os comandos de plot para o script do gnuplot
{
    fprintf(fp, " %s w lines ls %d t '%s', "\
									, polynom_gnu,\
									lc_color,\
									polynom_latex);
}

void MakeGnuplotMultPlotScript(const char *file, Matrix **X, \
                       int N_x, const char *eps_name)
//imprime em file os comandos no gnuplot para traçar
//os pontos juntos com o ajuste
{
        FILE *fp;
        char *polynom_gnu, *polynom_latex;
        fp = fopen(file, "w+");
        int i;

        writeGnuHeader(fp);

        fprintf(fp, "set output \"%s.tex\" \n", eps_name);

        fprintf(fp, "set title 'Fitting Polinomial várias \
ordens' \n");
        //define o título
		
			fprintf(fp, "plot");
		for(i = 0; i < N_x - 1; i++)
			{//escrevemos para todos menos o último
			polynom_gnu = MakePolynom_gnu(X[i]);
			polynom_latex = MakePolynom_latex(X[i]);

			writeGnuMultPlotCommand(fp, polynom_gnu,\
								polynom_latex, eps_name, i+1);
			libera(polynom_gnu);
			libera(polynom_latex);
			}
			//escrevemos o comando para o último(n termina
			//em ','!
			polynom_gnu = MakePolynom_gnu(X[i]);
			polynom_latex = MakePolynom_latex(X[i]);
		    fprintf(fp, "%s w lines ls %d t '%s',\
'data.dat' u 1:2 w points t 'data points' lt 6 lc 0 lw 6 \
 \n \n", polynom_gnu, i +1, polynom_latex);
			libera(polynom_gnu);
			libera(polynom_latex);
			fprintf(fp," ! mv './%s.eps' ./images \n \
! mv './%s.tex' ./images", eps_name, eps_name);


}



Matrix *PolynomialFitting(const char *file, int M_data \
										  , int N_grau)
/*Retorna matriz alocada com os coeficientes do polinômio
 * de grau n.
 *A convenção adotada é A[0] = alpha_0, A[n] = alpha_n
 * onde:
 * p(t) = alhpa_0 + alpha_1.t + ... + alpha_n . t^n
 * 
 * Deve receber:
 * - Arquivo com os dados em duas colunas
 * 			sendo a forma (t_k, y_k) adotada
 * - número M de pontos a serem considerados
 * - númeor N do polinômio a ser gerado
 * 
 * Basicamente o algoritmo resolve o sistema:
 * 
 * (A^t . A)x = A^ty
 * 
 * Notamos:
 * 	-A^t . A = D
 * 	-A^ty	 = E
 * 
 * portanto basta resolver D.x = E
 * 
 * para isso podemos fazer:
 * -	x = D^{-1}.E
 * -	Jacobi ou Gauss-Seidel
 * -	eliminação gaussiana
 * 
 * Nessa versão do código usamos a primeira opção  por sim-
 * plicidade
 * 
 * Algoritmo e notação seguindo 
 * Numerical Analysis Lectures Notes - Peter J. Oliver
 * cap: 13 Approxiation and Interpolation; pgs 224-230 
 * 
 * obs. : Se 
 *  m+1 = n temos um fitting perfeito para os dados
 *  m+1 > n temos o erro quadrático mínimo
 *  m+1 < n ??? 
 * */
{
		Matrix *DATA, *A, *Y, *At, *D, *D_inv, *E;
		Matrix *X;
		int 	i,j;
		
		DATA 	= get_data_from_file(file, M_data);
		//DATA->a[n][0] : representa t_n
		//DATA->a[n][1] : representa y_n
		//A notação é resolvida pelo #define ao início
		A 		= new_M(M_data,N_grau + 1);
		Y 		= new_M(M_data, 1);
		
		for(i = 0; i < A->rows; i++)
			for(j = 0; j < A->columns; j++)
				{
				if(j == 0)
						A->a[i][j] = 1;
				else
						A->a[i][j] = pow(DATA->a[i][0],j);
				}
		for(i = 0; i < Y->rows; i++)
				Y->a[i][0] = DATA->a[i][1];
				
		print_M(A);
		print_M(Y);
				
		At 		= Transpose_M(A);
		
		D 		= M_cross_N(At, A);
		
		E		= M_cross_N(At, Y);
		
		D_inv 	= Inverse(D);
		
		X		= M_cross_N(D_inv, E);
		
		kill_M(&DATA);
		kill_M(&A);
		kill_M(&Y);
		kill_M(&At);
		kill_M(&D);
		kill_M(&D_inv);
		kill_M(&E);
		
		return(X);
}
/*
int main()
{

	Matrix *X, *Y, *Z, **A;
	
	A = (Matrix **)aloca(3 * sizeof(Matrix *));
	
	X = PolynomialFitting("data.dat", 5, 2);
	Y = PolynomialFitting("data.dat", 5, 3);
	Z = PolynomialFitting("data.dat", 5, 4);
	//exemplo pg 230 : resultado ok!
	A[0] = X;
	A[1] = Y;
	A[2] = Z;
	
	
	MakeGnuplotScript("plot.GNU", X, "leGraph", 2, 5);
	MakeGnuplotMultPlotScript("MultPlot.GNU", A, 3, "MultGraph");
	
	kill_M(&X);
	
	
	
	EndProgram();
	
	return(0);
}

*/
