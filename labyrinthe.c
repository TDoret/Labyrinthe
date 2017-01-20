
/*1-	Haut 
2-	Droite
3-	Bas
4-	Gauche

i = ligne
j = colonne*/

void generateMaze(int M, int N)
{
	int tabV[M][N] = {0}
	int tabX[M][N+1] = {0}
	int tabY[M+1][N] = {0} 

	int i = rand % M-1;
	int j = rand % N-1;
	int cpt = 0;
	int nbCases = M*N;
		while (cpt  <= nbCases) {
			tabV[i][j] = 1;
			//todo tant que inférieur à nb cases (M*N)
			//on checke qu'il puisse aller dans une direction random
			int isValidDirect = 0;
			int isValidUp = 0;
			int isValidRight = 0;
			int isValidDown = 0;
			int isValidLeft = 0;	
			//tant que la direction n'est pas bonne
			do {
				//si toutes les directions ne sont pas bonnes, on retourne au dernier noeud
				if(isValidUp == -1 && isValidRight == -1 && isValidDown == -1 && isValidLeft == -1)
				{
					break;
				}
				int dir = (rand % 4) + 1;
				//checker si c'est pas borné
				switch(dir) {
					case 1 :
						if (i > 0)
							isValidDirect = 1; 
						else
							isValidUp = -1;
						break;
					case 2 : 
						if (j < n-1)
							isValidDirect = 1;
						else
							isValidRight = -1;
						break;
					case 3 : 
						if (i < n-1)
							isValidDirect = 1;
						else
							isValidDown = -1;
						break;
					case 4 : 
						if (j > 0)
							isValidDirect = 1;
						else
							isValidLeft = -1;
						break;
					default : 
						break;
				}

				if (isValidDirect == 1) {
					//faire une liste qui stocke les coordonnées courantes avant de se déplacer
					//on check si on a pas déjà visité la case vers où on veut aller
					switch(dir) {
						case 1 :
							if(tabV[i-1][j] != 0)
							{
								isValidDirect = 0;
								isValidUp = -1;
							}
							break;
						case 2 :
							if(tabV[i][j+1] != 0)
							{
								isValidDirect = 0;
								isValidRight = -1;
							}
							break;
						case 3 :
							if(tabV[i+1][j] != 0)
							{
								isValidDirect = 0;
								isValidDown = -1;
							}
							break;
						case 4 :
							if(tabV[i][j-1] != 0)
							{
								isValidDirect = 0;
								isValidLeft = -1;
							}
							break;
						default :
							break ;
					}
				}

			}while(isValidDirect == 0)

			//si on est sorti, peut casser le mur
			if (isValidDirect == 1) {
				switch (dir) {
					case 1 :
						//on casse le mur
						tabX[i][j] = 1;
						//on stocke la position courante
						//on se déplace
						i++;
						//on incrémente le compteur
						cpt ++;
						break;
					case 2 :
						tabY[i][j+1] = 1;
						j++;
						//on incrémente le compteur
						cpt ++;
						break;
					case 3 :
						tabX[i+1][j] = 1;
						i--;
						//on incrémente le compteur
						cpt ++;
						break;
					case 4 :
						tabY[i][j] = 1;
						j--;
						//on incrémente le compteur
						cpt ++;
						break;
					default :
						break;
				}
			}
		}
		
}








