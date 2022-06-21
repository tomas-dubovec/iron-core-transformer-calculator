#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

///////////////////   DO NOT USE scanf function, it separates input on first [SPACE] !!!!!  ;   /////////////////////
// pro tip: loop through elements in the array of ArrayWithLength search for NULL, if yes, try to find out...

const double DEFAULT_EFFICIENCY_MULTIPLIER = 0.8;
double DEFAULT_EFFICIENCY_DEVIDER = 1 / DEFAULT_EFFICIENCY_MULTIPLIER;
int MAX_INPUT_LENGTH = 100;

struct DoubleArrayWithLength{
    double *Array;
    int Length;
};
struct ArrayWithLength{
    double **Array;
    int Length;
};

struct Transformer{
    struct ArrayWithLength PrimaryVoltage;  
    struct ArrayWithLength PrimaryCurrent;
    struct ArrayWithLength PrimaryPower;
    struct ArrayWithLength PrimaryDiameter;
    struct ArrayWithLength PrimaryNumberOfTurns;
    struct ArrayWithLength Q;
    struct ArrayWithLength N;    
    struct ArrayWithLength Efficiency;
    struct ArrayWithLength SecondaryPowerTotal;
    struct ArrayWithLength SecondariesPowers;  
    struct ArrayWithLength SecondariesVoltages;
    struct ArrayWithLength SecondariesCurrents;
    struct ArrayWithLength SecondariesNumberOfTurns;
    struct ArrayWithLength SecondariesDiameters;

};



char* QuestionsInOrder[] = {"Primary Voltage: ", "Primary Current: ","Primary Power: ",
 "Primary Diameter: ","Primary number of turns: ","Cross section of Core: ","Turns per 1 Volt: ",
"Core Efficiency (Default 0.8): ","Total power of secondaries: ", "Powers of secondaries (Remain order!): ",
"Voltages of secondaries(Remain order!): ","Currents of secondaries(Remain order!): ",
"Numbers of turns of secondaries(Remain order!): ","Diameters of secondaries(Remain order!): "};


double calcP(double Voltage, double Current){
    double Power = Voltage * Current;
    return Power;
}

// double calcPsecondary(double *Secondaries){
//     double Psecondary = 0;
//     for( int i = 0; i < arrayDoubleLength(Secondaries); i++){    // problem in arrayDoubleLength()
//         Psecondary += (Secondaries[i]); 
//     }
//     return Psecondary;
// }
double calcPprimary(double Psecondary, double EficienyDevider){
    double Pprimary = (Psecondary / EficienyDevider);
    return Pprimary;
}
double calcQ(double Pprimary, double EficiencyMultiplier){
    double Q = EficiencyMultiplier * sqrt(Pprimary);
    return Q;
}
double calcN(double TransformerConstant, double Q){
    double N = TransformerConstant / Q;
    return N;
}
double calcD(double Current){
    double D = sqrt(Current / 2 );  // 2 is constant - see fyzics
    return D;
}
double calcS(double D){
    double S = (double)3.141 * pow( (D/2), 2);
    return S;
}
double calcI(double Power, double Voltage){
    double Current = Power/ Voltage; 
    return Current;
}
double calcTurns(double N, double Voltage){
    double Turns = N * Voltage;
    return Turns;
}
double calcU(double Power, double Current){
    double U = Power / Current;
    return U;
}

struct ArrayWithLength strToDoubleArrayWithLength(char *str){
    struct ArrayWithLength ReturnArrayWithLength;
    if(strlen(str) <= 0){
        printf("Here");
        ReturnArrayWithLength.Array = NULL;
        ReturnArrayWithLength.Length = 0;

        return ReturnArrayWithLength;
    }

    double** Nums = malloc(strlen(str) * sizeof(int));
    char* buffer = malloc((strlen(str)+1) * sizeof(char));  // +1 for null terminator
    int bufferStack = 0;
    int numStack = 0;

        
        for(int i = 0; i <= strlen(str); i++){   
            //printf("%c", str[i]);

            if( (str[i] < 58 && str[i] > 47) || str[i] == '.' || str[i] == ','){   
                buffer[bufferStack] = str[i];
                bufferStack++;
                //printf("Got where normal num should get");
            }
            else if(str[i] == ' ' || str[i] == '\n' || str[i] == '?' || str[i] == '\0'){ // zatial tomu chapem - T.D. 31.5.2022, stale 7.6.2022
                if(bufferStack > 0){
                    buffer[bufferStack] = '\0';

                    double* oneUse = malloc(sizeof(double));
                    oneUse[0] = atof(buffer);
                    Nums[numStack] = oneUse;

                    bufferStack = 0;
                    numStack++; // number of elements
                    printf("(%lf) \n", oneUse[0] );
                }
                if(str[i] == '?'){
                    Nums[numStack] = NULL; // another element is empty.
                    numStack++;
                    bufferStack = 0;  // making sure it is nulled out.

                }
            }
                
            else {
                printf("Invalid token: %c ignoring it. \n", str[i]);
            }
            
        } 
    
