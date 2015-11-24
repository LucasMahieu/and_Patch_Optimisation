/**
 * \file computePatchOpt.c
 * \brief
 * ======================================================================
 *
 *       Filename:  comuptePatchOpt.c
 *
 *    Description: Programme qui va trouver le Patch de coût minimum 
 *
 *        Version:  1.0
 *        Created:  17.11.2015 13:43:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lucas MAHIEU (LM), lucas.mahieu@phelma.grenoble-inp.fr
 *        Company:  Grenoble_INP - 1A_PHELMA - 2A_ENSIMAG
 *
 * ======================================================================
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>


int computePatchOpt_it(int, int);
/**
 * \struct "cellule" qui contient pour chaque combinaison de ligne AiBj 
 * le cout min, 
 * le nom de la derniere cmd,
 *  et les coordonnées de la la cases qui à donné ce cout min.
 */
typedef struct {
	int cout;
	fpos_t l_cpy;
	int pereI;
	int pereJ;
} cellule;

/**
 * Variable globale: Pointeur qui servira à stocker toutes les cellules
 */
cellule** mem = NULL;
/**
 * Variable globale: Fichier d'entrée
 */
FILE* fA;
/**
 * Variable globale: Fichier de sortie
 */
FILE* fB;

/**
 * \fn int computePatchOpt_it (int n, int m)
 * Cette fonction permet de créer un patch de cout min = patch Optimal
 * qui transformera le fichier d'entré en le fichier de sortie
 *
 * \param[in] n nombre de ligne du fichier d'entrée
 * \param[in] m nombre de ligne du fichier de sortie
 * \return int cout du patch créer, ce cout est optimal
 */
