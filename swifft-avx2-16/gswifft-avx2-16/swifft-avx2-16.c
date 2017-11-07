#include "ntt16.h"

int main(){
	
	int xbyte[16][4];
	double cycles[repeat],min;
	int temp;
	/* Set Up part */
	
	//Set_Mask();
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
	outputY(op);
	printf("Median of the required no. of Cycles to Hash 128 bytes to 64 bytes: %f\n",cycles[repeat/2]);
	return 0;
	
	return 0;
}


void read_Input(){
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<64;j++){
			scanf("%d",&(x[i][j]));
		}
	}
	for(i=0;i<16;i++){
		for(j=0;j<64;j++){
			scanf("%d",&(a[i][j]));
		}
	}
}

void print_vec(vec Y){
	uu16 opi[16];
	//for(int i=0;i<8;i++){
	    	opi[0]  =  0[(uu16 *) &Y];
	   	opi[1]  =  1[(uu16 *) &Y];
	    	opi[2]  =  2[(uu16 *) &Y];
	    	opi[3]  =  3[(uu16 *) &Y];
	    	opi[4]  =  4[(uu16 *) &Y];
	   	opi[5]  =  5[(uu16 *) &Y];
	    	opi[6]  =  6[(uu16 *) &Y];
	    	opi[7]  =  7[(uu16 *) &Y];
	    	opi[8]  =  8[(uu16 *) &Y];
	   	opi[9]  =  9[(uu16 *) &Y];
	    	opi[10] = 10[(uu16 *) &Y];
	    	opi[11] = 11[(uu16 *) &Y];
	    	opi[12] = 12[(uu16 *) &Y];
	   	opi[13] = 13[(uu16 *) &Y];
	    	opi[14] = 14[(uu16 *) &Y];
	    	opi[15] = 15[(uu16 *) &Y];
	//}	
		printf("[");
		for(int i0=0;i0<16;i0++)
			printf("%hu, ",opi[i0]);
		printf("]\n");

}


inline void parse(int xbyte[16][4], int x[16][8]){
	int i,j,k;
	
	for(int row=0;row<16;row++){
		for(i=0; i<4; i++){
			xbyte[row][i] = x[row][2*i]+(x[row][2*i+1]<<8);
		}
	}

}

void ReduceY1(vec *Y){
	vec a0,a1;

	a0 = _mm256_and_si256(*Y,mask255); a1 = _mm256_srli_epi16(*Y,8); *Y = _mm256_sub_epi16(a0,a1);
}

void ReduceY2(vec *Y){
	vec a0,a1;

	a0 = _mm256_and_si256(*Y,mask255); a1 = _mm256_srli_epi16(*Y,8); *Y = _mm256_sub_epi16(a0,a1);
	a0 = _mm256_cmpgt_epi16(*Y,allone);//print(*Y);
	a0 = _mm256_xor_si256(a0,allone); a0 = _mm256_and_si256(a0,p257);
	*Y = _mm256_add_epi16(*Y,a0);//print(*Y);
	
}

//T0 = _mm256_and_si256(I0, I1); T1 = _mm256_and_si256(T0, p256);	T2 = _mm256_srli_epi16(T1,8); \