    if (numStack != 0) {
        void *AllocatedMemory = realloc(Nums, numStack* sizeof(int));
        free(buffer);
        if (AllocatedMemory == NULL){   // not sure if not useless
            printf("Function 'strToDoubleArrayWithLength' says: ERROR: Array poiter empty!, exiting program !");
            exit(1);
        }
        
        ReturnArrayWithLength.Array = Nums;
        ReturnArrayWithLength.Length = numStack;
        return ReturnArrayWithLength;
    }
    else{
        printf("Function 'strToDoubleArrayWithLength' says: INVALID input. Acting if value (or values) is (are) UNKNOWN . \n");
        free(Nums);
        free(buffer);
        double **emptyArray = malloc(sizeof(double));
        emptyArray[0] = NULL;
        ReturnArrayWithLength.Array = emptyArray; // not working null output
        ReturnArrayWithLength.Length = 1;
        return ReturnArrayWithLength;
    }
    
}

struct Transformer fillTransformerList(char **Questions){

   

    char *MultiUseValue = malloc(MAX_INPUT_LENGTH * sizeof(char));  
    
    struct Transformer CurrentTransformer;

    printf("%s", Questions[0]);     // inputing two numbers does push its second value to another questions input
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);  // give pointer pushed by 1 on one side (who knows? + or -) to not parse into restricted memory
    CurrentTransformer.PrimaryVoltage = strToDoubleArrayWithLength(MultiUseValue);

    printf("%s", Questions[1]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.PrimaryCurrent = strToDoubleArrayWithLength(MultiUseValue);

    
    printf("%s", Questions[2]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.PrimaryPower = strToDoubleArrayWithLength(MultiUseValue);
    
    printf("%s", Questions[3]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.PrimaryDiameter = strToDoubleArrayWithLength(MultiUseValue);
        
    printf("%s", Questions[4]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.PrimaryNumberOfTurns = strToDoubleArrayWithLength(MultiUseValue);
    
    printf("%s", Questions[5]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.Q = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[6]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.N = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[7]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.Efficiency = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[8]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondaryPowerTotal = strToDoubleArrayWithLength(MultiUseValue); 
    
//////////////////////////////////////////////////////
    printf("%s", Questions[9]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondariesPowers = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[10]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondariesVoltages  = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[11]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondariesCurrents = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[12]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondariesNumberOfTurns = strToDoubleArrayWithLength(MultiUseValue);
    

    printf("%s", Questions[13]);
    fgets(MultiUseValue, MAX_INPUT_LENGTH, stdin);
    CurrentTransformer.SecondariesDiameters = strToDoubleArrayWithLength(MultiUseValue);

    free(MultiUseValue);
  


    return CurrentTransformer;
}
void printQuestionAndArrayWLength(char* question,struct ArrayWithLength CurrentArrayWithLength){
    printf("%s \n", question);
    for(int i=0; i < CurrentArrayWithLength.Length ; i++){
        if (CurrentArrayWithLength.Array[i] == NULL){
            printf(" %d, -- \n",i);
        }
        else {
            printf("%d, %lf \n", i, *CurrentArrayWithLength.Array[i]);
        }
    }
}
void printQuestion(char *Question){
    printf("%s\n", Question);
}
void PrintQuestions(char **Questions){
    for(int i = 0; i < 14; i++){
        printQuestion(Questions[i]);
    }
}

void printTransformerStruct(char **Q_s, struct Transformer Trans){
    printQuestionAndArrayWLength( Q_s[0],Trans.PrimaryVoltage);
    printQuestionAndArrayWLength( Q_s[1],Trans.PrimaryCurrent);
    printQuestionAndArrayWLength( Q_s[2],Trans.PrimaryPower);
    printQuestionAndArrayWLength( Q_s[3],Trans.PrimaryDiameter);
    printQuestionAndArrayWLength( Q_s[4],Trans.PrimaryNumberOfTurns);
    printQuestionAndArrayWLength( Q_s[5],Trans.Q);
    printQuestionAndArrayWLength( Q_s[6],Trans.N);
    printQuestionAndArrayWLength( Q_s[7],Trans.Efficiency);
    printQuestionAndArrayWLength( Q_s[8],Trans.SecondaryPowerTotal);
    printQuestionAndArrayWLength( Q_s[9],Trans.SecondariesPowers);
    printQuestionAndArrayWLength( Q_s[10],Trans.SecondariesVoltages);
    printQuestionAndArrayWLength( Q_s[11],Trans.SecondariesCurrents);
    printQuestionAndArrayWLength( Q_s[12],Trans.SecondariesNumberOfTurns);
    printQuestionAndArrayWLength( Q_s[13],Trans.SecondariesDiameters);
    
}

void mainBeaviourFunction(){
    struct Transformer ListFilledByUser = fillTransformerList(QuestionsInOrder);
    printTransformerStruct(QuestionsInOrder,ListFilledByUser);

}

void main(int argc,char **argv) {

    mainBeaviourFunction();
   exit(0);
}

 

 // wsl -t Ubuntu-20.04  vypni Ubuntu


 // decision "Logical" function -> for all info in list all possible formulas [actually n + 1 because values 
 //calculations can be in bad order ] (all forms of formula) or end if you did not calculate any new value in last
 // cycle
 // try to calculate in every step of cycle  to get all values