int computePatchOpt_it(int n, int m)
{
	/*	struct timeval tempsD, tempsF;
		if( gettimeofday(&tempsD,NULL)){
		printf("erreur GetTimeOfDay !!! ");
		return -1;
		}
		*/
	//!Initialisation des variables
	//! -------------------------------
	int i=0, j=0;
	int min = 0, cout=0;
	char* tmpA = NULL;
	size_t lA = 0;
	char* tmpB = NULL;
	size_t lB = 0;
	fpos_t l_pos=0;
	int lBlen=0, lAlen=0;
	int cd = 10, cD=15, ca=0, cs=0;
	int pi=0,pj=0;
	int iD=0;
	int add=0, sub=0, del=0, Del=0;
	int sumB=0;
	//! -------------------------------
	rewind(fB);
	for(j = 0; j <=m; j++) {
		if (j!=0) {
			fgetpos(fB, &l_pos);
			lBlen = getline(&tmpB, &lB, fB);
			sumB += 10 + lBlen;
		}
		rewind(fA);
		iD=0;
		for(i = 0; i <=n; i++) {
			if(i!=0){
				lAlen = getline(&tmpA, &lA, fA);
			}
			// i=j=0	
			if (i==0) {
				if (j==0) {
					mem[0][0].cout = 0;
					mem[0][0].pereI = 0;
					mem[0][0].pereJ = 0;
				}
				//i=0 et j!=0  -> f(0,j) = sum(10+LkB) avec k=1..j, calculé à chaque getline
				else{
				//	rewind(fB);
				//	for(k=1; k<=j; k++){
				//		lBlen = getline(&tmpB, &lB,fB);
				//		L += 10 + lBlen;
				//	}
					pi = i;
					pj = j-1;
					//if( (ecrit = sprintf(toPrint,"+ %d\n%s",i,tmpB))==-1){
					//Maj du cout, de la cmd, et des peres pour retrouver le chemin
					mem[0][j].cout = sumB; 
					mem[0][j].l_cpy = l_pos;
					mem[0][j].pereI = pi; 
					mem[0][j].pereJ = pj; 
				}
			} 
			// i!=0 
			else {
				//Pour j=0, on Delete
				if (j==0) {
					// Si i=1, deletion coute 10
					if (i==1) {
						//if ((ecrit = sprintf(toPrint,"d %d\n",i)) ==-1){
						mem[i][0].cout = 10;
						pi = i-1;
						pj = j;
					}
					// j=0 et i!=1
					else{
						// Si i>1, deletion coute 15
						//if( (ecrit = sprintf(toPrint,"D %d %d\n",1,i))==-1){
						mem[i][0].cout = 15;
						pi = i-i;
						pj = j;
					}
					mem[i][0].pereI = pi; 
					mem[i][0].pereJ = pj; 
				}
				// i!=0 et j!=0
				else {
					// Pour comparer Li(A) et Lj(B) n regarde d'abords leur taille	
					if (lBlen == lAlen){
						if(strcmp(tmpA,tmpB) == 0){
							//Même taille et même chaine 
							cs = 0;
						}
						else{
							//Même taille mais différents
							cs = 10 + lBlen;
						}
					}
					//Taille différentes donc différent
					else {
						cs = 10 + lBlen;
					}
					ca = 10 + lBlen;
					// Calcules du cout des 3 opérations possibles
					add = mem[i][j-1].cout + ca;
					sub = mem[i-1][j-1].cout + cs;
					del = mem[i-1][j].cout + cd;
					Del = mem[iD][j].cout + cD;
					// On selectionne l'opération de cout min
					min = sub;
					if(cs == 0){
						//ecrit = sprintf(toPrint,"");
						pi = i-1;
						pj = j-1;
					}
					else{
						//if((ecrit = sprintf(toPrint,"= %d\n%s",i,tmpB))==-1){
						pi = i-1;
						pj = j-1;
					}
					if( add<=min ){
						min = add;
						//if((ecrit = sprintf(toPrint,"+ %d\n%s",i,tmpB))==-1){
						pi = i;
						pj = j-1;
					}
					if( del<min ){
						min = del;
						//if((ecrit = sprintf(toPrint,"d %d\n",i))==-1){
						pi = i-1;
						pj = j;
					}
					if( Del<min ){
						min = Del;
						//if((ecrit = sprintf(toPrint,"D %d %d\n",iD+1,i-iD))==-1){
						pi = iD;
						pj = j;
					}
					// Maj du cout, de la cmd, et de l'opération qui a donnée ce min
					mem[i][j].cout = min;
					mem[i][j].l_cpy = l_pos;
					mem[i][j].pereI = pi;
					mem[i][j].pereJ = pj;
					// On stock dans iD le min des cout de la colonne i
					if(min<=mem[iD][j].cout) {
						iD=i;
					}
				}
			}
		}
	}
	i = i-1;
	j = j-1;
	cout = mem[i][j].cout;
	int prevPi=n, prevPj=m;
	int futurPi=-1, futurPj=-1;

	do{
		prevPi = mem[i][j].pereI;
		prevPj = mem[i][j].pereJ;
		mem[i][j].pereI = futurPi;
		mem[i][j].pereJ = futurPj;
		futurPi = i;
		futurPj = j;
		i = prevPi;
		j = prevPj;
	}while(futurPi>0 || futurPj>0);
	// Cette zone commentée permet une bel affichage du résulta du programme 
	/*	if( gettimeofday(&tempsF, NULL) ){
		printf(" erreur getTimeOfDay fin !! ");
		}
		else{
		suseconds_t timeE = tempsF.tv_sec*1000000 + tempsF.tv_usec;
		suseconds_t timeB = tempsD.tv_sec*1000000 + tempsD.tv_usec;
		int sec =(int)((timeE - timeB)/1000000);
		int usec =(int)((timeE - timeB)-1000000*sec);
		int minutes = 0;
		if (sec>=60){
		min = sec/60;
		sec = sec%60;
		}
		printf("#############################################################\n");
		printf("-------------- Patch Optimal généré en ----------------------\n");
		printf("-------------- %02dminutes %02dsec %06dus -------------------------\n",min,sec,usec);
		}
		*/
	free(tmpA);
	free(tmpB);
	return cout;
}

/**
 * \fn int main ( int argc, char* argv[] )
 * \param argc nombre de parametre
 * \param argv[] liste de chaine de char
 *
 */