inline int SWIFFT(int x[16][64]){
	int i1;
	int k0;
	vec Yt[8];
	int t;
	vec op_temp[8];
	vec mk0i0, poi1k0;
	vec X[16][4],Y[16][4];

	for(int row=0;row<16;row++){
		for(int k0=0; k0 < 4; k0++){
			0[(u16 *) &(X[row][k0])]  = x[row][16*k0+0];
			1[(u16 *) &(X[row][k0])]  = x[row][16*k0+1];
			2[(u16 *) &(X[row][k0])]  = x[row][16*k0+2];
			3[(u16 *) &(X[row][k0])]  = x[row][16*k0+3];
			4[(u16 *) &(X[row][k0])]  = x[row][16*k0+4];
			5[(u16 *) &(X[row][k0])]  = x[row][16*k0+5];
			6[(u16 *) &(X[row][k0])]  = x[row][16*k0+6];
			7[(u16 *) &(X[row][k0])]  = x[row][16*k0+7];
			8[(u16 *) &(X[row][k0])]  = x[row][16*k0+8];
			9[(u16 *) &(X[row][k0])]  = x[row][16*k0+9];
			10[(u16 *) &(X[row][k0])] = x[row][16*k0+10];
			11[(u16 *) &(X[row][k0])] = x[row][16*k0+11];
			12[(u16 *) &(X[row][k0])] = x[row][16*k0+12];
			13[(u16 *) &(X[row][k0])] = x[row][16*k0+13];
			14[(u16 *) &(X[row][k0])] = x[row][16*k0+14];
			15[(u16 *) &(X[row][k0])] = x[row][16*k0+15];
		}
	}

	gntt16(X,Y);
	
	for(int i=0; i<4; i++){
		vecMult16Reduce3(Y[0][i],  A[0][i],  Y_temp1[0]);
		vecMult16Reduce3(Y[1][i],  A[1][i],  Y_temp1[1]);
		vecMult16Reduce3(Y[2][i],  A[2][i],  Y_temp1[2]);
		vecMult16Reduce3(Y[3][i],  A[3][i],  Y_temp1[3]);
		vecMult16Reduce3(Y[4][i],  A[4][i],  Y_temp1[4]);
		vecMult16Reduce3(Y[5][i],  A[5][i],  Y_temp1[5]);
		vecMult16Reduce3(Y[6][i],  A[6][i],  Y_temp1[6]);
		vecMult16Reduce3(Y[7][i],  A[7][i],  Y_temp1[7]);
		vecMult16Reduce3(Y[8][i],  A[8][i],  Y_temp1[8]);
		vecMult16Reduce3(Y[9][i],  A[9][i],  Y_temp1[9]);
		vecMult16Reduce3(Y[10][i], A[10][i], Y_temp1[10]);
		vecMult16Reduce3(Y[11][i], A[11][i], Y_temp1[11]);
		vecMult16Reduce3(Y[12][i], A[12][i], Y_temp1[12]);
		vecMult16Reduce3(Y[13][i], A[13][i], Y_temp1[13]);
		vecMult16Reduce3(Y[14][i], A[14][i], Y_temp1[14]);
		vecMult16Reduce3(Y[15][i], A[15][i], Y_temp1[15]);

		op_temp[0] = _mm256_add_epi16(Y_temp1[0],Y_temp1[1]);   op_temp[1] = _mm256_add_epi16(Y_temp1[2],Y_temp1[3]); 
		op_temp[2] = _mm256_add_epi16(Y_temp1[4],Y_temp1[5]);   op_temp[3] = _mm256_add_epi16(Y_temp1[6],Y_temp1[7]);
		op_temp[4] = _mm256_add_epi16(Y_temp1[8],Y_temp1[9]);   op_temp[5] = _mm256_add_epi16(Y_temp1[10],Y_temp1[11]); 
		op_temp[6] = _mm256_add_epi16(Y_temp1[12],Y_temp1[13]); op_temp[7] = _mm256_add_epi16(Y_temp1[14],Y_temp1[15]);

		op_temp[0] = _mm256_add_epi16(op_temp[0],op_temp[1]); op_temp[2] = _mm256_add_epi16(op_temp[2],op_temp[3]); 
		op_temp[4] = _mm256_add_epi16(op_temp[4],op_temp[5]); op_temp[6] = _mm256_add_epi16(op_temp[6],op_temp[7]);

		op_temp[0] = _mm256_add_epi16(op_temp[0],op_temp[2]); op_temp[4] = _mm256_add_epi16(op_temp[4],op_temp[6]); 

		op[i] = _mm256_add_epi16(op_temp[0],op_temp[4]);	ReduceY2(&op[i]);
		
	}
	
	return unpackY(op);

}



inline void set_Key(){
	int y[16][64];
	int i,k,j;
	int t;
	int om_odd_pow[64];
	vec om_base[8], om_temp[8][8], key[16][4],temp[4];
	vec tk0i0k[8];
	
	vec Y_temp1[4],Y_temp2[8],Y_temp3[8];


	for(int row=0;row<16;row++){
		for(int k0=0; k0 < 4; k0++){
			0[(u16 *) &(key[row][k0])]  = a[row][k0+4*0];
			1[(u16 *) &(key[row][k0])]  = a[row][k0+4*1];
			2[(u16 *) &(key[row][k0])]  = a[row][k0+4*2];
			3[(u16 *) &(key[row][k0])]  = a[row][k0+4*3];
			4[(u16 *) &(key[row][k0])]  = a[row][k0+4*4];
			5[(u16 *) &(key[row][k0])]  = a[row][k0+4*5];
			6[(u16 *) &(key[row][k0])]  = a[row][k0+4*6];
			7[(u16 *) &(key[row][k0])]  = a[row][k0+4*7];
			8[(u16 *) &(key[row][k0])]  = a[row][k0+4*8];
			9[(u16 *) &(key[row][k0])]  = a[row][k0+4*9];
			10[(u16 *) &(key[row][k0])] = a[row][k0+4*10];
			11[(u16 *) &(key[row][k0])] = a[row][k0+4*11];
			12[(u16 *) &(key[row][k0])] = a[row][k0+4*12];
			13[(u16 *) &(key[row][k0])] = a[row][k0+4*13];
			14[(u16 *) &(key[row][k0])] = a[row][k0+4*14];
			15[(u16 *) &(key[row][k0])] = a[row][k0+4*15];
		}
	}

	gntt16(key,A);
}


