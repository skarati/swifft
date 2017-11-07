#include "ntt8.h"

vec A[16][8] align;
vec Y[16][8] align;
vec YA[16][8] align;
vec op[8] align;
int opi[8][8] align;

/*======================*/

int x[16][8];
int a[16][64];

int main(){
	
	int count=0;
	
	int temp;
	double cycles[repeat],min;
	int c1,c2;
	int xb[16][8];

	read_Input();
	set_Key();

	/* Computation */
	for(int i=0; i<repeat; i++){	/*1000000*//*around 1450cycles*/
		MEASURE({
			SWIFFT(x);
		});
		cycles[i] = RDTSC_total_clk;
		printf("%lf\n",cycles[i]);
	}

	for(int i=0; i<repeat; i++){
		temp = i;
		for(int j=i+1; j<repeat; j++){
			if(cycles[temp] > cycles[j]){
				temp = j;
			}
		}
		min = cycles[i]; cycles[i] = cycles[temp]; cycles[temp] = min; 
	}
	outputY(opi);
	printf("Median of the required no. of Cycles to Hash 128 bytes to 64 bytes: %f\n",cycles[repeat/2]);
	return 0;
}

void read_Input(){
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<8;j++){
			scanf("%d",&(x[i][j]));
		}
	}
	for(i=0;i<16;i++){
		for(j=0;j<64;j++){
			scanf("%d",&(a[i][j]));
		}
	}

}


inline void set_Key(){
	int y[16][64];
	int i,k,j;
	int t;
	int om_odd_pow[64];
	vec om_base[8], om_temp[8][8], key[16][8],temp[8];
	vec tk0i0k[8];
	vec t1,t2;
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	
	for(int row=0;row<16;row++){
		for(int k0=0; k0 < 8; k0++){
			0[(u32 *) &(key[row][k0])] = a[row][k0+8*0];
			1[(u32 *) &(key[row][k0])] = a[row][k0+8*1];
			2[(u32 *) &(key[row][k0])] = a[row][k0+8*2];
			3[(u32 *) &(key[row][k0])] = a[row][k0+8*3];
			4[(u32 *) &(key[row][k0])] = a[row][k0+8*4];
			5[(u32 *) &(key[row][k0])] = a[row][k0+8*5];
			6[(u32 *) &(key[row][k0])] = a[row][k0+8*6];
			7[(u32 *) &(key[row][k0])] = a[row][k0+8*7];
		}
	}
	
	gntt8(key, A);
}	

