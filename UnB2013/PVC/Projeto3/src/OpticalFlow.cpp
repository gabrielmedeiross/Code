#include "../headers/OpticalFlow.h"
#include "../headers/myMath.h"


#define GREEN CV_RGB(0,255,0)
#define RED CV_RGB(0,0,255)
#define BLUE CV_RGB(255,0,0)

using namespace cv;
using namespace std;

static TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
static Size  winSize(31,31),  subPixWinSize(10,10);
/*essas últimas duas linahs definem o tamanho da imagem a ser utilizado
 * esses parametros são passados como argumento da função que calcula
 * o fluxo óptico.
 * eles não são necessários, isto é, a função tem valores default para eles
 * */

void drawNormalizedArrows(Mat &img, vector<Point2f> &P,
		vector<Point2f> &P2)
{
    double l_max = -10;
    for (size_t i = 0; i < P2.size(); i++)
        {
    	//descobre o maior módulo
        float dx = P2[i].x - P[i].x;
        float dy = P2[i].y- P[i].y;

        double l = sqrt(dx*dx + dy*dy);

        if(l>l_max) l_max = l;
        }



    for (size_t i = 0; i < P2.size(); i++)
    {
        float dx = P2[i].x - P[i].x;
        float dy = P2[i].y- P[i].y;
        double l = sqrt(dx*dx + dy*dy);
        //'l' é módulo da velocidade
        if (l > 0)
        {//dezenha flecha com ângulo de ponta definido
            double spinSize = l/l_max;
            double angPonta = 3.1416 / 4;

            //dezenha o traço da flecha
            line(img, P[i], P2[i], GREEN, 1, CV_AA);

            //dezenha a cabeça da flexa
            double angle;                                                                           // Draws the spin of the arrow
            angle = atan2( (double) P[i].y - P2[i].y, (double) P[i].x - P2[i].x );

            CvPoint p;
            p.x = (int) (P2[i].x + spinSize * cos(angle + angPonta));
            p.y = (int) (P2[i].y + spinSize * sin(angle + angPonta));
            line(img, p,P2[i], CV_RGB(0,255,0), 1, CV_AA, 0 );

            p.x = (int) (P2[i].x + spinSize * cos(angle - angPonta));
            p.y = (int) (P2[i].y + spinSize * sin(angle - angPonta));
            line( img, p, P2[i], CV_RGB(0,255,0), 1, CV_AA, 0 );
        }
   }

}

float maxElement(vector<float> data){
	float max = -100000;
	for(size_t i = 0; i < data.size(); i++)
		if(data[i] > max)
			max =data[i];

	return max;
}

void sparseOpticFlowCalculate(Mat &previous, Mat &current, Mat &output)
{
    Mat gray, prevGray;
    vector<Point2f> pointsToTrack, pointsFounded, V;

     if(current.empty() || previous.empty()){
    	 	return;
     	 }
     //conversão para PB e filtragem básica
     cvtColor(current, gray, CV_BGR2GRAY);
     cvtColor(previous, prevGray, CV_BGR2GRAY);
     myFilter(gray);
     myFilter(prevGray);

     int MAX_COUNT = 500;
     goodFeaturesToTrack(gray, pointsToTrack,
    		 MAX_COUNT, 0.01, 10, Mat(),
    		 3, 0, 0.04);

     cornerSubPix(gray, pointsToTrack,
    		 subPixWinSize, Size(-1,-1), termcrit);

    vector<uchar> status;
    vector<float> err;
    /*status guarda o sucesso da busca para cada ponto
     * err guarda o erro da medida para cada ponto*/

    calcOpticalFlowPyrLK(prevGray, gray, pointsToTrack,
    					pointsFounded, status, err, winSize,
                                 3, termcrit, 0, 0.001);


    //eliminando dados ruins
    size_t i, k;
    float maxError = maxElement(err);

    for( i = k = 0; i < pointsFounded.size(); i++ )
    	{
       	//se o ponto falhou não será computado!
    	if( !status[i] )
    		continue;
    	if( err[i] > 0.2*maxError )
    	    		continue;

    	V.push_back(pointsFounded[i] - pointsToTrack[i]);

    	pointsFounded[k] = pointsFounded[i];
    	pointsToTrack[k++] = pointsToTrack[i];
    	}
    	pointsToTrack.resize(k);
    	pointsFounded.resize(k);

    	drawNormalizedArrows(output, pointsToTrack, pointsFounded);

    	//myK_Means(output, pointsToTrack, V);

    return ;
}

