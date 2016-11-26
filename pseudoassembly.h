/**@<pseudoassembly.h>::**/
#include <mypas.h>

/*unified label counter*/

extern int labelcounter;

/*control pseudo instructions*/

int gofalse(int label);
int jump (int label);
int mklabel (int label);
int lmovel (char const *variable);
int lmoveq (char const *variable);
int rmovel (char const *variable);
int rmoveq (char const *variable);

/*ULA pseudo-instructions*/

/*unary*/
int neglog(void);
int negint(void);
int negflt(void);
int negdbl(void);

/*binary addition and inversion*/
int addlog(int VAR1,int VAR2);
int addint(int VAR1,int VAR2);
int addflt(float VAR1,float VAR2);
int adddbl(double VAR1,double VAR2);
int subint(int VAR1,int VAR2);
int subflt(float VAR1,float VAR2);
int subdbl(double VAR1,double VAR2);

/*binary multiplication and inverse*/
int mullog(int VAR1,int VAR2);
int mulint(int VAR1,int VAR2);
int mulflt(float VAR1,float VAR2);
int muldbl(double VAR1,double VAR2);
int addflt(float VAR1,float VAR2);
int adddbl(double VAR1,double VAR2);
int divint(int VAR1,int VAR2);
int divflt(float VAR1,float VAR2);
int divdbl(double VAR1,double VAR2);