inline void SWIFFT(int x[16][8]){
	int i1;
	int k0;
	vec Yt[8];
	vec Y_temp1[8],Y_temp2[8],Y_temp3[8];
	int t;
	vec op_temp[8];
	vec mk0i0, poi1k0;
	int xbyte[8];

	bntt8(x, Y);

	for(int i=0; i<8; i++){
		Y[0][i]  = _mm256_mullo_epi32(Y[0][i], A[0][i]);
		Y[1][i]  = _mm256_mullo_epi32(Y[1][i], A[1][i]);
		Y[2][i]  = _mm256_mullo_epi32(Y[2][i], A[2][i]);
		Y[3][i]  = _mm256_mullo_epi32(Y[3][i], A[3][i]);
		Y[4][i]  = _mm256_mullo_epi32(Y[4][i], A[4][i]);
		Y[5][i]  = _mm256_mullo_epi32(Y[5][i], A[5][i]);
		Y[6][i]  = _mm256_mullo_epi32(Y[6][i], A[6][i]);
		Y[7][i]  = _mm256_mullo_epi32(Y[7][i], A[7][i]);
		Y[8][i]  = _mm256_mullo_epi32(Y[8][i], A[8][i]);
		Y[9][i]  = _mm256_mullo_epi32(Y[9][i], A[9][i]);
		Y[10][i] = _mm256_mullo_epi32(Y[10][i],A[10][i]);
		Y[11][i] = _mm256_mullo_epi32(Y[11][i],A[11][i]);
		Y[12][i] = _mm256_mullo_epi32(Y[12][i],A[12][i]);
		Y[13][i] = _mm256_mullo_epi32(Y[13][i],A[13][i]);
		Y[14][i] = _mm256_mullo_epi32(Y[14][i],A[14][i]);
		Y[15][i] = _mm256_mullo_epi32(Y[15][i],A[15][i]);
		
		op_temp[0] = _mm256_add_epi32(Y[0][i],Y[1][i]);   op_temp[1] = _mm256_add_epi32(Y[2][i],Y[3][i]); 
		op_temp[2] = _mm256_add_epi32(Y[4][i],Y[5][i]);   op_temp[3] = _mm256_add_epi32(Y[6][i],Y[7][i]);
		op_temp[4] = _mm256_add_epi32(Y[8][i],Y[9][i]);   op_temp[5] = _mm256_add_epi32(Y[10][i],Y[11][i]); 
		op_temp[6] = _mm256_add_epi32(Y[12][i],Y[13][i]); op_temp[7] = _mm256_add_epi32(Y[14][i],Y[15][i]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[1]); op_temp[2] = _mm256_add_epi32(op_temp[2],op_temp[3]); 
		op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[5]); op_temp[6] = _mm256_add_epi32(op_temp[6],op_temp[7]);

		op_temp[0] = _mm256_add_epi32(op_temp[0],op_temp[2]); op_temp[4] = _mm256_add_epi32(op_temp[4],op_temp[6]); 

		op[i] = _mm256_add_epi32(op_temp[0],op_temp[4]);	ReduceY_final(&op[i]);
	}

	unpackY(op);

}


void ReduceY(vec *Y){
	vec a0,a1;

	a0 = _mm256_and_si256(*Y,mask255); a0 = _mm256_add_epi32(a0,p257); a1 = _mm256_srai_epi32(*Y,8); *Y = _mm256_sub_epi32(a0,a1);
}
void ReduceY_final(vec *Y){
	vec a0,a1;

	a0 = _mm256_and_si256(*Y,mask255); a0 = _mm256_add_epi32(a0,p257); a1 = _mm256_srai_epi32(*Y,8); *Y = _mm256_sub_epi32(a0,a1);
	a0 = _mm256_and_si256(*Y,mask255); a0 = _mm256_add_epi32(a0,p257); a1 = _mm256_srai_epi32(*Y,8); *Y = _mm256_sub_epi32(a0,a1);

	a0 = _mm256_and_si256(*Y,mask255); a1 = _mm256_srli_epi32(*Y,8); a0 = _mm256_sub_epi32(a0,a1);
	a1 = _mm256_cmpgt_epi32(a0,allone); a1 = _mm256_xor_si256(a1,allone); a1 = _mm256_and_si256(a1,p257);
	*Y = _mm256_add_epi32(a0,a1);
}

inline void unpackY(vec Y[8]){
	for(int i=0;i<8;i++){
	    	opi[i][0] = 0[(u32 *) &Y[i]];
	   	opi[i][1] = 1[(u32 *) &Y[i]];
	    	opi[i][2] = 2[(u32 *) &Y[i]];
	    	opi[i][3] = 3[(u32 *) &Y[i]];
	    	opi[i][4] = 4[(u32 *) &Y[i]];
	   	opi[i][5] = 5[(u32 *) &Y[i]];
	    	opi[i][6] = 6[(u32 *) &Y[i]];
	    	opi[i][7] = 7[(u32 *) &Y[i]];
	}
}
inline void outputY(u32 Y[8][8]){
	printf("\n\n\n\n");
	for(int i1=0;i1<8;i1++){
		for(int i0=0;i0<8;i0++)
			printf("%d ",Y[i1][i0]);
	}
	printf("\n\n\n\n");
}

