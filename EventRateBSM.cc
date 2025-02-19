#include <iostream>
#include <cmath>
#include <string>
#include<float.h>
#include<complex.h>
#include <vector>
#include<gsl/gsl_complex.h>
#include<gsl/gsl_complex_math.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_blas.h>
#include <globes/globes.h>
#include<fstream>

#include <algorithm>

extern "C"
{
	#include "bsm.h"
}

using namespace std;

char AEDLFILE[] = "DUNE_GLoBES.glb";
string OUTFILEe = "eventrate_e_cmue_1.0e-24_arg4.dat";
string OUTFILEmu = "eventrate_mu_cmue_1.0e-24_arg4.dat";
FILE * outstde = NULL;
FILE * outstdmu = NULL;

int main(int argc, char * argv[])
{

	glbInit(argv[0]);
	glbInitExperiment(AEDLFILE, &glb_experiment_list[0], &glb_num_of_exps);

	outstde = fopen(OUTFILEe.c_str(), "w");
	if (outstde == NULL) 
    {
        printf("Error opening output file.\n");
		return -1;
	}

	outstdmu = fopen(OUTFILEmu.c_str(), "w");
	if (outstdmu == NULL) 
    {
        printf("Error opening output file.\n");
		return -1;
	}

	double dm21 = 7.42e-5;
	double dm31 = 2.51e-3;
	double theta12 = asin(sqrt(0.304));
	double theta23 = asin(sqrt(0.449));
	double theta13 = asin(sqrt(0.02246));
	double deltacp = 1.27 * M_PI;

    bsm_init_probability_engine_3();

	glbRegisterProbabilityEngine(8 * 9 - 3,
                               &bsm_probability_matrix,
							   &bsm_set_oscillation_parameters,
  							   &bsm_get_oscillation_parameters,
  							   NULL);

	/* Define "true" oscillation parameter vector */
	glb_params true_values = glbAllocParams();

    for(unsigned int i=0; i < 69; i++)
	{
	glbSetOscParams(true_values, 0.0, i);
	}

    glbDefineParams(true_values,theta12,theta13,theta23,deltacp,dm21,dm31);
	
    double abs_a_ee = 0;
	double abs_a_mue = 0;
	double arg_a_mue = 0;
	double abs_a_etau = 0;
	double arg_a_etau = 0;
	double abs_a_mumu = 0;
	double abs_a_mutau = 0;
	double arg_a_mutau = 0;
	double abs_a_tautau = 0;

	double abs_c_ee = 0;
	double abs_c_mue = 1.0 * 10e-24 * 10e9;//-2.0e-25 * 1.0e9;
	double arg_c_mue = 2 * M_PI;
	double abs_c_etau = 0;
	double arg_c_etau = 0;
	double abs_c_mumu = 0;//1.0e-22;
	double abs_c_mutau = 0;
	double arg_c_mutau = 0;
	double abs_c_tautau = 0;
    
    //############ LIV Parameter #################################//
	glbSetOscParams(true_values, abs_a_ee, 51);  // a_ee 
	glbSetOscParams(true_values, abs_a_mue, 52);  // a_mue magnitude
    glbSetOscParams(true_values, arg_a_mue, 53);  // a_mue phase
    glbSetOscParams(true_values, abs_a_etau, 54);  // a_etau 
    glbSetOscParams(true_values, arg_a_etau, 55);  // a_etau phase
    glbSetOscParams(true_values, abs_a_mumu, 56);  // a_mumu
    glbSetOscParams(true_values, abs_a_mutau, 57);  // a_mutau
    glbSetOscParams(true_values, arg_a_mutau, 58);  // a_mutau phase
    glbSetOscParams(true_values, abs_a_tautau, 59);  // a_tautau

	glbSetOscParams(true_values, abs_c_ee, 60);  // c_ee 
	glbSetOscParams(true_values, abs_c_mue, 61);  // c_mue magnitude
    glbSetOscParams(true_values, arg_c_mue, 62);  // c_mue phase
    glbSetOscParams(true_values, abs_c_etau, 63);  // c_etau 
    glbSetOscParams(true_values, arg_c_etau, 64);  // c_etau phase
    glbSetOscParams(true_values, abs_c_mumu, 65);  // c_mumu
    glbSetOscParams(true_values, abs_c_mutau, 66);  // c_mutau
    glbSetOscParams(true_values, arg_c_mutau, 67);  // c_mutau phase
    glbSetOscParams(true_values, abs_c_tautau, 68);  // c_tautau
    
    glbSetDensityParams(true_values, 1.0, GLB_ALL);

    glb_params input_errors = glbAllocParams();
  	
	glbSetDensityParams(input_errors, 0.05, GLB_ALL);

	glbSetOscillationParameters(true_values);
	glbSetRates();

	int glbNameToValue(int exp, const char* context, const char *name);
    int glbShowRuleRates(FILE* stream, int exp, int rule, int pos, int effi, int bgi, int coeffi, int signal);

    //int ruleosc = glbNameToValue(0, "rule", "#Nu_Mu_Appearance");
    int ruleosce = glbNameToValue(0, "rule", "#nue_app");
    printf("%d\n",ruleosce);
    glbShowRuleRates(outstde, 0, ruleosce, GLB_ALL, GLB_W_EFF, GLB_W_BG, GLB_W_COEFF, GLB_SIG);

	int ruleoscmu = glbNameToValue(0, "rule", "#numu_dis");
    printf("%d\n",ruleoscmu);
    glbShowRuleRates(outstdmu, 0, ruleoscmu, GLB_ALL, GLB_W_EFF, GLB_W_BG, GLB_W_COEFF, GLB_SIG);


    //const char *glbValueToName(int exp,const char* context, int value); 
    //const char* name = glbValueToName(0, "rule", 2);
    //printf("%s\n",name);

	fclose(outstde);
	fclose(outstdmu);
	
	glbFreeParams(true_values);
 	return 0;

}