int unpackY(vec Y[4]){
	u32 opi[4][16];
	for(int i=0;i<4;i++){
	    	opi[i][0]  = 0[(u16 *) &Y[i]];
	   	opi[i][1]  = 1[(u16 *) &Y[i]];
	    	opi[i][2]  = 2[(u16 *) &Y[i]];
	    	opi[i][3]  = 3[(u16 *) &Y[i]];
	    	opi[i][4]  = 4[(u16 *) &Y[i]];
	   	opi[i][5]  = 5[(u16 *) &Y[i]];
	    	opi[i][6]  = 6[(u16 *) &Y[i]];
	    	opi[i][7]  = 7[(u16 *) &Y[i]];
	    	opi[i][8]  = 8[(u16 *) &Y[i]];
	   	opi[i][9]  = 9[(u16 *) &Y[i]];
	    	opi[i][10] = 10[(u16 *) &Y[i]];
	    	opi[i][11] = 11[(u16 *) &Y[i]];
	    	opi[i][12] = 12[(u16 *) &Y[i]];
	   	opi[i][13] = 13[(u16 *) &Y[i]];
	    	opi[i][14] = 14[(u16 *) &Y[i]];
	    	opi[i][15] = 15[(u16 *) &Y[i]];
	}
	return opi[0][0];
}

void outputY(vec Y[4]){
	u32 opi[4][16];
	for(int i=0;i<4;i++){
	    	opi[i][0]  = 0[(u16 *) &Y[i]];
	   	opi[i][1]  = 1[(u16 *) &Y[i]];
	    	opi[i][2]  = 2[(u16 *) &Y[i]];
	    	opi[i][3]  = 3[(u16 *) &Y[i]];
	    	opi[i][4]  = 4[(u16 *) &Y[i]];
	   	opi[i][5]  = 5[(u16 *) &Y[i]];
	    	opi[i][6]  = 6[(u16 *) &Y[i]];
	    	opi[i][7]  = 7[(u16 *) &Y[i]];
	    	opi[i][8]  = 8[(u16 *) &Y[i]];
	   	opi[i][9]  = 9[(u16 *) &Y[i]];
	    	opi[i][10] = 10[(u16 *) &Y[i]];
	    	opi[i][11] = 11[(u16 *) &Y[i]];
	    	opi[i][12] = 12[(u16 *) &Y[i]];
	   	opi[i][13] = 13[(u16 *) &Y[i]];
	    	opi[i][14] = 14[(u16 *) &Y[i]];
	    	opi[i][15] = 15[(u16 *) &Y[i]];
	}

	for(int i1=0;i1<4;i1++){
		for(int i0=0;i0<16;i0++)
			printf("%hu ",opi[i1][i0]%257);
		//printf("\n");
	}

	printf("\n\n\n\n");
}

void print(vec Y){
	u32 opi[1][16];
	for(int i=0;i<1;i++){
	    	opi[i][0]  = 0[(u16 *) &Y[i]];
	   	opi[i][1]  = 1[(u16 *) &Y[i]];
	    	opi[i][2]  = 2[(u16 *) &Y[i]];
	    	opi[i][3]  = 3[(u16 *) &Y[i]];
	    	opi[i][4]  = 4[(u16 *) &Y[i]];
	   	opi[i][5]  = 5[(u16 *) &Y[i]];
	    	opi[i][6]  = 6[(u16 *) &Y[i]];
	    	opi[i][7]  = 7[(u16 *) &Y[i]];
	    	opi[i][8]  = 8[(u16 *) &Y[i]];
	   	opi[i][9]  = 9[(u16 *) &Y[i]];
	    	opi[i][10] = 10[(u16 *) &Y[i]];
	    	opi[i][11] = 11[(u16 *) &Y[i]];
	    	opi[i][12] = 12[(u16 *) &Y[i]];
	   	opi[i][13] = 13[(u16 *) &Y[i]];
	    	opi[i][14] = 14[(u16 *) &Y[i]];
	    	opi[i][15] = 15[(u16 *) &Y[i]];
	}

	for(int i1=0;i1<1;i1++){
		for(int i0=0;i0<16;i0++)
			printf("%hu ",opi[i1][i0]);
		//printf("\n");
	}

	printf("\n\n\n\n");
}



