#include "swifft-avx2-16.h"

inline void bntt16(int xbyte[16][4], vec Y[16][4]){
	for(int row=0;row<16;row++){	
		vecMult16Reduce1(M_K0_I0[0], T_K0_I0[xbyte[row][0]], Y_temp1[0]);
		vecMult16Reduce1(M_K0_I0[1], T_K0_I0[xbyte[row][1]], Y_temp1[1]);
		vecMult16Reduce1(M_K0_I0[2], T_K0_I0[xbyte[row][2]], Y_temp1[2]);
		vecMult16Reduce1(M_K0_I0[3], T_K0_I0[xbyte[row][3]], Y_temp1[3]);

		Y_temp2[0] = _mm256_add_epi16(Y_temp1[0],Y_temp1[2]);
		Y_temp2[1] = _mm256_add_epi16(Y_temp1[1],Y_temp1[3]);
		Y[row][0] = _mm256_add_epi16(Y_temp2[0],Y_temp2[1]);
		
		Y_temp2[2] = _mm256_add_epi16(Y_temp2[0],p257_2);
		Y[row][2] = _mm256_sub_epi16(Y_temp2[2],Y_temp2[1]);
				
		
		Y_temp2[0] = _mm256_add_epi16(Y_temp1[0],p257_2);
		Y_temp2[1] = _mm256_sub_epi16(Y_temp2[0],Y_temp1[2]);

		Y_temp2[2] = _mm256_add_epi16(Y_temp1[1],p257_2);
		Y_temp2[3] = _mm256_sub_epi16(Y_temp2[2],Y_temp1[3]);
		Y_temp2[4] = _mm256_slli_epi16(Y_temp2[3],4);
		Y[row][1] = _mm256_add_epi16(Y_temp2[1],Y_temp2[4]);
		
		Y_temp2[1] = _mm256_add_epi16(Y_temp2[1],p257_5);
		Y[row][3] = _mm256_sub_epi16(Y_temp2[1],Y_temp2[4]);
	}
}
inline void gntt16(vec key[16][4], vec A[16][4]){
	vec t1,t2,t3,t4;
	vec temp[4];
	for(int row=0;row<16;row++){
		for(int k0=0; k0<4; k0++){
			temp[k0] = zero;
			for(int k1=0; k1<8; k1++){
				t1 = _mm256_permutevar8x32_epi32(key[row][k0], permute[k1]);	
				t2 = _mm256_and_si256(t1,selec[1]);
				t1 = _mm256_and_si256(t1,selec[0]);
				t3 = _mm256_slli_epi32(t1,16);
				t4 = _mm256_srli_epi32(t2,16);
				t1 = _mm256_or_si256(t1,t3);
				t2 = _mm256_or_si256(t2,t4);

				
				vecMult16Reduce3(t1, k0i0[2*k1], t3); //print(t3);
				vecMult16Reduce3(t2, k0i0[2*k1+1], t4);
				temp[k0]  = _mm256_add_epi16(temp[k0],t3); 	//ReduceY2(&temp[k0]);
				temp[k0]  = _mm256_add_epi16(temp[k0],t4); 	ReduceY2(&temp[k0]);
			}
		}

		vecMult16Reduce3(M_K0_I0[0], temp[0], Y_temp1[0]);
		vecMult16Reduce3(M_K0_I0[1], temp[1], Y_temp1[1]);
		vecMult16Reduce3(M_K0_I0[2], temp[2], Y_temp1[2]);
		vecMult16Reduce3(M_K0_I0[3], temp[3], Y_temp1[3]);

		Y_temp2[0] = _mm256_add_epi16(Y_temp1[0],Y_temp1[2]);
		Y_temp2[1] = _mm256_add_epi16(Y_temp1[1],Y_temp1[3]);
		A[row][0] = _mm256_add_epi16(Y_temp2[0],Y_temp2[1]);	ReduceY2(&A[row][0]);//ReduceY2(&A[row][0]);
		
		Y_temp2[2] = _mm256_add_epi16(Y_temp2[0],p257_2);
		A[row][2] = _mm256_sub_epi16(Y_temp2[2],Y_temp2[1]);	ReduceY2(&A[row][2]);
				
		
		Y_temp2[0] = _mm256_add_epi16(Y_temp1[0],p257_2);
		Y_temp2[1] = _mm256_sub_epi16(Y_temp2[0],Y_temp1[2]);

		Y_temp2[2] = _mm256_add_epi16(Y_temp1[1],p257_2);
		Y_temp2[3] = _mm256_sub_epi16(Y_temp2[2],Y_temp1[3]);
		Y_temp2[4] = _mm256_slli_epi16(Y_temp2[3],4);
		A[row][1] = _mm256_add_epi16(Y_temp2[1],Y_temp2[4]);	ReduceY2(&A[row][1]);
		
		Y_temp2[1] = _mm256_add_epi16(Y_temp2[1],p257_7);
		A[row][3] = _mm256_sub_epi16(Y_temp2[1],Y_temp2[4]);	ReduceY2(&A[row][3]);

	}	

}