int main ( int argc, char* argv[] )
{
	if( argc!=3 ){
		printf(" !!!! lancer: computepatchOpt F1 F2 !!!!\n");
		return EXIT_FAILURE;
	}
	// Permet un affichage graphique du résultat 
	/*	if( argc!=4 ){
		printf(" !!!! lancer: computepatchOpt source target patch_à_creer !!!!!\n");
		return EXIT_FAILURE;
		}
		*/
	//	printf("#############################################################\n");
	//	printf("#################  GENERATEUR DE PATCH OPTIMAL ##############\n");
	//	printf("#############################################################\n");
	//	printf("-------------- Entrée du Patch : %s\n",argv[1]);
	//	printf("-------------- Sortie du Patch : %s\n",argv[2]);
	int n = 0;
	int m = 0;
	int i = 0;
	int j = 0;
	/*
	   struct timeval tempsDmain;
	   struct timeval tempsFmain;
	   if( gettimeofday(&tempsDmain,NULL)){
	   printf("erreur GetTimeOfDay !!! ");
	   return -1;
	   }
	   */
	if((fA = fopen(argv[1], "r")) == NULL){
		printf("Erreur fopen f1");
		return -1;
	}
	if((fB = fopen(argv[2], "r")) == NULL){
		printf("Erreur fopen f2");
		return -1;
	}

	//tmp sert juste à stocker une ligne d'un fichier pour compter le nombre de ligne
	char tmp[500];
	// On calcule le nombre de lignes de F1
	while (fgets(tmp,500, fA)!=NULL) {
		n++;
	}
	// On calcule le nombre de lignes de F2
	while (fgets(tmp, 500, fB)!= NULL) {
		m++;
	}
	// On créer le taleau de memorisation de taille (n+1)*(m+1)
	if( (mem =(cellule**)calloc(n+1, sizeof(*mem))) == NULL) {
		printf("Erreur calloc");
		return -1;
	}
	for (i = 0; i < n+1; i++) {
		if( (mem[i] = (cellule*)calloc(m+1, sizeof(**mem))) == NULL) {
			printf("Erreur calloc");
			return -1;
		}
		for (j = 0;  j< m+1; j++) {
			mem[i][j].cout = 0;
			mem[i][j].l_cpy = 0;
			mem[i][j].pereI = 0;
			mem[i][j].pereJ = 0;
		}
	}
	int cout = computePatchOpt_it(n,m);
	printf("%d\n",cout);
	//	printf("-------------- Coût du Patch :  %d \n",computePatchOpt_it(n,m));
	//	printf("#############################################################\n");
	//	printf("-------------- Patch en écriture veuillez patienter\n");

	//	FILE* p = NULL;
	//	p = fopen(argv[3],"w");
	//	int compteur = 0;

	// Dans cette partie, on créer le patch dans le fichier de sortie ou dans stdin
	int l=0, c=0,lTmp=l, cTmp=c;
	int pi=0, pj=0;
	char* str=NULL;
	size_t lB=0;
	do{
		pi = mem[l][c].pereI;
		pj = mem[l][c].pereJ;
		//fputs(mem[l][c].cmd,p);
		// Cas de la SUBTITUTION		
		if( (pi==l+1) && (pj==c+1) ){
			// Al = Bc -> cout = 0
			if(mem[l][c].cout == mem[pi][pj].cout){
				//On doit recopier Al sur la sortie -> le patch ne fait rien
			}
			//On doit substituer la ligne Al par Bc
			else{
				fsetpos(fB, &mem[pi][pj].l_cpy);
				getline(&str, &lB, fB);
				printf("= %d\n%s",pi,str);
			}
		}
		//Cas de l'addition
		else if( (pi==l) && (pj==c+1) ){
			fsetpos(fB, &mem[pi][pj].l_cpy);
			getline(&str, &lB, fB);
			printf("+ %d\n%s",l,str);
		}
		//Cas de la deletion simple
		else if( (pi==l+1) && (pj==c)){
			printf("d %d\n",l+1);	
		}
		//Cas de la deletion multiple
		else{
			printf("D %d %d\n",l+1,pi-l);
		}
		//		if(compteur>=m/100){
		//			compteur = 0;
		//			printf("#");
		//		}
		//		else{
		//		compteur++;
		//		}
		l = mem[lTmp][cTmp].pereI;
		c = mem[lTmp][cTmp].pereJ;
		cTmp = c;
		lTmp = l;
	}while(l<n || c<m);


	//	printf(" 100%%\n");
	//	printf("-------------- Patch écrit dans '%s'\n",argv[3]);
	for (i = n;  i>-1; i--) {
		free(mem[i]);
	}
	free(mem);
	fclose(fA);
	fclose(fB);
	//Permet l'affiche graphique du résultat et du temps d'exécution
	/*	fclose(p);
		if( gettimeofday(&tempsFmain, NULL) ){
		printf(" erreur getTimeOfDay fin !! ");
		}
		else{
		suseconds_t timeFmain = tempsFmain.tv_sec*1000000 + tempsFmain.tv_usec;
		suseconds_t timeDmain = tempsDmain.tv_sec*1000000 + tempsDmain.tv_usec;
		int sec =(int)((timeFmain - timeDmain)/1000000);
		int usec =(int)((timeFmain - timeDmain)-1000000*sec);
		int min = 0;
		if (sec>=60){
		min = sec/60;
		sec = sec%60;
		}
		printf("#############################################################\n");
		printf("--------------- Temps total de l'opération ------------------\n");
		printf("--------------- %02dmin %02dsec %06dus ------------------------\n",min,sec,usec);
		printf("#############################################################\n");
		}
		*/
	return EXIT_SUCCESS;
}/* ----------  end of function main  ---------- */