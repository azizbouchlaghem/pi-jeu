#include"collisionparfaite.h"

SDL_Color getpixel(SDL_Surface *pSurface,int x,int y)//lezem n3dilha el masque 5tr hia fih elloun ak7el //mini
{	
	SDL_Color color;
	Uint32 col=0;
	char* pPosition=(char* ) pSurface->pixels;
	pPosition+= (pSurface->pitch * y);
	pPosition+= (pSurface->format->BytesPerPixel *x);
	memcpy(&col ,pPosition ,pSurface->format->BytesPerPixel);
	SDL_GetRGB(col,pSurface->format, &color.r, &color.g, &color.b);
	return (color);
}
int  collisionparfaite(SDL_Surface *psurface,personne p)
{	
	int tabx[7],taby[7],i;//7atit les position de x fi tableau mta3 x o hatit les position mat3 y fi tableau mta3 y o 3mlt coleur chntstih bih
	SDL_Color color1,color;//color lil test 
	color1.r = 0;
	color1.g = 0;
	color1.b = 0;
	tabx[0]=p.pos_perso.x;
	tabx[1]=(p.pos_perso.x)+((p.pos_perso.w)/2);
	tabx[2]=(p.pos_perso.x+p.pos_perso.w);
	tabx[3]=p.pos_perso.x;
	tabx[4]=p.pos_perso.x;
	tabx[5]=(p.pos_perso.x)+((p.pos_perso.w)/2);
	tabx[6]=(p.pos_perso.x+p.pos_perso.w);
	tabx[7]=(p.pos_perso.x+p.pos_perso.w);
	taby[0]=p.pos_perso.y;
	taby[1]=p.pos_perso.y;
	taby[2]=p.pos_perso.y;
	taby[3]=(p.pos_perso.y)+((p.pos_perso.h)/2);
	taby[4]=(p.pos_perso.y+p.pos_perso.h);
	taby[5]=(p.pos_perso.y+p.pos_perso.h);
	taby[6]=(p.pos_perso.y+p.pos_perso.h);
	taby[7]=(p.pos_perso.y)+((p.pos_perso.h)/2);
	
	for(i=0;i<8;i++)
	{
	color=getpixel(psurface,tabx[i],taby[i]);//trj3lik el coleur ili inty fih 
	if(color.r==color1.r && color.b==color1.b && color.g==color1.g)
	{
		return i;//trj3li indice win saret collision 

	}
	}
	return 10;//maasartch collision kain raja3li 1
	
}	

void collision(SDL_Surface *psurface,personne *p)//il suffit saret collision scrolling leze nwkfo wktli collisionparfaite traja3 10 ya3ni mfmfch collision 
{	int i;
	i=collisionparfaite(psurface,*p);

	switch (i)//wkt tsir collisision el perso idizo chwya 
	{
		case 0 :p->pos_perso.y=p->pos_perso.y+20;
			break;
		case 1 :p->pos_perso.y=p->pos_perso.y+20;
			 break;
		case 2 :p->pos_perso.y=p->pos_perso.y+20;
			break;
		case 3 :p->pos_perso.x=p->pos_perso.x+20;
			break;
	        case 4 :p->pos_perso.y=p->pos_perso.y-20;
			break;
		case 5 :p->pos_perso.y=p->pos_perso.y-50; 
			break;
		case 6 :p->pos_perso.y=p->pos_perso.y-30; 
			break;
		case 7 :p->pos_perso.x=p->pos_perso.x-20;
			break;
		

	}
}


